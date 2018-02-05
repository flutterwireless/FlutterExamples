/*
This example code for Flutter is
Copyright 2018, Taylor Alexander and Flutter Wireless, Inc.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "Flutter.h"

boolean _running = false;
Flutter flutter;

byte target_address = 2;

#define LED_ON 1
#define LED_OFF 0

#define ON_TIME 500
#define OFF_TIME 500

// Instructions:
// Set MAX_ADDRESS to the number of devices you have,
// then program each of your boards while changing 
// ADDRESS from 1, to 2, 3, etc.
// Power on all devices, and you should see device 1
// Blink repeatedly while the other devices blink one
// at a time in succession.

// Set this to the total number of devices you have.
#define MAX_ADDRESS 5

// Change this value from 1-N as you program boards.
#define ADDRESS 1

//A made up number. Serves as an additional filter on packets. Not necessary.
#define PACKET_CODE 0x20


void setup()
{
  flutter.band = BAND;
  flutter.setNetworkName("MultiAddress");

  if (flutter.init() == true)
  {
    flutter.ledLightShow();
    delay(500);
  }
  else
  {
    flutter.setLED(RED);
    while (true);
  }


  flutter.setAddress(ADDRESS);

  flutter.connect(1); //The number here does not matter at this time.
}



void loop()
{

    if(flutter.getState()!=NORMAL_OPERATION) //if we aren't synchronized with another radio, just loop and blink lights.
    {
      if(millis()%400<200)
      {
        flutter.setLED(RED);
      }
      
      else
      {
        flutter.setLED(BLUE);
      }
      
    }

  if(ADDRESS==1)
  {
  
    
    // Turn off old unit.
    byte data_array[2] = {PACKET_CODE, LED_OFF};
    flutter.sendData(data_array, 2, target_address);
    flutter.sendData(data_array, 2, target_address);
    flutter.setLED(0,0,0);
    delay(OFF_TIME);
  
    
    target_address++;
    if(target_address > MAX_ADDRESS)
    {
      target_address = 2; 
    }
    data_array[1] = LED_ON;
    flutter.sendData(data_array, 2, target_address);
    flutter.sendData(data_array, 2, target_address);
    flutter.setLED(GREEN);
    delay(ON_TIME);
    
  } else
  {
  
    if (flutter.dataAvailable() > 0)
    {
      int packetSize = flutter.nextPacketLength();
      byte data_array[packetSize];
      flutter.readBytes(data_array, packetSize);
  
      // PACKET_CODE is an optional field to help filter unwanted packets.
      if (data_array[5] == PACKET_CODE)
      {
  
        // Read our data byte.
        if(data_array[6] == LED_ON)
        {
          flutter.setLED(GREEN);
        } else
        {
          flutter.setLED(0,0,0);
        }
  
      }
  
      flutter.nextPacket();
    }
    delay(1);
  
  }

}


void button1()
{
  interrupts();
  int val = digitalRead(BUTTON1); //top button

  if (val == HIGH)
  {
    // _button1=255;
  }
  else
  {
    //  _button1=0;
  }

// buttonsChanged=true;
}

void button2()
{
  interrupts();
  int val = digitalRead(BUTTON2);
#ifdef FLUTTER_R2

  if (val == HIGH)
#else
  if (val == LOW)
#endif
  {
    //_button2=255;
  }
  else
  {
    //_button2=0;
  }

// buttonsChanged=true;
}

void systemReset()
{
  flutter.setLED(0, 0, 255);
  delayMicroseconds(16000);
  delayMicroseconds(16000);
  flutter.setLED(0, 0, 0);
  delayMicroseconds(16000);
  delayMicroseconds(16000);
  flutter.setLED(0, 255, 0);
  delayMicroseconds(16000);
  delayMicroseconds(16000);
  flutter.setLED(0, 0, 0);
  delayMicroseconds(16000);
  delayMicroseconds(16000);
  flutter.setLED(255, 0, 0);
  delayMicroseconds(16000);
  delayMicroseconds(16000);
  flutter.setLED(0, 0, 0);
  initiateReset(1);
  tickReset();
}



void radioInterrupt()
{
  flutter.interrupt();
}
void softInt()
{
  flutter.processSoftInt();
}

extern boolean tickInterrupt()
{
  if (!flutter.initialized)
  {
    return true;
  }

  return flutter.tickInt();
}



