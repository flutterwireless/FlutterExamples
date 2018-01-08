
#include "Arduino.h"

#include <Flutter_NeoPixel.h>
#include "Flutter.h"
#include "Reset.h"
#include <SPI.h>

#define PIN 7
#define LED_DIM 20
#define LED_BRIGHT 100
#define LED_OFF 0
#define LED_DELAY 1000
#define NUM_LEDS 144

float bright = 0.0;

boolean auto_ = true;
#define AUTO_MODE auto_


int mode = 0;
int maxmode=2;

Flutter flutter;

long timeOn = 0;
byte ledBright = 0;

int lastPacket = 0;

int offset = 0;



Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

//int ledValue = 0;

void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);

 strip.begin();
  strip.show();

  if(AUTO_MODE==false)
  {
    flutter.setNetworkName("Game network");
  flutter.band = NORTH_AMERICA;
  if(flutter.init()==true)
  {
      Serial.println("Init success.");
    //  flutter.ledLightShow();
      delay(500);
      //analogWrite(LED_R, 128);
  } else
  {
    flutter.setLED(RED);
    Serial.println("Init failed.");
    while(true);
  }

  flutter.setAddress(2);
  flutter.connect(1); //form a network with this and one other device

  }
  
  pinMode(6, OUTPUT);

}




 void loop() {
  // put your main code here, to run repeatedly:

    offset+=1;
    
    if(offset>NUM_LEDS) offset=0;
    if(offset<0) offset = NUM_LEDS;

       int r,g,b = 0;
      int i, j, k = 0;
      int numColors = 5;
      int colors[numColors];
      int pixel = 0;

      for(i = 0; i<NUM_LEDS; i++)
      {
        
            j = i+NUM_LEDS/3;
            if(j>NUM_LEDS) j-=NUM_LEDS;
    
            k = i+2*NUM_LEDS/3;
            if(k>NUM_LEDS) k-=NUM_LEDS;
            
            if(i<NUM_LEDS/2)
            {
            r = (byte)(255*sin( ((i)*PI)/ (NUM_LEDS/2) ) );
            } else
            {
              r=0;
            }
            if(j<NUM_LEDS/2)
            {
               g = (byte)(255*sin( ((j)*PI)/ (NUM_LEDS/2) ) );
            } else
            {
              g=0;
            }
               if(k<NUM_LEDS/2)
            {
             b = (byte)(255*sin( ((k)*PI)/ (NUM_LEDS/2) ) );
            } else
            {
              b=0;
            }

    
            pixel = i+offset;
            if(pixel>=NUM_LEDS) pixel-=NUM_LEDS;
            
          

            // Ramp brightness up over 3 seconds
            if(millis()<3000)
            {
             bright=millis()/6000.0f; 
            }else
            {
              bright=0.5f;
            }
        

        strip.setPixelColor(pixel, bright*r, bright*g, bright*b);
       
      }

      strip.show();
      delay(20);
    }
    
  


void button1()
{
  interrupts();
  int val = digitalRead(BUTTON1); //top button

  if(val==HIGH)
  {
  // systemReset();
  }else
  {
  //  _button1=0;
  }

 // buttonsChanged=true;
    
}

void systemReset()
{
  
      flutter.setLED(0,0,255);
      delayMicroseconds(16000);
      delayMicroseconds(16000);
      flutter.setLED(0,0,0);
      delayMicroseconds(16000);
      delayMicroseconds(16000);
      flutter.setLED(0,255,0);
      delayMicroseconds(16000);
      delayMicroseconds(16000);
      flutter.setLED(0,0,0);
      delayMicroseconds(16000);
      delayMicroseconds(16000);
      flutter.setLED(255,0,0);
      delayMicroseconds(16000);
      delayMicroseconds(16000);
      flutter.setLED(0,0,0);

   //  initiateReset(1);
   //  tickReset();
      
}

void button2()
{
  interrupts();
  int val = digitalRead(BUTTON2);

  #ifdef FLUTTER_R2
  if(val==HIGH)
  #else
  if(val==LOW)
  #endif
  {

  }else
  {
//  systemReset();
  }

 // buttonsChanged=true;
  
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
  if(!flutter.initialized) 
  return true;
  return flutter.tickInt();
}



