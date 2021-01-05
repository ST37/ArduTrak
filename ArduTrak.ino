#include "SBUS.h"
#include <Servo.h>

// a SBUS object, which is on hardware
// serial port 1
SBUS XM(Serial1);

Servo Rservo;
Servo Lservo;
int Rsval;
int Lsval;

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

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
}

void loop() {

  // look for a good SBUS packet from the receiver
  if(XM.read(&channels[0], &failSafe, &lostFrame)){
    Rsval = map(channels[2], 172, 1811, 0, 180);     // scale it to use it with the servo (value between 0 and 180)
    Rservo.write(Rsval);
    Serial.print("R = ");
    Serial.print(Rsval);

    Lsval = map(channels[1], 172, 1811, 0, 180);     // scale it to use it with the servo (value between 0 and 180)
    Lservo.write(Lsval);
    Serial.print("  L = ");
    Serial.println(Lsval);
  
  }
}
