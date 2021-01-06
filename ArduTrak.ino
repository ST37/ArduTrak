#include "SBUS.h"
#include <Servo.h>
#include <EasyNeoPixels.h>

// a SBUS object, which is on hardware
// serial port 1#include <EasyNeoPixels.h>
SBUS XM(Serial1);

Servo Rservo;
Servo Lservo;
int Tval;
int Sval;
int DMode;
int Rweel,TrimmR,Rraw;
int Lweel,TrimmL,Lraw;
int DZ;
bool LEDS;

// channel, fail safe, and lost frames data
uint16_t channels[16];
bool failSafe;
bool lostFrame;

const int ledPin =  9;

void setup() {
  // begin the SBUS communication
  XM.begin();
  Serial.begin(115200);
  Rservo.attach(5);
  Lservo.attach(6);
  setupEasyNeoPixels(15, 1);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  DMode = 1;
  TrimmR = -11;
  TrimmL = -1;
  Rweel = 90 + TrimmR;
  Lweel = 90 + TrimmL;
  DZ = 2 ;
  LEDS = false;
}

void loop() {

  // look for a good SBUS packet from the receiver
  if(XM.read(&channels[0], &failSafe, &lostFrame)){
    if(channels[4] < 1000) DMode = 1;
    if(channels[4] > 1000) DMode = 2;
    Tval = map(channels[2], 172, 1811, 0, 180);     // scale it to use it with the servo (value between 0 and 180)
    Sval = map(channels[1], 172, 1811, 0, 180);     // scale it to use it with the servo (value between 0 and 180)
    
    if(DMode == 1){
      Rraw = ((180-Tval)-(90-Sval));
      Lraw = (Tval-(90-Sval));
      }
      
    if(DMode == 2){
      if (Sval >= (90+DZ)) Rraw =90 + (90-Sval);
      if (Sval <= (90-DZ)) Lraw =90 - (Sval-90) ;
      if (Sval <(90+DZ) & Sval > (90-DZ)){
        Rraw = 90;
        Lraw = 90;
      }

      
     
      }

      Rweel = Rraw + TrimmR;
      Lweel = Lraw + TrimmL;
      
    Rservo.write(Rweel);
    Lservo.write(Lweel);
    
    }
    Serial.print("Mode = ");
    Serial.print(DMode);  
    
    Serial.print(" Tval = ");
    Serial.print(Tval);  
    Serial.print("  Sval = ");
    Serial.print(Sval);

    Serial.print(" Rweel = ");
    Serial.print(Rweel);  
    Serial.print("  Lweel = ");
    Serial.println(Lweel);
   if(LEDS)  writeEasyNeoPixel(0, HIGH);
   
  
}
int expo(float pos, float amount)
{
 pos = pos / 100;
 amount = amount / 100;
 if (amount < 0) {
   float temp = pow((float)pos,(float)(1.0/3.0));
   return (((1-amount)*pos)+((amount*temp)))*100;
 } else {
   return (((1-amount)*pos)+((amount*pow(pos,3))))*100;
 }
}
