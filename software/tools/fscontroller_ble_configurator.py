#!/usr/bin/env python3

# This Python file uses the following encoding: utf-8

import sys
import os
import argparse
import time
import asyncio
from bleak import BleakScanner
from bleak import BleakClient
from fscontroller_config_converter import FSControllerConfigParser


class FSControllerBle:
    CONFIG_UUID = "ae0f58f2-43c5-11ec-81d3-0242ac130003"
    
    async def scan_ble(self):
        devices = await BleakScanner.discover()
        for d in devices:
            print(d)


    async def read_config(self, device_address, bin_filename):
        async with BleakClient(device_address) as client:
            current_config = await client.read_gatt_char(self.CONFIG_UUID)        
            print (current_config)
            
            new_file = open(bin_filename, "wb")
            new_file.write(current_config)
            
        
    async def write_config_bin(self, device_address, bin_filename):
        bin_config = bytearray()
        
        # for i in range(1,100):
        #     bin_config.append(i)
            
        try:
            f = open(bin_filename, 'rb')
            while (rbyte := f.read(1)):
                bin_config += rbyte

        except IOError:
            print('Error Opening the file!')
            return
        

        print(bin_config)
        
        async with BleakClient(device_address) as client:
            await client.write_gatt_char(self.CONFIG_UUID, bin_config)        
            
        


if __name__ == "__main__":
    # Instantiate the parser
    parser = argparse.ArgumentParser(description='FLR BLE Controller - GATT read/write tool ')

    # mode

    parser.add_argument('--scan', action='store_true',
                    help='Scan for BLE devices')
    
    parser.add_argument('--read_name', action='store_true',
                    help='Read the BLE controller name')

    parser.add_argument('--write_name', type=str, default='',
                    help='Read the BLE controller name (max 30 chars)')


    parser.add_argument('--device', type=str, default='30:ae:a4:21:2d:d2',
                    help='Use this device (MAC)')

    # parser.add_argument('--read_config', action='store_true',
    #                 help='Read the BLE controller configuration (binary)')

    parser.add_argument('--read_config_bin', type=str, default='',
                    help='Read config from device and store it on passed file')

    parser.add_argument('--write_config_bin', type=str, default='',
                    help='Write the binary config content of passed file to device')

    parser.add_argument('--write_config_json', type=str, default='',
                    help='Convert the JSON passed file to bin and write to device')
    
    args = parser.parse_args()

    if (args.scan): 
        asyncio.run(FSControllerBle().scan_ble())
    else :
        # print(args.device)
        if (len(args.device) <= 0):
            print("You need to specify the device")
        else:
            if (args.read_name):
                print("no yet implemented 1")
            elif (args.write_name):
                print("no yet implemented 2")
           
            elif (args.read_config_bin):
                asyncio.run(FSControllerBle().read_config(args.device, args.read_config_bin))
            elif (args.write_config_bin):
                asyncio.run(FSControllerBle().write_config_bin(args.device, args.write_config_bin))
                
            elif (args.write_config_json):
                bin_config = FSControllerConfigParser().process_config(2, args.write_config_json)

                bin_config_file = args.write_config_json + ".bin"
                new_file = open(bin_config_file, "wb")
                new_file.write(bin_config)
                new_file.close()
                asyncio.run(FSControllerBle().write_config_bin(args.device, bin_config_file))                
            else:
                parser.print_help()




# address = "30:AE:A4:21:2D:D2"
# BLE_CONFIG_UUID = "ae0f58f2-43c5-11ec-81d3-0242ac130003"

# async def main(address):
#     async with BleakClient(address) as client:
#         current_config = await client.read_gatt_char(BLE_CONFIG_UUID)
#         print("Current Configuration: ")
#         print(current_config)

# asyncio.run(main(address))
