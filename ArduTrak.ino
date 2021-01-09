#include "SBUS.h"
#include <Servo.h>
SBUS XM(Serial1);
Servo Rservo;
Servo Lservo;

int Tval; //Thrust variable 
int Sval; //Steering variable
int S1,S2; // Switches for Mode and Light
int DMode; // Drive Mode variable
int Rweel,Rraw;
int Lweel,Lraw;
int TrimmL,TrimmR;
int DZ;
int MDR,MDL;

const int Dpwm = 1000;
const int Mpwm = 1500;
const int Upwm = 2000; 

bool CalibS;

// channel, fail safe, and lost frames data
uint16_t channels[16];
bool failSafe;
bool lostFrame;
bool Debug;
int t=0;


void setup() {
  // Begin the SBUS communication
  XM.begin();
  // Serial for Debugging
  Serial.begin(9600);
  
  //Drive Mode
  DMode = 1;
  
  // Motor configuration right
  Rservo.attach(5); // Hardware pin for the right motor
  TrimmR = 0;       // Trimm
  MDR = -1;         // direction of the right motor
  Rweel = Mpwm + TrimmR; //Start setting
  
  // Motor configuration left
  Lservo.attach(6); // Hardware pin for left motor
  TrimmL = 0;       // Trimm
  MDL = -1;         // direction of the left motor
  Lweel = Mpwm + TrimmL; //Start setting
  
  // Deadzone 
  DZ = 10 ;
  //Debugging Mode thrue -> Channel's to Serial Port
  Debug = true; 
  // Calibration Mode
  CalibS = false;
}

void loop() {
  // SBUS signal processing
  if(!CalibS & (XM.read(&channels[0], &failSafe, &lostFrame))){
    Tval = map(channels[2], 172, 1811, Dpwm, Upwm);    
    Sval = map(channels[1], 172, 1811, Dpwm, Upwm);    
    S1 = map(channels[4], 172, 1811, Dpwm, Upwm); 
    S2 = map(channels[5], 172, 1811, Dpwm, Upwm);
    
    // Drive Mode 1 Normal
    if(S1 < 1500){
      Rraw = Mpwm-(((Tval-(Sval-Mpwm)-Mpwm)));
      Lraw = Mpwm+(((Tval+(Sval-Mpwm)-Mpwm)));
      }
      
    // Drive Mode 2 Crawling 
    if(S1 > 1500){
      if (Sval >= (Mpwm+DZ)){ 
        Lraw = Mpwm - (Mpwm-Tval);
        Rraw = Mpwm;
      }
      if (Sval <= (Mpwm-DZ)){ 
        Rraw = Mpwm - (Tval-Mpwm) ;
        Lraw = Mpwm;
      }
      
      if ((Sval <(Mpwm+DZ)) & (Sval > (Mpwm-DZ))){
        Rraw = Mpwm;
        Lraw = Mpwm;
        } 
      }


    // Add Deadzone 
    if((Rraw < Mpwm+ DZ) & (Rraw > Mpwm- DZ)) Rraw = Mpwm;
    if((Lraw < Mpwm+ DZ) & (Lraw > Mpwm- DZ)) Lraw = Mpwm; 
    // Add Trimm and and Motor direction
    Rweel = Mpwm + ((Mpwm - (Rraw + TrimmR))*MDR);
    Lweel = Mpwm + ((Mpwm - (Lraw + TrimmL))*MDL);      
    Rservo.writeMicroseconds(Rweel);
    Lservo.writeMicroseconds(Lweel);



    
    }
  // Debuggingoutput via serial port
  if(Debug & (t>1000)){    
    Serial.print("Min:1000, ");
    Serial.print(Tval);  
    Serial.print(",");
    //Serial.print("Sval:");
    Serial.print(Sval);
    Serial.print(",");
    //Serial.print("Rweel:");
    Serial.print(Rweel);  
    Serial.print(",");
    //Serial.print("Lweel:");
    Serial.print(Lweel);
    Serial.print(",");
    Serial.println("Max:2000");
    if(t>1000) t = 0;
    
  }
  t++;
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
