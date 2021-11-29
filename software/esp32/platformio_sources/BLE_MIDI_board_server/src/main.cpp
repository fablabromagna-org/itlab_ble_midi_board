/**
 * ITLab BLE MIDI Board
 * Copyright (C) 2020 Ivan Tarozzi (itarozzi@gmail.com) 
 * 
 * [https://github.com/itarozzi/itlab_ble_midi_board]
 * 
 *
 * A BLE (Bluetooth Low Energy) MIDI implementation based on ESP32
 * 
 * This source is part of a project to build a BLE MIDI Pedal Board
 * I created this project to control music and guitar applications,
 * but you can use it for any BLE MIDI compatible application ;)
 * 
 * You can find the full project documentation on github page:
 * 
 * -----------------------------------------------------------------
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * Credits:
 * -------- 
 * Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleServer.cpp
 * Ported to Arduino ESP32 by Evandro Copercini
 *  updates by chegewara
*/

#include <Arduino.h>
#include <NimBLEDevice.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include "ota/ota.hpp"

#define MAX_OTA_PROGRESS 100
#define MAX_NUMBER_OF_LCD_CHARS 16

#define SERVICE_UUID "03b80e5a-ede8-4b33-a751-6ce34ec4c700"        // The MIDI Service
#define CHARACTERISTIC_UUID "7772e5db-3868-4112-a1a9-f2669d106bf3" // The MIDI Characteristic

#define OTA_SERVICE_UUID "fb1e4001-54ae-4a28-9f74-dfccb248601d"
#define OTA_CHARACTERISTIC_UUID_RX "fb1e4001-54ae-4a28-9f74-dfccb248601d"
#define OTA_CHARACTERISTIC_UUID_TX "fb1e4001-54ae-4a28-9f74-dfccb248601d"

#define BUILTINLED 2

const int midi_channel = 1;

const int buttonPin1 = 34;
const int buttonPin2 = 35;
const int VCC3v3 = 23;

const int cc_code_btn1 = 0x20;
const int cc_code_btn2 = 0x21;
const int cc_code_btn3 = 0x22;

BLECharacteristic *pCharacteristic;
BLECharacteristic *otaCharacteristic;

int cc_code_prev = 0;

enum MODES
{
  NORMAL_MODE,
  UPDATE_MODE,
  OTA_MODE
};

bool deviceConnected = false;
int MODE = NORMAL_MODE;

LiquidCrystal lcd(14, 27, 26, 25, 33, 32);

uint8_t midiPacket[] = {
    0x80, // header
    0x80, // timestamp, not implemented
    0x00, // status
    0x3c, // data - key note: 0x3c == 60 == middle c
    0x00  // velocity
};

void onOtaProgress(float progress)
{
  lcd.setCursor(0, 0);
  lcd.println("Updating");
  lcd.setCursor(0, 1);

  int numberOfHash = progress * MAX_NUMBER_OF_LCD_CHARS;
  for (size_t i = 0; i < numberOfHash; i++)
  {
    lcd.print("#");
  }
  Serial.print("Updating progress: ");
  Serial.println(progress);
}

void onOtaFinished(bool isSuccessful, int error)
{
  lcd.setCursor(0, 0);

  if (isSuccessful)
  {
    lcd.print("Update success!");
  }
  else
  {
    lcd.print("Update failed..");
  }
  for (int i = 0; i < 5; i++)
  {
    lcd.setCursor(0, 1);
    lcd.printf("Restart in: %d", 5 - i);
    delay(1000);
  }
  ESP.restart();
}

OtaManager otaManager(onOtaFinished, onOtaProgress);

class ServerCallbacks : public BLEServerCallbacks
{
  void onConnect(BLEServer *pServer)
  {
    deviceConnected = true;
    Serial.println("Device connected");
    digitalWrite(BUILTINLED, HIGH);
  }
  void onDisconnect(BLEServer *pServer)
  {
    deviceConnected = false;
    Serial.println("Device disconnected");
    digitalWrite(BUILTINLED, LOW);
  }
};

class OTACallbacks : public BLECharacteristicCallbacks
{
  void onRead(BLECharacteristic *pCharacteristic)
  {
    Serial.print(pCharacteristic->getUUID().toString().c_str());
    Serial.print(": onRead(), value: ");
    Serial.println(pCharacteristic->getValue().c_str());
  };

  void onNotify(BLECharacteristic *pCharacteristic)
  {
    //uint8_t* pData;
    std::string pData = pCharacteristic->getValue();
    int len = pData.length();
    //        Serial.print("Notify callback for characteristic ");
    //        Serial.print(pCharacteristic->getUUID().toString().c_str());
    //        Serial.print(" of data length ");
    //        Serial.println(len);
    Serial.print("TX  ");
    for (int i = 0; i < len; i++)
    {
      Serial.printf("%02X ", pData[i]);
    }
    Serial.println();
  }

