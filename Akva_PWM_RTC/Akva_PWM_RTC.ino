
#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 DS1307;
DateTime CurrentTime;

//change is change
// define pins for output
const int PwmPin = 10;  //not used 
const int Pwm2Pin = 11; // 
const int Pwm3Pin = 9;


// define time variables
int hours;
int minutes;
int seconds;
long hoursec;
long minutesec;
long secsec;
long totalsec;
int delaytime = 1000;

////////////////////////////PWM////////////////////////////
// define PWM fader intensities
int dimstartint = 160;
int dimendint = 2;

int dimstartintPwm3 = 100;
int dimendintPwm3 = 2;


// define PWM fader times
float dimstart = 21.0*60*60L;
float dimend = 22*60*60L;
float morningstart = 11.5*60*60L;
float morningend = 12.5*60*60L;


float dimstartPwm3 = 22.0*60*60L;
float dimendPwm3 = 22.5*60*60L;
float morningstartPwm3 = 14.0*60*60L;
float morningendPwm3 = 14.5*60*60L;


// define PWM primary calculations 
float incr = (dimend - dimstart)/(dimstartint - dimendint);
float secfromdimstart;
float secfrommorningstart;
int ledstate;
int ledstate2=ledstate;

float incrPwm3 = (dimendPwm3 - dimstartPwm3)/(dimstartintPwm3 - dimendintPwm3);
float secfromdimstartPwm3;
float secfrommorningstartPwm3;
int ledstatePwm3;



// ----------------------- Setup -----------------------
void setup() {
 // serial interfce setup
Serial.begin(9600);

// pin setup
pinMode(PwmPin, OUTPUT);
pinMode(Pwm2Pin, OUTPUT); 
pinMode(Pwm3Pin, OUTPUT); 

 
  // Clock
Wire.begin();
DS1307.begin();
//DS1307.adjust(DateTime(__DATE__, __TIME__));  // Set RTC time to sketch compilation time, only use for 1 (ONE) run. Will reset time at each device reset!
}


// ----------------------- Loop -----------------------
void loop() {

    
// Get current time and calculate totalseconds
DateTime datetime = DS1307.now();
hours=(datetime.hour());
minutes=(datetime.minute());
seconds=(datetime.second());
hoursec=hours*60*60L;
minutesec=minutes*60L;
secsec=seconds;
totalsec = (hoursec+minutesec+secsec);
 

secfromdimstart = totalsec - dimstart;
secfrommorningstart = totalsec - morningstart;

secfromdimstartPwm3 = totalsec - dimstartPwm3;
secfrommorningstartPwm3 = totalsec - morningstartPwm3;

delay(delaytime);



//---------------------PWM Loop---------------------
if (ledstate >=3){
        ledstate2=ledstate;}
if (ledstate < 3){
        ledstate2 = 0;}


if (totalsec <= morningstart) {
        ledstate = dimendint;
        analogWrite(PwmPin, ledstate);
        analogWrite(Pwm2Pin, ledstate);  // ledstate2
        }
    
else if ((morningstart<totalsec)&&(totalsec<morningend)){
        ledstate = (dimendint + (secfrommorningstart/incr));
        analogWrite(PwmPin, ledstate);
        analogWrite(Pwm2Pin, ledstate);  // ledstate2
        }
    
else if ((morningend <= totalsec)&&(totalsec < dimstart)){
        ledstate = dimstartint;
        analogWrite(PwmPin, ledstate);
        analogWrite(Pwm2Pin, ledstate);  // ledstate2
        }
    
else if ((dimstart <= totalsec)&&(totalsec <= dimend)){
        ledstate = (dimstartint - (secfromdimstart/incr));
        analogWrite(PwmPin, ledstate);
        analogWrite(Pwm2Pin, ledstate);  // ledstate2
        }
    
else {
        ledstate = dimendint;
        Serial.println ("to ti nejak nevychadza");
        analogWrite(PwmPin, ledstate);
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


//Serial.println (hours);
//Serial.println (minutes);
//Serial.println (minutesec);
//Serial.println (seconds);
//Serial.println ();
//Serial.println (dimstartint);
//Serial.println ();
//Serial.println (ledstate);
//Serial.println (hoursec);
//Serial.println (minutesec);
//Serial.println (secsec);
//Serial.println (hoursec+minutesec+secsec);
//Serial.println (totalsec);
//Serial.println (analogValScaled);

}
