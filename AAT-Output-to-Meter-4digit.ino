/****************************************************************************** 
High-precision four-digit analog output (+-180.0 mV) from an NVE AAT003 
angle sensor with an Arduino board (no other components required).
Calibration for sensitivity and offset. 
Sensor SIN output on pin 4; COS on pin 5; 
+analog output on pin 10; -out on 9.                              Rev. 6/15/16
******************************************************************************/
#include <PWM.h> //Library routine for 16-bit (high-resolution) PWM

//AAT signals
int AATcos;
int AATsin;

//Define sensor min and max outputs (values determined in calibration routine)
int AATcosmin=1023;
int AATcosmax=0;
int AATsinmin=1023;
int AATsinmax=0;

float angle; //Define indicated angle in radians

//Factors to scale meter to 180 mV full scale with a 5000 mV supply
int FSn = -2359;
int FSp = 2359;

float pi=3.14159;
int x; //Sample counter

void setup() {
  InitTimersSafe(); //Initiate PWM timer
  bool success = SetPinFrequencySafe(9, 1001); //Set PWM at 131 Hz (slow-ish, but maximizes resolution)
  success = SetPinFrequencySafe(10, 1001); //Set pin 10 PWM
  pinMode(13, OUTPUT); //Set up blue LED
  delay(2000); //Display neutral (zero) output for 2 sec. for verification
  pwmWriteHR(10,0); pwmWriteHR(9, -FSn); //Display fullscale negative (-180 deg.) for meter calibration
  delay(2000);
  pwmWriteHR(9,0); pwmWriteHR(10, FSp); //Display fullscale positive (+180 deg.) for meter calibration
  delay(2000);

//Calibrate by rotating several turns
  digitalWrite(13, HIGH); //Turn on blue LED to indicate calibration mode
  for(x=1; x<=32000; x++) //Sample for a few seconds
  {
    AATcos = analogRead(5); //Find sensor output minimums and maximums
    AATsin = analogRead(4);
    if (AATcos<AATcosmin) AATcosmin=AATcos;
    if (AATcos>AATcosmax) AATcosmax=AATcos;
    if (AATsin<AATsinmin) AATsinmin=AATsin;
    if (AATsin>AATsinmax) AATsinmax=AATsin;
  }
  digitalWrite(13, LOW); //Turn off blue LED to indicate calibration done
}

void loop() {
AATsin = 0;
AATcos = 0;
for(x=1; x<=16; x++) //Total 16 samples
  {
  AATcos = AATcos + 2*analogRead(5)-(AATsinmin+AATsinmax); //Read the sensor and subtract the offsets
  AATsin = AATsin + 2*analogRead(4)-(AATsinmin+AATsinmax);
  }
  angle = atan2(float(AATcos)/(AATcosmax-AATcosmin),float(AATsin)/(AATsinmax-AATsinmin)); //Calculate angle
  if(angle<0) {pwmWriteHR(10,0); pwmWriteHR(9,angle/pi*FSn);} //Scale output for 180 mV (1 mV/deg.)
  else {pwmWriteHR(9,0); pwmWriteHR(10,angle/pi*FSp);} //Output 9 for negative; 10 for positive
}
