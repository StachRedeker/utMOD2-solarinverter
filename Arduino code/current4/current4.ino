//*** CURRENT USING HALL EFFECT SENSOR ***//

float zeroPoint;
float resolution = 0.1838966;

void setup() {
  Serial.begin(9600);

  // Automatic Calibration Process //
  // This calibration assumes that the resolution of the ACS712 stays the same, but the voltage readout for 0 A fluctuates //
  // To calibrate, make sure that the script initializes with 0A on the sensor //
  zeroPoint = get_voltage(10000);
  Serial.print("Zero point = ");
  Serial.println(zeroPoint);
}


void loop() {

  // ask for an average sensor voltage of 10000 measurments
  float sensorVoltage = get_voltage(100); 
  float current;

  // print the voltage output of the sensor
  Serial.print("Voltage: ");
  Serial.println(sensorVoltage ,3);

  current = ((sensorVoltage - zeroPoint)/resolution);
  Serial.println(current);

}

float get_voltage(int n_datapoints)
{

  float voltage = 0;

  for(int i=0; i < n_datapoints; i++)
  {
    voltage = voltage + analogRead(A1) * (5.0 / 1023.0);
  }

  voltage = voltage/n_datapoints;
  return(voltage);
}
