#include "ota.hpp"
#include <Update.h>
const char *OtaManager::OTA_FILENAME = "/update.bin";

OtaManager ::OtaManager(function<void(bool, int)> onOtaFinished, function<void(int)> onProgessUpdate)
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
        totalParts = data[1] << 8 || data[2];
        if (FLASH.exists(OTA_FILENAME))
        {
            FLASH.remove(OTA_FILENAME);
        }
        break;
    }
    case NEXT_PACKET:
    {
        writeOtaPacket(data.c_str(), len - 1);
    }
    break;
    case LAST_PACKET:
    {
        writeOtaPacket(data.c_str(), len - 1);
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
    for (size_t i = 0; i < size; i++)
    {
        bytesToWrite[i] = bytes[i + 1];
    }
    File otaFile = FLASH.open(OTA_FILENAME, FILE_APPEND);
    otaFile.write(bytesToWrite, size);
    otaFile.close();
    processedParts += 1;
    if (onProgessUpdate != NULL)
    {
        onProgessUpdate((processedParts / totalParts) * 100);
    }
}

void OtaManager::updateSystem()
{
    File otaFile = FLASH.open(OTA_FILENAME);
    bool otaSuccessful;
    int error = NULL;
    if (Update.begin(otaFile.size()))
    {
        Update.writeStream(otaFile);
        otaSuccessful = Update.end() && Update.isFinished();
        if (!otaSuccessful)
        {
            int error = Update.getError();
        }
    }
    else
    {
        error = NOT_ENOUG_SPACE;
    }
    onOtaFinished(otaSuccessful, error);
}