  void onWrite(BLECharacteristic *pCharacteristic)
  {
    //uint8_t* pData;
    std::string pData = pCharacteristic->getValue();
    // Serial.print("Write callback for characteristic ");
    // Serial.print(pCharacteristic->getUUID().toString().c_str());
    // Serial.print(" of data length ");
    // Serial.println(len);
    // Serial.print("RX  ");
    // for (int i = 0; i < len; i++)
    // { // leave this commented
    //   Serial.printf("%02X ", data[i]);
    // }
    // Serial.println();
    otaManager.onOTADataReceived(pData);
  }
};

/**
 * In this first version I use a DigiTech FS3X footswitch connected to a Wemos D1 R32 ESP32 board
 * 
 * The FS3X has a 6,35mm Jack Stereo output where pins are connected as described in github page.
 * I pulled up 2 DI of ESP32 and I use the tip and the ring of the Jack to pull low the ESP32 inputs
 * 
 * When the UP switch is pressed, both inputs are pulled down
 * 
 * This version use a fixed MIDI channel (1) and use a CC (Control Change) command with values:
 *    1 -> switch pressed
 *    0 -> switch released
 * 
 */
void setup()
{
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  lcd.begin(16, 2);
  lcd.print("ITLAB MIDI BLE");

  /** Init the DIGITAL INPUT **/
  pinMode(BUILTINLED, OUTPUT);
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(VCC3v3, OUTPUT);
  digitalWrite(VCC3v3, HIGH);

  /** Init the BLE Service **/
  BLEDevice::init("ITLabMIDI_Board");
  BLEDevice::setMTU(517);
  BLEDevice::setPower(ESP_PWR_LVL_P9);
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *otaService = pServer->createService(OTA_SERVICE_UUID);
  otaCharacteristic = otaService->createCharacteristic(
      OTA_CHARACTERISTIC_UUID_TX,
      NIMBLE_PROPERTY::READ |
          NIMBLE_PROPERTY::NOTIFY |
          NIMBLE_PROPERTY::WRITE_NR |
          NIMBLE_PROPERTY::WRITE);

  otaCharacteristic->setCallbacks(new OTACallbacks());
  otaCharacteristic->addDescriptor(new BLE2904());

  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
      CHARACTERISTIC_UUID,
      NIMBLE_PROPERTY::READ |
          NIMBLE_PROPERTY::NOTIFY |
          NIMBLE_PROPERTY::WRITE_NR);

  pServer->setCallbacks(new ServerCallbacks());
  otaService->start();
  pService->start();
  pServer->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  //pAdvertising->addServiceUUID(OTA_SERVICE_UUID);
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  pAdvertising->start();
  Serial.println("Characteristic defined! Now you can read it in your phone!");
}

void loop()
{

  if (MODE == NORMAL_MODE)
  {
    int buttonState1 = !digitalRead(buttonPin1);
    int buttonState2 = !digitalRead(buttonPin2);
    int cc_code = 0;

    if (!buttonState1 && !buttonState2)
    {
      cc_code = cc_code_btn3;
    }
    else if (!buttonState1)
    {
      cc_code = cc_code_btn1;
    }
    else if (!buttonState2)
    {
      cc_code = cc_code_btn2;
    }

    if ((cc_code > 0) && (cc_code != cc_code_prev))
    {
      /** Send the CC MIDI message ON **/
      midiPacket[2] = 0xB0 + midi_channel - 1; // CC on channel midi_channel
      midiPacket[3] = cc_code;                 // Control number
      midiPacket[4] = 0x01;                    // Control value
      Serial.println("sending MIDI packet ...");

      pCharacteristic->setValue(midiPacket, 5); // packet, length in bytes
      pCharacteristic->notify();
      vTaskDelay(100 / portTICK_PERIOD_MS);
    }
    if ((cc_code == 0) && (cc_code_prev > 0))
    {

      /** Send the CC MIDI message OFF **/
      midiPacket[2] = 0xB0 + midi_channel - 1; // CC on channel midi_channel
      midiPacket[3] = cc_code_prev;            // Control number
      midiPacket[4] = 0x00;                    // Control value
      Serial.println("sending MIDI packet ...");

      pCharacteristic->setValue(midiPacket, 5); // packet, length in bytes
      pCharacteristic->notify();
      vTaskDelay(100 / portTICK_PERIOD_MS);
    }
    cc_code_prev = cc_code;

    delay(50);
  }
}
