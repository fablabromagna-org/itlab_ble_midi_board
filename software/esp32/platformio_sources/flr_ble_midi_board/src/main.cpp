/**
 * FablabRomagna BLE MIDI Board
 * Copyright (C) 2021 FablabRomagna (posta@fablabromagna.org) 
 * 
 * [https://github.com/fablabromagna-org/itlab_ble_midi_board]
 * 
 *
 * A BLE (Bluetooth Low Energy) MIDI implementation based on ESP32
 * 
 * This source is part of a project to build a BLE MIDI Pedal Board
 * 
 * We created this project to control music and guitar applications,
 * but you can use it for any BLE MIDI compatible application ;)
 * 
 * You can find the full project documentation on github page
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
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#include <MidiHelper.h>
#include <FootSwitch.h>
#include <FootSwitchController.h>


#define SERVICE_MIDI_UUID        "03b80e5a-ede8-4b33-a751-6ce34ec4c700"  // The MIDI Service
#define CHARACTERISTIC_MIDI_UUID "7772e5db-3868-4112-a1a9-f2669d106bf3"  // The MIDI Characteristic

#define SERVICE_CONFIGURATION_UUID        "98611bc6-43c5-11ec-81d3-0242ac130003"
#define CHARACTERISTIC_CONFIGURATION_UUID "ae0f58f2-43c5-11ec-81d3-0242ac130003"

#define SERVICE_BATTERY_UUID  BLEUUID((uint16_t)0x180F)
#define CHARACTERISTIC_BATTERY_UUID BLEUUID((uint16_t)0x2A19)


typedef  enum FsEvent {
        FS_EVENT_NONE,
        FS_EVENT_TAP,
        FS_EVENT_HOLD
    } FsEvent;
// #define ARRAY_LENGTH(x) (sizeof(x)/sizeof(x[0]))

BLEServer *pServer = NULL;
BLECharacteristic *pCharacteristicMidi, *pCharacteristicConfiguration, *pCharacteristicBattery;
FootSwitch  *footswitchArray;
FootSwitchController  footSwitchController;


// TODO: this vars should be moved to config
const int midi_channel = 1;
const int fs_number = 4;

// TODO: read from config or assign fixed for max footswitch managed (8?)

volatile int fs_events[fs_number] = {FS_EVENT_NONE, FS_EVENT_NONE, FS_EVENT_NONE, FS_EVENT_NONE};

const int fs_input_pin[fs_number] = {2,4,34,35};

const int fs1_led_pin = 16;     
const int fs2_led_pin = 17;
const int fs3_led_pin = 25;
const int fs4_led_pin = 26;

bool bleDeviceConnected = false;
bool oldBleDeviceConnected = false;

typedef void (*isr_callback)();

struct hardware_interrupt
{
  int pin_no;
  isr_callback isr;
};


// int cc_code_prev = 0;


void send_midi_command(MidiHelper::MidiMessage midi_message) {
    if (midi_message.length > 0) {
      Serial.println(" MIDI Message: ");
      Serial.println(midi_message.content[2]);  // CH and TYPE 
      Serial.println(midi_message.content[3]);  // CC NR
      Serial.println(midi_message.content[4]);  // CC Value

      pCharacteristicMidi->setValue(midi_message.content, midi_message.length); 
      pCharacteristicMidi->notify();
      vTaskDelay(100/portTICK_PERIOD_MS);
    }
    else {
      Serial.println ("No MIDI message to send  ");     
    }

}

void send_midi_command(MidiHelper::MidiMessageType msg_type, u_char midi_channel, u_char midi_cmd_nr, u_char midi_cmd_value) {

  //  TODO: inserire il controllo sul range dei comandi 

  MidiHelper midiHelper;
  MidiHelper::MidiMessage midi_message;

  if ( midiHelper.buildMidiCommand(msg_type, midi_channel,midi_cmd_nr, midi_cmd_value , &midi_message) ) {

    // Serial.println("Sending MIDI message ");
    // Serial.println(midi_message.length);
    // for (int i=0 ; i< midi_message.length; i++) {
    //   Serial.println(midi_message.content[i]);
    // }
    
    pCharacteristicMidi->setValue(midi_message.content, midi_message.length); 
    pCharacteristicMidi->notify();
    vTaskDelay(100/portTICK_PERIOD_MS);
  }
 }


void process_tap_event(uint8_t fs_nr) {
  Serial.print ("Processing TAP Event   " ); 
  Serial.println ( fs_nr); 

  // TODO: gestire invio MIDI sulla base della configurazione
  //send_midi_command(MidiHelper::MIDI_CC, 1 , 12, 1);

  send_midi_command(footSwitchController.processEvent(fs_nr, FootSwitch::FS_TAP));

  //Serial.print(footSwitchController.debugThis());
  
  // send_midi_command(footSwitchController.processEvent1(fs_nr, FootSwitch::FS_TAP));
}

void process_hold_event(uint8_t fs_nr) {
  // TODO: gestire invio MIDI sulla base della configurazione

  Serial.print("Processing HOLD Event   "); 
  Serial.println(fs_nr); 
  send_midi_command(footSwitchController.processEvent(fs_nr, FootSwitch::FS_HOLD));
  // send_midi_command(MidiHelper::MIDI_CC, 1, 18, 5);
  
  //Serial.print(footSwitchController.debugThis());
}


void process_interrupt(uint8_t fs_nr, bool new_state_pressed) {
  
  FootSwitch *my_footswitch = &footswitchArray[fs_nr-1];
 
  if ((new_state_pressed)  && (!my_footswitch->isPressed())) {
    if  (my_footswitch->press(millis())) {
      // Serial.println("PRESSED "); 
    }
  }
  else if ((!new_state_pressed)  && (my_footswitch->isPressed())) {
    bool tap_event = false;

    if (my_footswitch->release(millis(), &tap_event)) {
      // Serial.println("RELEASED ");
    }

    if (tap_event) {
      fs_events[fs_nr] = FS_EVENT_TAP;
      // Serial.println("TAP EVENT");
      // process_tap_event(fs_nr);
    }
  }
}

void isr1(){
  // Serial.println("ISR 1");
  process_interrupt(1, digitalRead(fs_input_pin[0])); 
}
void isr2(){ process_interrupt(2, digitalRead(fs_input_pin[1])); }
void isr3(){ process_interrupt(3, digitalRead(fs_input_pin[2])); }
void isr4(){ process_interrupt(4, digitalRead(fs_input_pin[3])); }

hardware_interrupt isrArray[] = 
{
  {fs_input_pin[0], isr1},
  {fs_input_pin[1], isr2},
  {fs_input_pin[2], isr3},
  {fs_input_pin[3], isr4}
};

/** The BLE charact. callback to process new incoming device configuration and send the current one **/
class ConfigCallbacks: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    Serial.println("**** WRITE CONFIG *****");

    uint8_t config_bin[512];
    uint8_t *data_in;
    data_in = pCharacteristic->getData();
    Serial.println(*data_in, HEX);

    std::string rxValue = pCharacteristic->getValue();

    if (rxValue.length() > 0) {
      Serial.println("*********");
      Serial.print("Received Value: ");
      for (int i = 0; i < rxValue.length(); i++)
        Serial.print(rxValue[i]);

      //TODO: important!!! insert some check of packet (integrity and well formed packet)
      footSwitchController.processBinaryConfiguration((uint8_t*)rxValue.c_str(), rxValue.length(), true);

    }
    



  }

  void onRead(BLECharacteristic *pCharacteristic) {
    Serial.println("**** READ CONFIG *****");
   
    pCharacteristic->setValue(footSwitchController.getBinConfiguration(), sizeof(FootSwitchController::ControllerConfiguration));
  }

};


