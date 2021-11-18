#!/usr/bin/env python3

# This Python file uses the following encoding: utf-8

import sys
import os
import json
import argparse



class FSControllerConfigParser:
    ### Mode:
    # 1: read JSON file, output as C init variable
    # 2: read JSON file, output as binary file
    # 10: read binary file, output as json file

    def process_config(self,mode, filename):
        out_bin = self.json2bin(mode, filename)
        
        if (mode == 1):
            return ','.join('0x{:02x}'.format(x) for x in out_bin)
        elif (mode == 2):
            return out_bin
            

    def json2bin(self, mode, filename):
        with open(filename) as f:
            data = json.load(f)
        
        # json to bin
        str_name = "FLR BLE MIDI Controller"
        enc_str = str_name.encode()
        frame_out = bytearray(enc_str)
        for i in range(32 - len(str_name)):
            frame_out.append(0x00)
        
        # TODO: da gestire
        # per ora fisso... poi vedremo
        # version
        frame_out.append(1)
        frame_out.append(2)
        
        frame_out.append(data["footswitch_nr"])
        
        frame_out.append(1 if (data["ble_mode"] == "server") else 2 )
        for fsnr in range(data["footswitch_nr"]):
            fs_config = data["fs_config"][fsnr]
            # frame_out.append(fs_config["fs_id"])

            for detail in ("tap", "hold"):
                # Control byte
                frame_out.append(0x80)
                
                detail_config = fs_config[detail]
                
                if detail_config["event"] == "single":
                    frame_out.append(1)    # event
                    frame_out.append(detail_config["midi_channel"])    # midi_ch
                    frame_out.append(0x01 if detail_config["midi_type"] == "cc" else 2)    # midi_type
                    frame_out.append(detail_config["midi_nr"])    # midi_nr
                    frame_out.append(detail_config["midi_value"])    # midi_value_on
                    
                    for i in range(9):
                        frame_out.append(0x00)

                
                elif detail_config["event"] == "repeat":
                    frame_out.append(2)    # event
                    frame_out.append(detail_config["midi_channel"])    # midi_ch
                    frame_out.append(0x01 if detail_config["midi_type"] == "cc" else 2)    # midi_type
                    frame_out.append(detail_config["midi_nr"])    # midi_nr
                    frame_out.append(detail_config["midi_value"])    # midi_value_on
                    frame_out.append(0x00)    # midi_value_off
                    
                    # interval_bytes = detail_config["interval"].to_bytes(2, byteorder='big', signed=True)
                    # frame_out.append(interval_bytes[0])    # repeat_interval
                    # frame_out.append(interval_bytes[1])    # repeat_interval

                    frame_out.append(detail_config["interval"]/10)
                    
                    for i in range(6):
                        frame_out.append(0x00)
                    
                elif detail_config["event"] == "increment":
                    frame_out.append(3)    # event
                    frame_out.append(detail_config["midi_channel"])    # midi_ch
                    frame_out.append(0x01 if detail_config["midi_type"] == "cc" else 0x02)    # midi_type
                    frame_out.append(detail_config["midi_nr"])    # midi_nr
                    frame_out.append(0x00)    # midi_value_on
                    frame_out.append(0x00)    # midi_value_off

                    try :
                        frame_out.append(detail_config["interval"]/10)
                        # interval_bytes = detail_config["interval"].to_bytes(2, byteorder='big', signed=True)
                    except :
                        frame_out.append(0x00)
                        # interval_bytes = 0x00.to_bytes(2, byteorder='big', signed=True)
                
                    # frame_out.append(interval_bytes[0])    # repeat_interval
                    # frame_out.append(interval_bytes[1])    # repeat_interval

                    frame_out.append(0x00)    # group_idx
                    frame_out.append(detail_config["variable"])    # intval_idx
                    frame_out.append(detail_config["min"])    # intval_min
                    frame_out.append(detail_config["max"])    # intval_max
                    
                    frame_out.append(1 if detail_config["step"] > 0 else 0)    # intval_step is positive?
                    frame_out.append(abs(detail_config["step"]))    # intval_step
                    frame_out.append(0x01 if detail_config["cycle"] else 0x00)    # uint8_t cycle;
                    

                elif detail_config["event"] == "on_off":
                    frame_out.append(4)    # event
                    frame_out.append(detail_config["midi_channel"])    # midi_ch
                    frame_out.append(0x01 if detail_config["midi_type"] == "cc" else 0x02)    # midi_type
                    frame_out.append(detail_config["midi_nr"])    # midi_nr
                    frame_out.append(detail_config["midi_value_on"])    # midi_value_on
                    frame_out.append(detail_config["midi_value_off"])    # midi_value_off
                    frame_out.append(0x00)    # repeat_interval
                    frame_out.append(0x00)    # repeat_interval
                    frame_out.append(detail_config["group"])    # group_idx
                    frame_out.append(0x00)    # intval_idx
                    frame_out.append(0x00)    # intval_min
                    frame_out.append(0x00)    # intval_max
                    frame_out.append(0x00)    # intval_step
                    frame_out.append(0x00)    # uint8_t cycle;
                    
                else:
                    for i in range(14):
                        frame_out.append(0x00)
                
            
                # Control byte
                frame_out.append(0x88)
        
        return frame_out
            
        


if __name__ == "__main__":
    # Instantiate the parser
    parser = argparse.ArgumentParser(description='FLR BLE Controller - Configuration converter tool ')

    # mode
    parser.add_argument('--json2c', action='store_true',
                    help='read JSON file, output as C init variable')

    parser.add_argument('--json2bin', action='store_true',
                    help='read JSON file, output as binary stream')

    parser.add_argument('file_name', type=str,
                    help='The input file name')
    
    args = parser.parse_args()
    
    process_mode = 0
    if (args.json2c): 
        process_mode = 1
        
    elif (args.json2bin): 
        process_mode = 2

    out = FSControllerConfigParser().process_config(process_mode, args.file_name)

    # print (','.join('0x{:02x}'.format(x) for x in out))
    print (out)