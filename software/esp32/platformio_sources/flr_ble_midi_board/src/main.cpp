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
#define SERVICE_UUID        "03b80e5a-ede8-4b33-a751-6ce34ec4c700"  // The MIDI Service
#define CHARACTERISTIC_UUID "7772e5db-3868-4112-a1a9-f2669d106bf3"  // The MIDI Characteristic

#define DEBOUNCE_TIME 1  //ms

#define HOLD_TIME  1000  //ms


// TODO: this vars should be moved to config
const int midi_channel = 1;
const int fs_number = 4;

// TODO: read from config or assign fixed for max footswitch managed (8?)
const int fs_input_pin[fs_number] = {14,27,12,13};

// const int fs1_input_pin = 36;     
// const int fs2_input_pin = 39;
// const int fs3_input_pin = 39;
// const int fs4_input_pin = 39;

const int fs1_led_pin = 36;     
const int fs2_led_pin = 39;
const int fs3_led_pin = 39;
const int fs4_led_pin = 39;


const int cc_code_btn1 = 0x20;     
const int cc_code_btn2 = 0x21;     
const int cc_code_btn3 = 0x22;     


#define ARRAY_LENGTH(x) (sizeof(x)/sizeof(x[0]))

typedef void (*isr_callback)();

struct hardware_interrupt
{
  uint8_t pin_no;
  isr_callback isr;
};


struct footswitch
{
  bool pressed = false;
  bool hold = false;
  bool group_active = false;
  unsigned long t_pressed = 0;
  unsigned long t_released = 0;
  unsigned long t_last_repeat = 0;
};


footswitch  footswitchArray[fs_number]; 

BLECharacteristic *pCharacteristic;
int cc_code_prev = 0;

void send_midi_command(bool isCC, u_char midi_channel, u_char midi_cmd_nr, u_char midi_cmd_value) {

  //  TODO: inserire il controllo sul range dei comandi 

  
  uint8_t midiPacket[] = {
    0x80,  // header
    0x80,  // timestamp, not implemented 
    0x00,  // type + channel
    0x3c,  // data nr - key note: 0x3c == 60 == middle c
    0x00   // data value
  };

  /** Send the CC MIDI message ON **/
  midiPacket[2] = 0xB0 + midi_channel -1; // CC on channel midi_channel
  midiPacket[3] = midi_cmd_nr; // Control number
  midiPacket[4] = midi_cmd_value;    // Control value
  Serial.println("sending MIDI packet ..."); 
  
  pCharacteristic->setValue(midiPacket, 5); // packet, length in bytes
  pCharacteristic->notify();
  vTaskDelay(100/portTICK_PERIOD_MS);

}


void process_tap_event(uint8_t fs_nr) {
  Serial.println ("Processing TAP Event   " + fs_nr); 

  // TODO: gestire invio MIDI sulla base della configurazione

  send_midi_command(true, 1 , 12, 1);

}

void process_hold_event(uint8_t fs_nr) {
  // TODO: gestire invio MIDI sulla base della configurazione

  Serial.println("Processing HOLD Event   " + fs_nr); 

  send_midi_command(true, 1, 18, 5);
}


void process_interrupt(uint8_t fs_nr, bool new_state_pressed) {
  footswitch *my_footswitch = &footswitchArray[fs_nr-1];

  // Serial.print("Processing Interrupt "); 
  // Serial.print(fs_nr);
  // Serial.println(new_state_pressed?"P":"R"); 

 

  if ((new_state_pressed)  && (!my_footswitch->pressed)) {
    if ((millis() - my_footswitch->t_released) > DEBOUNCE_TIME) {
      my_footswitch->t_pressed = millis();
      my_footswitch->pressed = true;

      Serial.println("PRESSED "); 
    }
  }
  else if ((!new_state_pressed)  && (my_footswitch->pressed)) {
    if ((millis() - my_footswitch->t_pressed) > DEBOUNCE_TIME) {
      my_footswitch->t_released = millis();
      my_footswitch->pressed = false;
      my_footswitch->hold = false;
      Serial.println("RELEASED "); 

      if ( (my_footswitch->t_released - my_footswitch->t_pressed) < HOLD_TIME) {
        process_tap_event(fs_nr);
      }
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






void setup() {

  Serial.begin(115200);

  Serial.println("FablabRomagna BLE MIDI Controller");

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
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_NOTIFY |
                                         BLECharacteristic::PROPERTY_WRITE_NR
                                       );

  pService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now you can read it in your phone!");
}



void loop() {


  for (uint8_t idx = 0; idx<fs_number; idx++) {
    if ((footswitchArray[idx].pressed) && 
        (!footswitchArray[idx].hold) && 
        ((millis() - footswitchArray[idx].t_pressed) >= HOLD_TIME )
       ) {
         footswitchArray[idx].hold = true;
         process_hold_event(idx+1);
    }
  }
  
  //TODO: gestire il caso di HOLD_REPEAT !!!

 
  
  
  // if ((cc_code > 0) && (cc_code != cc_code_prev)) {
  //   /** Send the CC MIDI message ON **/
  //   midiPacket[2] = 0xB0 + midi_channel -1; // CC on channel midi_channel
  //   midiPacket[3] = cc_code; // Control number
  //   midiPacket[4] = 0x01;    // Control value
  //   Serial.println("sending MIDI packet ..."); 
    
  //   pCharacteristic->setValue(midiPacket, 5); // packet, length in bytes
  //   pCharacteristic->notify();
  //   vTaskDelay(100/portTICK_PERIOD_MS);
 
  // }
  // if ((cc_code == 0) && ( cc_code_prev>0)) {
    
  //   /** Send the CC MIDI message OFF **/
  //   midiPacket[2] = 0xB0 + midi_channel - 1; // CC on channel midi_channel
  //   midiPacket[3] = cc_code_prev; // Control number
  //   midiPacket[4] = 0x00;         // Control value
  //   Serial.println("sending MIDI packet ..."); 
    
  //   pCharacteristic->setValue(midiPacket, 5); // packet, length in bytes
  //   pCharacteristic->notify();
  //   vTaskDelay(100/portTICK_PERIOD_MS);

  // }
  // cc_code_prev = cc_code;


  delay(50);
}