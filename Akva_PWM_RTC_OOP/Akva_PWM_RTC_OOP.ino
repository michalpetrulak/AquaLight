
#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 DS1307;
DateTime CurrentTime;

// define pins for output
const int Pwm1Pin = 9;  //pwm pin for channel 1 
const int Pwm2Pin = 10; //pwm pin for channel 2
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

////////////////////////////PWM////////////////////////////
// define PWM CHannels min and max intensities (0-255)
// set this PWM values as desired

int Pwm1maxInt = 160;
int Pwm1minInt = 2;

int Pwm2maxInt = 160;
int Pwm2minInt = 2;

int Pwm3maxInt = 160;
int Pwm3minInt = 2;


// define PWM Channels fading times, time is in seconds so just change the hour in 0-24 format
// Set this times as desired

float Pwm1MorningStart = 11.5*60*60L; // time when the "sunrise starts" for channel 1 
float Pwm1MorningEnd = 12.5*60*60L; // time when the "sunrise ends" for channel 1 
float Pwm1EveningStart = 21.0*60*60L; // time when the "sunset starts" for channel 1 
float Pwm1EveningEnd = 22*60*60L; // time when the "sunset ends" for channel 1 

float Pwm2MorningStart = 11.5*60*60L; // time when the "sunrise starts" for channel 2 
float Pwm2MorningEnd = 12.5*60*60L; // time when the "sunrise ends" for channel 2 
float Pwm2EveningStart = 21.0*60*60L; // time when the "sunset starts" for channel 2 
float Pwm2EveningEnd = 22*60*60L; // time when the "sunset ends" for channel 2 

float Pwm3MorningStart = 11.5*60*60L; // time when the "sunrise starts" for channel 3 
float Pwm3MorningEnd = 12.5*60*60L; // time when the "sunrise ends" for channel 3 
float Pwm3EveningStart = 21.0*60*60L; // time when the "sunset starts" for channel 3 
float Pwm3EveningEnd = 22*60*60L; // time when the "sunset ends" for channel 3 

//PWM calculations CHannel1
float IncrementUpPwm1 = (Pwm1EveningEnd - Pwm1EveningStart)/(Pwm1maxInt - Pwm1minInt);
float IncrementDownPwm1 = (Pwm1MorningEnd - Pwm1MorningStart)/(Pwm1maxInt - Pwm1minInt);
float pwm1SecFromMorningStart;
float pwm1SecFromEveningStart;
int Pwm1Val = 5; // some value to inicialize, must be > Pwm1minInt

//PWM calculations CHannel2
float IncrementUpPwm2 = (Pwm2EveningEnd - Pwm2EveningStart)/(Pwm2maxInt - Pwm2minInt);
float IncrementDownPwm2 = (Pwm2MorningEnd - Pwm2MorningStart)/(Pwm2maxInt - Pwm2minInt);
float pwm2SecFromMorningStart;
float pwm2SecFromEveningStart;
int Pwm2Val = 5; // some value to inicialize, must be > Pwm2minInt

//PWM calculations CHannel3
float IncrementUpPwm3 = (Pwm3EveningEnd - Pwm3EveningStart)/(Pwm3maxInt - Pwm3minInt);
float IncrementDownPwm3 = (Pwm3MorningEnd - Pwm3MorningStart)/(Pwm3maxInt - Pwm3minInt);
float pwm3SecFromMorningStart;
float pwm3SecFromEveningStart;
int Pwm3Val = 5; // some value to inicialize, must be > Pwm3minInt

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
//DS1307.adjust(DateTime(__DATE__, __TIME__));  // Set RTC time to sketch compilation time, only use for 1 (ONE) run. Will reset time at each device reset!

}


