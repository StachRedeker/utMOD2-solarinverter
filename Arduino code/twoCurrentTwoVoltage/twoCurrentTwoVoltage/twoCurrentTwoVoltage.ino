//********************************//
//*** TWO TIMES CURRENT USING HALL EFFECT SENSOR ***// and
//*** TWO TIMES VOLTAGE USING VOLTAGE DIVIDER ***//
//********************************//

// Locations of the current sensors and voltage dividers //
const byte locationCurrentIn = A0;
const byte locationCurrentOut = A1;
const byte locationVoltageIn = A2;
const byte locationVoltageOut = A3;

// Resolution of the current sensors //
float resolutionIn = 0.1838966;
float resolutionOut = 0.18; // TBD

// Voltage devider values //
float Vin_R1 = 5000.0;
float Vin_R2 = 7500.0;
float Vout_R1 = 2000.0;
float Vout_R2 = 5000.0;

// Other global variables //
float zeroPointIn; // zero point of the current sensor before the boost converter
float zeroPointOut; // zero point of the current sensor after the boost converter
float resolution = 5.0/1024; // resolution of the ADC of the microcontroller/microcomputer

void setup() {
  Serial.begin(9600);

  // Automatic Calibration Process //
  // This calibration assumes that the resolution of the ACS712 stays the same, but the voltage readout for 0 [A] fluctuates //
  // To calibrate, make sure that the script initializes with 0 [A] on the sensor //
  zeroPointIn = get_voltage(10000, locationCurrentIn);
  Serial.print("Zero point IN = ");
  Serial.println(zeroPointIn);
  zeroPointOut = get_voltage(10000, locationCurrentOut);
  Serial.print("Zero point OUT = ");
  Serial.println(zeroPointOut);
}


void loop() {

  float currentIn;
  float currentOut;

  int VinRead = 0; // value read for Vin
  int VoutRead = 0; // value read for Vout
  float voltageIn; 
  float voltageOut; 

  ///** CURRENT IN **///
  // ask for an average sensor voltage of 100 measurments
  float sensorVoltage = get_voltage(100, locationCurrentIn); 

  // print the voltage output of the sensor
  //Serial.print("Voltage: ");
  //Serial.println(sensorVoltage ,3);

  currentIn = ((sensorVoltage - zeroPointIn)/resolutionIn);
  Serial.print("currentIn = ");
  Serial.println(currentIn);

  ///** CURRENT OUT **///
  // ask for an average sensor voltage of 100 measurments
  sensorVoltage = get_voltage(100, locationCurrentOut); 

  // print the voltage output of the sensor
  //Serial.print("Voltage: ");
  //Serial.println(sensorVoltage ,3);

  currentOut = ((sensorVoltage - zeroPointOut)/resolutionOut);
  Serial.print("currentOut = ");
  Serial.println(currentOut);  

  ///** VOLTAGES **///
    // read  voltage from analog pins
  VinRead = analogRead(locationVoltageIn);
  VoutRead = analogRead(locationVoltageOut);

  // calculate real voltages
  voltageIn = VinRead * (resolution)*((Vin_R1 + Vin_R2)/Vin_R2);
  voltageOut = VoutRead * (resolution)*((Vout_R1 + Vout_R2)/Vout_R2);

  // print voltages
  Serial.print("VoltageIn =");
  Serial.println(voltageIn);
  Serial.print("VoltageOut =");
  Serial.println(voltageOut);

  delay(500);

}

float get_voltage(int n_datapoints, const byte pin)
{

  float voltage = 0;

  for(int i=0; i < n_datapoints; i++)
  {
    voltage = voltage + analogRead(pin) * (resolution);
  }

  voltage = voltage/n_datapoints;
  return(voltage);
}
