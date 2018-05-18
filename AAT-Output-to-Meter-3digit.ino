/******************************************************************************************
Basic routine to read an AAT003 angle sensor with an Arduino Uno, calculate the angle, 
and provide an analog output proportional to the angle. 
Scaled for a 3-digit panel meter (36 mV full scale), with no other components required.
Uses 10-bit PWM resolution to allow 36 mV full scale.
Sensor SIN output on IO4; COS on IO5; analog angle output on IO9.
*******************************************************************************************/
#include <TimerOne.h> //Library routine for 10-bit PWM resolution 
void setup() {Timer1.initialize(1000);} //Set pwm timer for 1 ms period

//Calculate angle; output 0.1 mV/deg (512 assumes no offset):
void loop() {Timer1.pwm(9,(atan2(float(analogRead(5)-512),float(analogRead(4)-512))/3.14159+1)*509);}
