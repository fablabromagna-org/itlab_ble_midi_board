#include "ota.hpp"
#include <Update.h>
#include <Arduino.h>
#include <CRC32.h>
const char *OtaManager::OTA_FILENAME = "/update.bin";

OtaManager ::OtaManager(function<void(bool, int)> onOtaFinished, function<void(float)> onProgessUpdate)
{
    this->onOtaFinished = onOtaFinished;
    this->onProgessUpdate = onProgessUpdate;
}

void OtaManager::onOTADataReceived(std::string data)
{
    char commandData = data[0];
    int len = data.length();

    switch (commandData)
    {
    case INFO_PACKET_COMMAND:
    {
        totalParts = (data[1] << 8) | data[2];
        Serial.print("Update total parts:");
        Serial.println(totalParts);

        FLASH.begin(true);
        if (FLASH.exists(OTA_FILENAME))
        {
            FLASH.remove(OTA_FILENAME);
        }
        break;
    }
    case NEXT_PACKET:
    {
        writeOtaPacket(data.substr(1, len - 1).c_str(), len - 1);
    }
    break;
    case LAST_PACKET:
    {
        writeOtaPacket(data.substr(1, len - 1).c_str(), len - 1);
        updateSystem();
    }
    break;
    default:
        break;
    }
}

void OtaManager::writeOtaPacket(const char *bytes, int size)
{
    uint8_t bytesToWrite[size];
    memcpy(bytesToWrite, bytes, size);
    File otaFile = FLASH.open(OTA_FILENAME, FILE_APPEND);
    otaFile.write(bytesToWrite, size);
    otaFile.close();
    processedParts += 1;
    if (onProgessUpdate != NULL)
    {
        onProgessUpdate(((float)processedParts) / totalParts);
    }
}

void OtaManager::updateSystem()
{
    File otaFile = FLASH.open(OTA_FILENAME);
    bool otaSuccessful;
    int error = NULL;
    Serial.print("Update file size: ");
    Serial.println(otaFile.size());
    if (Update.begin(otaFile.size()))
    {
        Update.writeStream(otaFile);
        otaSuccessful = Update.end();
        if (!otaSuccessful)
        {
            error = Update.getError();
        }
    }
    else
    {
        error = NOT_ENOUG_SPACE;
    }
    Serial.println(Update.errorString());
    onOtaFinished(otaSuccessful, error);
}