void setup() {

  Serial.begin(115200);

  Serial.println("FablabRomagna BLE MIDI Controller");

// Launch SPIFFS file system  
  if(!SPIFFS.begin()){ 
    Serial.println("An Error has occurred while mounting SPIFFS");  
  }

  footSwitchController.loadBinaryConfiguration();


  footswitchArray = new FootSwitch[fs_number]; 

  /** Init the DIGITAL INPUT **/
  for (int idx = 0; idx<fs_number; idx++) {
    pinMode(fs_input_pin[idx], INPUT);
  }
 
  /** Init the ISR for the footswitches **/
  for( auto& iSR: isrArray)
  {
    attachInterrupt(digitalPinToInterrupt(iSR.pin_no), iSR.isr, CHANGE);
  }


  /** Init the BLE Service **/ 
  Serial.println("Starting BLE Server");
  BLEDevice::init("FLR_MIDI_Board");

  pServer = BLEDevice::createServer();

  class MyServerCallbacks: public BLEServerCallbacks {
      void onConnect(BLEServer* pServer) {
        bleDeviceConnected = true;
      };

      void onDisconnect(BLEServer* pServer) {
        bleDeviceConnected = false;
      }
  };

  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pServiceMidi = pServer->createService(SERVICE_MIDI_UUID);
  pCharacteristicMidi = pServiceMidi->createCharacteristic(
                                         CHARACTERISTIC_MIDI_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_NOTIFY |
                                         BLECharacteristic::PROPERTY_WRITE_NR
                                       );

  BLEService *pServiceConfig = pServer->createService(SERVICE_CONFIGURATION_UUID);
  pCharacteristicConfiguration = pServiceConfig->createCharacteristic(
                                         CHARACTERISTIC_CONFIGURATION_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_NOTIFY |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );
  
  pCharacteristicConfiguration->setCallbacks(new ConfigCallbacks());
  

  BLEService *pServiceBattery = pServer->createService(SERVICE_BATTERY_UUID);

  pCharacteristicBattery = pServiceBattery->createCharacteristic(
                                         CHARACTERISTIC_BATTERY_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_NOTIFY |
                                         BLECharacteristic::PROPERTY_WRITE_NR
                                       );


  pServiceMidi->start();
  pServiceConfig->start();
  pServiceBattery->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_MIDI_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now you can read it in your phone!");
}


void loop() {

  if (bleDeviceConnected) {
    for (uint8_t idx = 0; idx<fs_number; idx++) {
      if (footswitchArray[idx].checkHold(millis())) {
        //process_hold_event(idx+1);
      }


      if (fs_events[idx] == FS_EVENT_TAP) {
        fs_events[idx] = FS_EVENT_NONE;
        Serial.println("TAP EVENT");
        process_tap_event(idx);
      }

    //TODO: gestire il caso di HOLD_REPEAT !!!
      if (footswitchArray[idx].isHold()) {
        //process_hold_event(idx+1);
      }

      

    }

  
  
	}

  // disconnecting
  if (!bleDeviceConnected && oldBleDeviceConnected) {
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println(" BLEstart advertising");
        oldBleDeviceConnected = bleDeviceConnected;
    }
  // connecting
  if (bleDeviceConnected && !oldBleDeviceConnected) {
    Serial.println("BLE Connecting");
    // do stuff here on connecting
    oldBleDeviceConnected = bleDeviceConnected;
  }

  delay(50); // bluetooth stack will go into congestion, if too many packets are sent

}