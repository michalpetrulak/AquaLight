
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

int Pwm1maxInt = 160;
int Pwm1minInt = 2;

int Pwm2maxInt = 160;
int Pwm2minInt = 2;

int Pwm3maxInt = 160;
int Pwm3minInt = 2;


// define PWM Channels fading times, time is in seconds so just change the hour in 0-24 format

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

//PWM calculations CHannel1
float IncrementUpPwm1 = (Pwm1EveningEnd - Pwm1EveningStart)/(Pwm1maxInt - Pwm1minInt);
float IncrementDownPwm1 = (Pwm1MorningEnd - Pwm1MorningStart)/(Pwm1maxInt - Pwm1minInt);
float pwm1SecFromMorningStart;
float pwm1SecFromEveningStart;
int Pwm1Val;

//PWM calculations CHannel2
float IncrementUpPwm2 = (Pwm2EveningEnd - Pwm2EveningStart)/(Pwm2maxInt - Pwm2minInt);
float IncrementDownPwm2 = (Pwm2MorningEnd - Pwm2MorningStart)/(Pwm2maxInt - Pwm2minInt);
float pwm2SecFromMorningStart;
float pwm2SecFromEveningStart;
int Pwm2Val;

//PWM calculations CHannel3
float IncrementUpPwm3 = (Pwm3EveningEnd - Pwm3EveningStart)/(Pwm3maxInt - Pwm3minInt);
float IncrementDownPwm3 = (Pwm3MorningEnd - Pwm3MorningStart)/(Pwm3maxInt - Pwm3minInt);
float pwm3SecFromMorningStart;
float pwm3SecFromEveningStart;
int Pwm3Val;

}


// ----------------------- Loop -----------------------
void loop() {

    
// Get current time and calculate totalseconds
void CalcSec();

// prints values for debugging into serial port
void PrintSerial();


delay(delaytime);



//---------------------PWM Loop---------------------
if (ledstate >=3){
        ledstate2=ledstate;}
if (ledstate < 3){
        ledstate2 = 0;}


if (totalsec <= morningstart) {
        ledstate = dimendint;
        analogWrite(Pwm1Pin, ledstate);
        analogWrite(Pwm2Pin, ledstate);  // ledstate2
        }
    
else if ((morningstart<totalsec)&&(totalsec<morningend)){
        ledstate = (dimendint + (secfrommorningstart/incr));
        analogWrite(Pwm1Pin, ledstate);
        analogWrite(Pwm2Pin, ledstate);  // ledstate2
        }
    
else if ((morningend <= totalsec)&&(totalsec < dimstart)){
        ledstate = dimstartint;
        analogWrite(Pwm1Pin, ledstate);
        analogWrite(Pwm2Pin, ledstate);  // ledstate2
        }
    
else if ((dimstart <= totalsec)&&(totalsec <= dimend)){
        ledstate = (dimstartint - (secfromdimstart/incr));
        analogWrite(Pwm1Pin, ledstate);
        analogWrite(Pwm2Pin, ledstate);  // ledstate2
        }
    
else {
        ledstate = dimendint;
        Serial.println ("to ti nejak nevychadza");
        analogWrite(Pwm1Pin, ledstate);
        analogWrite(Pwm2Pin, ledstate);  // ledstate2
        }

delay(delaytime);




if (totalsec <= morningstartPwm3) {
        ledstatePwm3 = dimendintPwm3;
        analogWrite(Pwm3Pin, ledstatePwm3);
        }
    
else if ((morningstartPwm3<totalsec)&&(totalsec<morningendPwm3)){
        ledstatePwm3 = (dimendintPwm3 + (secfrommorningstartPwm3/incrPwm3));
        analogWrite(Pwm3Pin, ledstatePwm3);
        }
    
else if ((morningendPwm3 <= totalsec)&&(totalsec < dimstartPwm3)){
        ledstatePwm3 = dimstartintPwm3;
        analogWrite(Pwm3Pin, ledstatePwm3);
        }
    
else if ((dimstartPwm3 <= totalsec)&&(totalsec <= dimendPwm3)){
        ledstatePwm3 = (dimstartintPwm3 - (secfromdimstartPwm3/incrPwm3));
        analogWrite(Pwm3Pin, ledstatePwm3);
        }
    
else {
        ledstatePwm3 = dimendintPwm3;
        Serial.println ("to ti nejak nevychadza");
        analogWrite(Pwm3Pin, ledstatePwm3);
        }

delay(delaytime);

if (ledstatePwm3 < 3){
        ledstatePwm3 = 0;}
        
delay(delaytime);



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