// ----------------------- Loop -----------------------
void loop() {

    
// Get current time and calculate totalseconds
void CalcSec();

void SetPwm1Val();

void SetPwm2Val();

void SetPwm3Val();

analogWrite(Pwm1Pin, Pwm1Val);

analogWrite(Pwm2Pin, Pwm2Val);

analogWrite(Pwm3Pin, Pwm3Val);

void MoonLightPwm1();

void MoonLightPwm2();

void MoonLightPwm3();

// prints values for debugging into serial port
void PrintSerial();



//---------------------PWM Loop---------------------


}


void CalcSec(){
// this function calculates the time in seconds elapsed from midnight 
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
Serial.println (hours);
Serial.println (minutes);
Serial.println (seconds);
Serial.println (totalsec);
Serial.println (Pwm1Val);
Serial.println (Pwm2Val);
Serial.println (Pwm3Val);

}



// Channel 1 decision function
void SetPwm1Val(){
  
if (totalsec <= Pwm1MorningStart) {
        Pwm1Val = Pwm1minInt;
        }
    
else if ((Pwm1MorningStart<totalsec)&&(totalsec<Pwm1MorningEnd)){
        Pwm1Val = (Pwm1minInt + (pwm1SecFromMorningStart/IncrementUpPwm1));
        }
    
else if ((Pwm1MorningEnd <= totalsec)&&(totalsec < Pwm1EveningStart)){
        Pwm1Val = Pwm1maxInt;
        }
    
else if ((Pwm1EveningStart <= totalsec)&&(totalsec <= Pwm1EveningEnd)){
        Pwm1Val = (Pwm1maxInt - (pwm1SecFromEveningStart/IncrementDownPwm1));
        }

else {
        Pwm1Val = Pwm1minInt;
        }

delay(delaytime);
  
  }

  
// Channel 2 decision function
void SetPwm2Val(){
  
if (totalsec <= Pwm2MorningStart) {
        Pwm2Val = Pwm2minInt;
        }
    
else if ((Pwm2MorningStart<totalsec)&&(totalsec<Pwm2MorningEnd)){
        Pwm2Val = (Pwm2minInt + (pwm2SecFromMorningStart/IncrementUpPwm2));
        }
    
else if ((Pwm2MorningEnd <= totalsec)&&(totalsec < Pwm2EveningStart)){
        Pwm2Val = Pwm2maxInt;
        }
    
else if ((Pwm2EveningStart <= totalsec)&&(totalsec <= Pwm2EveningEnd)){
        Pwm2Val = (Pwm2maxInt - (pwm2SecFromEveningStart/IncrementDownPwm2));
        }

else {
        Pwm2Val = Pwm2minInt;
        }

delay(delaytime);
  
  }


// Channel 3 decision function
void SetPwm3Val(){
  
if (totalsec <= Pwm3MorningStart) {
        Pwm3Val = Pwm3minInt;
        }
    
else if ((Pwm3MorningStart<totalsec)&&(totalsec<Pwm3MorningEnd)){
        Pwm3Val = (Pwm3minInt + (pwm3SecFromMorningStart/IncrementUpPwm3));
        }
    
else if ((Pwm3MorningEnd <= totalsec)&&(totalsec < Pwm3EveningStart)){
        Pwm3Val = Pwm3maxInt;
        }
    
else if ((Pwm3EveningStart <= totalsec)&&(totalsec <= Pwm3EveningEnd)){
        Pwm3Val = (Pwm3maxInt - (pwm3SecFromEveningStart/IncrementDownPwm3));
        }

else {
        Pwm3Val = Pwm3minInt;
        }

delay(delaytime);
  
  }

void MoonLightPwm1(){

  if (Pwm1Val < 2){
        Pwm1Val = 0;
        analogWrite(Pwm1Pin, Pwm1Val);
        }
  
  }

void MoonLightPwm2(){

  if (Pwm2Val < 2){
        Pwm1Val = 0;
        analogWrite(Pwm2Pin, Pwm2Val);
        }
  
  }

void MoonLightPwm3(){

  if (Pwm3Val < 2){
        Pwm3Val = 0;
        analogWrite(Pwm3Pin, Pwm3Val);
        }
  
  }
