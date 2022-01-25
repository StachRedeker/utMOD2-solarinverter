#include <Arduino.h>

// #define pin D9

// void setup() {
//   //STM32 Configuration data
//   TIM_TypeDef *Instance = (TIM_TypeDef *)pinmap_peripheral(digitalPinToPinName(pin), PinMap_PWM);
//   uint32_t channel = STM_PIN_CHANNEL(pinmap_function(digitalPinToPinName(pin), PinMap_PWM));
//   HardwareTimer *MyTim = new HardwareTimer(Instance);
//   MyTim->setPWM(channel, pin, 10000, 50); // 5 Hertz, 10% dutycycle

//   //Arduino commands
//   analogReadResolution(12);
//   Serial.begin(9600); //start serial connection
// }

// void loop() {
//   //Serial.print(analogRead(PA0)*3300/4096);
//   Serial.println(analogRead(PA0));
//   delay(10);
// }

/*
Measuring AC Current Using ACS712
*/
// const int sensorIn = A0;
// int mVperAmp = 185; // use 100 for 20A Module and 66 for 30A Module
 
// double Voltage = 0;
// double VRMS = 0;
// double AmpsRMS = 0;
 
// float getVPP()
// {
//   float result;
 
//   int readValue;             //value read from the sensor
//   int maxValue = 0;          // store max value here
//   int minValue = 4096;          // store min value here
 
//    uint32_t start_time = millis();
//    while((millis()-start_time) < 1000) //sample for 1 Sec
//    {
//        readValue = analogRead(sensorIn);
//        // see if you have a new maxValue
//        if (readValue > maxValue) 
//        {
//            /*record the maximum sensor value*/
//            maxValue = readValue;
//        }
//        if (readValue < minValue) 
//        {
//            /*record the maximum sensor value*/
//            minValue = readValue;
//        }
//    }
 
//    // Subtract min from max
//    result = ((maxValue - minValue) * 3.3)/4096.0;
 
//    return result;
//  }

// void setup(){ 
//  Serial.begin(9600);
// }
 
// void loop(){
 
//  Voltage = getVPP();
//  VRMS = (Voltage/2.0) *0.707; 
//  AmpsRMS = (VRMS * 1000)/mVperAmp;
//  Serial.print(AmpsRMS);
//  Serial.println(" Amps RMS");
 
// }
// pins and variables
int A0read = 0; // value read from A0
int A1read = 0; // value read from A1
float voltage1; // first voltage readout
float voltage2; // second voltage readout
int def1, def2;
float voltage1_1, voltage2_1;
 
// voltage devider values
float v1_R1 = 82000.0; // measure across this one (A0)
float v1_R2 = 10000.0;
float v2_R1 = 75000.0; // measure across this one (A1)
float v2_R2 = 1000.0;
 
void setup(){
  Serial.begin(9600); //start serial connection
  //analogReadResolution(8);
}
 
void loop(){
  // read  voltage from analog pins
  A0read = analogRead(A0);
  A1read = analogRead(A1);
 
  // calculate real voltages
  voltage1 = A0read * (3.3/1024)*((v1_R1 + v1_R2)/v1_R2);
  voltage2 = A1read * (3.3/1024)*((v2_R1 + v2_R2)/v2_R2);
 
  //print voltages every 0.5 seconds
  Serial.print("Voltage1 =");
  Serial.println(voltage1);
  Serial.print("Voltage2 =");
  Serial.println(voltage2);
  delay(1000);
}