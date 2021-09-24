// Code for MOSFET driven 3 channel aquarium light (or basically anythin else) based on RTC clock
// by Michal Petrulak 

#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 DS1307;
DateTime CurrentTime;

// define pins for output
const int Pwm1Pin = 9;  //pwm pin for channel 1 
const int Pwm2Pin = 5; //pwm pin for channel 2
const int Pwm3Pin = 11; //pwm pin for channel 3

int delaytime = 500;

// define time variables
int hours;
int minutes;
int seconds;
long hoursec;
long minutesec;
long secsec;
long totalsec;


// ----------------------- PWM settings -----------------------
// define PWM CHannels min and max intensities (0-255)
// PwmMax value can NOT be 0
// set the PWM values as desired to achieve sufficient light intensity

int Pwm1maxInt = 120; // 100
int Pwm1minInt = 2; // 2

int Pwm2maxInt = 255;
int Pwm2minInt = 0; // 0

int Pwm3maxInt = 160;  // 160
int Pwm3minInt = 2;  //2


// define PWM Channels fading times, time is in seconds so just change the hour in 0-24 format
// Set this times as desired

float Pwm1MorningStart = 14*60*60L; // time when the "sunrise starts" for channel 1 
float Pwm1MorningEnd = 14.5*60*60L; // time when the "sunrise ends" for channel 1 
float Pwm1EveningStart = 22*60*60L; // time when the "sunset starts" for channel 1 
float Pwm1EveningEnd = 22.5*60*60L; // time when the "sunset ends" for channel 1 

float Pwm2MorningStart = 7*60*60L; // time when the "sunrise starts" for channel 2 
float Pwm2MorningEnd = 8*60*60L; // time when the "sunrise ends" for channel 2 
float Pwm2EveningStart = 14*60*60L; // time when the "sunset starts" for channel 2 
float Pwm2EveningEnd = 15*60*60L; // time when the "sunset ends" for channel 2 

float Pwm3MorningStart = 11.5*60*60L; // time when the "sunrise starts" for channel 3 
float Pwm3MorningEnd = 12.5*60*60L; // time when the "sunrise ends" for channel 3 
float Pwm3EveningStart = 21*60*60L; // time when the "sunset starts" for channel 3 
float Pwm3EveningEnd = 22*60*60L; // time when the "sunset ends" for channel 3 

//PWM calculations CHannel1
float IncrementUpPwm1 = (Pwm1maxInt - Pwm1minInt)/(Pwm1MorningEnd - Pwm1MorningStart);
float IncrementDownPwm1 = (Pwm1maxInt - Pwm1minInt)/(Pwm1EveningEnd - Pwm1EveningStart);
float pwm1SecFromMorningStart;
float pwm1SecFromEveningStart;
int Pwm1Val = 10; // some value to inicialize, must be > Pwm1minInt

//PWM calculations CHannel2
float IncrementUpPwm2 = (Pwm2maxInt - Pwm2minInt)/(Pwm2MorningEnd - Pwm2MorningStart);
float IncrementDownPwm2 = (Pwm2maxInt - Pwm2minInt)/(Pwm2EveningEnd - Pwm2EveningStart);
float pwm2SecFromMorningStart;
float pwm2SecFromEveningStart;
int Pwm2Val = 10; // some value to inicialize, must be > Pwm2minInt

//PWM calculations CHannel3
float IncrementUpPwm3 = (Pwm3maxInt - Pwm3minInt)/(Pwm3MorningEnd - Pwm3MorningStart);
float IncrementDownPwm3 = (Pwm3maxInt - Pwm3minInt)/(Pwm3EveningEnd - Pwm3EveningStart);
float pwm3SecFromMorningStart;
float pwm3SecFromEveningStart;
int Pwm3Val = 10; // some value to inicialize, must be > Pwm3minInt

// ----------------------- Setup -----------------------
void setup() {
 // serial interfce setup
Serial.begin(9600);

// pin setup
pinMode(Pwm1Pin, OUTPUT);
pinMode(Pwm2Pin, OUTPUT); 
pinMode(Pwm3Pin, OUTPUT); 

// RTC Clock setup 
Wire.begin();
DS1307.begin();
//DS1307.adjust(DateTime(__DATE__, __TIME__));  //Set RTC time to sketch compilation time, only use for 1 (ONE) run.
                                                //otherwise it will reset time at each device reset!
}


// ----------------------- Loop -----------------------
void loop() {

CalcSec(); // Get current time and calculate totalseconds

SetPwm1Val(); //calculates the PWM channel 1 value
SetPwm2Val(); //calculates the PWM channel 2 value
SetPwm3Val(); //calculates the PWM channel 3 value

MoonLightPwm1(); //commend if not desired
MoonLightPwm2(); //commend if not desired
MoonLightPwm3(); //commend if not desired

analogWrite(Pwm1Pin, Pwm1Val); //commend to "turn off" channel
analogWrite(Pwm2Pin, Pwm2Val); //commend to "turn off" channel
analogWrite(Pwm3Pin, Pwm3Val); //commend to "turn off" channel

// PrintSerial();// prints values for debugging into serial port

}

