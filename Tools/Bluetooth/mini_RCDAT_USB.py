#! /usr/bin/env python3

###############################################################################
 #
 # Copyright (C) 2022-2023 Maxim Integrated Products, Inc. (now owned by
 # Analog Devices, Inc.),
 # Copyright (C) 2023-2024 Analog Devices, Inc.
 #
 # Licensed under the Apache License, Version 2.0 (the "License");
 # you may not use this file except in compliance with the License.
 # You may obtain a copy of the License at
 #
 #     http://www.apache.org/licenses/LICENSE-2.0
 #
 # Unless required by applicable law or agreed to in writing, software
 # distributed under the License is distributed on an "AS IS" BASIS,
 # WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 # See the License for the specific language governing permissions and
 # limitations under the License.
 #
 ##############################################################################

## mini_RCDAT_USB.py
 #
 # Sets the attenuation of a Mini-Circuits RCDAT USB attenuator
 #

import usb.core
import usb.util
import argparse
from argparse import RawTextHelpFormatter
import sys

# Set the VID and PID that we're going to try and connect to
vid=0x20ce
pid=0x0023

# Define the min and max attenuation values
minDbm=0
maxDbm=90

# Namespace class used to create function arguments similar to argparse
class Namespace:
    def __init__(self, **kwargs):
        self.__dict__.update(kwargs)

class mini_RCDAT_USB:

    def __init__(self, args):

      # Make sure attenuation is within the defined limits
      try:
         if(float(args.atten) > maxDbm) :
            print("Attenuation parameter is too high, max attenuation: "+str(maxDbm))
            print(descText)
            sys.exit(1)
         if(float(args.atten) < minDbm) :
            print("Attenuation parameter is too low, min attenuation: "+str(minDbm))
            print(descText)
            sys.exit(1)
      except ValueError as err:
         print("Error with attenuation value")
         print(descText)
         sys.exit(1)

      # Find our device
      dev = usb.core.find(idVendor=vid, idProduct=pid)

      if dev is None:
         raise ValueError('Device not found')

      for configuration in dev:
         for interface in configuration:
            ifnum = interface.bInterfaceNumber
            if not dev.is_kernel_driver_active(ifnum):
               continue
            try:
               dev.detach_kernel_driver(ifnum)
            except usb.core.USBError as e:
               pass

      # set the active configuration. with no args we use first config.
      dev.set_configuration()

      dev.reset()

      # Get the SN
      SerialN=""
      dev.write(1,"*:SN?") 
      sn=dev.read(0x81,64) 
      i=1
      while (sn[i]<255 and sn[i]>0):  
         SerialN=SerialN+chr(sn[i])
         i=i+1

      # Get the model number
      ModelN=""
      dev.write(1,"*:MN?") 
      mn=dev.read(0x81,64) 
      i=1
      while (mn[i]<255 and mn[i]>0):  
         ModelN=ModelN+chr(mn[i])
         i=i+1 

      # Get the firmware version
      Fw=""
      dev.write(1,"*:FIRMWARE?") 
      sn=dev.read(0x81,64) 
      i=1
      while (sn[i]<255 and sn[i]>0):  
         Fw=Fw+chr(sn[i])
         i=i+1

      retval=0

      # Set the attenuation
      attenCmdString="*:SETATT="+str(args.atten)+";"
      print(attenCmdString)
      dev.write(1,attenCmdString) 
      resp=dev.read(0x81,64)
      i=1
      AttResp=""
      while (resp[i]<255 and resp[i]>0):  
         AttResp=AttResp+chr(resp[i])
         i=i+1 
      if (AttResp != "1"):
         print("Error setting attenuation")
         retval=1

      # Check the attenuation
      getAttenString="*:ATT?"
      print(getAttenString)
      dev.write(1,getAttenString) # return attenuation value
      resp=dev.read(0x81,64)
      i=1
      AttResp=""
      while (resp[i]<255 and resp[i]>0):  
         AttResp=AttResp+chr(resp[i])
         i=i+1 
      print("Attenuation: " + AttResp)

      usb.util.release_interface(dev, 0)
      # dev.close()

if __name__ == '__main__':

   # Setup the command line description text
   descText = """
   Mini-Circuits RCDAT USB configuration tool.

   This tool is used to set the attenuation of a Mini Circuits RCDAT, using the USB interface.
   Attenuation value must be a decimal number between """+str(minDbm)+""" and """+str(maxDbm)+"""

   VID=0x"""+'%04X'%vid+"""
   PID=0x"""+'%04X'%pid

   # Parse the command line arguments
   parser = argparse.ArgumentParser(description=descText, formatter_class=RawTextHelpFormatter)
   parser.add_argument('atten',help='attenuation in dBm, 0.25 dB resolution')

   args = parser.parse_args()

   print("Mini-Circuits RCDAT USB configuration tool")
   print("Attenuation: "+str(args.atten))

   # Set the attenuation
   atten = mini_RCDAT_USB(args)

   sys.exit(0)