//---------------------function definitions---------------------
void CalcSec(){
// this function calculates the time in seconds elapsed from midnight
// and also seconds from the morning and evening start

DateTime datetime = DS1307.now();
hours=(datetime.hour());
minutes=(datetime.minute());
seconds=(datetime.second());
hoursec=hours*60*60L;
minutesec=minutes*60L;
secsec=seconds;
totalsec = (hoursec+minutesec+secsec);

pwm1SecFromMorningStart = totalsec - Pwm1MorningStart;
pwm1SecFromEveningStart = totalsec - Pwm1EveningStart;

pwm2SecFromMorningStart = totalsec - Pwm2MorningStart;
pwm2SecFromEveningStart = totalsec - Pwm2EveningStart;

pwm3SecFromMorningStart = totalsec - Pwm3MorningStart;
pwm3SecFromEveningStart = totalsec - Pwm3EveningStart;
}


void PrintSerial() { // prints values for debugging into serial port
Serial.print ("hours: ");
Serial.println (hours);
Serial.print ("minuts: ");
Serial.println (minutes);
Serial.print ("seconds: ");
Serial.println (seconds);
Serial.print ("totalseconds: ");
Serial.println (totalsec);
Serial.print ("Channel 1 value: ");
Serial.println (Pwm1Val);
Serial.print ("Channel 2 value: ");
Serial.println (Pwm2Val);
Serial.print ("Channel 3 value: ");
Serial.println (Pwm3Val);

}



// Channel 1 pwm value decision function
void SetPwm1Val(){
  
if (totalsec <= Pwm1MorningStart) {
        Pwm1Val = Pwm1minInt;
        }
    
else if ((Pwm1MorningStart<totalsec)&&(totalsec<Pwm1MorningEnd)){
        Pwm1Val = (Pwm1minInt + (pwm1SecFromMorningStart*IncrementUpPwm1));
        }
    
else if ((Pwm1MorningEnd <= totalsec)&&(totalsec < Pwm1EveningStart)){
        Pwm1Val = Pwm1maxInt;
        }
    
else if ((Pwm1EveningStart <= totalsec)&&(totalsec <= Pwm1EveningEnd)){
        Pwm1Val = (Pwm1maxInt - (pwm1SecFromEveningStart*IncrementDownPwm1));
        }

else {
        Pwm1Val = Pwm1minInt;
        }

delay(delaytime);
  
  }

  
// Channel 2 pwm value decision function
void SetPwm2Val(){
  
if (totalsec <= Pwm2MorningStart) {
        Pwm2Val = Pwm2minInt;
        }
    
else if ((Pwm2MorningStart<totalsec)&&(totalsec<Pwm2MorningEnd)){
        Pwm2Val = (Pwm2minInt + (pwm2SecFromMorningStart*IncrementUpPwm2));
        }
    
else if ((Pwm2MorningEnd <= totalsec)&&(totalsec < Pwm2EveningStart)){
        Pwm2Val = Pwm2maxInt;
        }
    
else if ((Pwm2EveningStart <= totalsec)&&(totalsec <= Pwm2EveningEnd)){
        Pwm2Val = (Pwm2maxInt - (pwm2SecFromEveningStart*IncrementDownPwm2));
        }

else {
        Pwm2Val = Pwm2minInt;
        }

delay(delaytime);
  
  }


// Channel 3 pwm value decision function
void SetPwm3Val(){
  
if (totalsec <= Pwm3MorningStart) {
        Pwm3Val = Pwm3minInt;
        }
    
else if ((Pwm3MorningStart<totalsec)&&(totalsec<Pwm3MorningEnd)){
        Pwm3Val = (Pwm3minInt + (pwm3SecFromMorningStart*IncrementUpPwm3));
        }
    
else if ((Pwm3MorningEnd <= totalsec)&&(totalsec < Pwm3EveningStart)){
        Pwm3Val = Pwm3maxInt;
        }
    
else if ((Pwm3EveningStart <= totalsec)&&(totalsec <= Pwm3EveningEnd)){
        Pwm3Val = (Pwm3maxInt - (pwm3SecFromEveningStart*IncrementDownPwm3));
        }

else {
        Pwm3Val = Pwm3minInt;
        }

delay(delaytime);
  
  }

void MoonLightPwm1(){ //This function is due to PWM value 1 which is for some reason
                      //behaving like "high" and therfore the mosfet fully opens 
                      //(same as if it would be pwm 255)

  if (Pwm1Val < 2){
        Pwm1Val = 0;
        analogWrite(Pwm1Pin, Pwm1Val);
        }
  
  }

void MoonLightPwm2(){

  if (Pwm2Val < 2){
        Pwm2Val = 0;
        analogWrite(Pwm2Pin, Pwm2Val);
        }
  
  }

void MoonLightPwm3(){

  if (Pwm3Val < 2){
        Pwm3Val = 0;
        analogWrite(Pwm3Pin, Pwm3Val);
        }
  
  }
