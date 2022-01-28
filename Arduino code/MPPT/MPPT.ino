//********************************//
//*** MPPT SYSTEM ***//
//********************************//

// Locations of the current sensors and voltage dividers //
const byte locationCurrentIn = A0;
const byte locationCurrentOut = A1;
const byte locationVoltageIn = A2;
const byte locationVoltageOut = A3;
int PWMPin = 13; //D7 on an ESP8266

// Resolution of the current sensors //
float resolutionIn = 0.1838966;
float resolutionOut = 0.18; // TBD

// Voltage devider values //
float Vin_R1 = 5000.0;
float Vin_R2 = 7500.0;
float Vout_R1 = 2000.0;
float Vout_R2 = 5000.0;

// Other global variables that can be changed //
float resolution = 5.0/1024; // resolution of the ADC of the microcontroller/microcomputer
float STEP_SIZE = 0.1; // step size (= precision) of the MPPT algorithm
int PWMfrequency = 10000; // PWM frequency of the signal to the boost converter in [Hz]

// Other global variables that cannot be changed //
float zeroPointIn; // zero point of the current sensor before the boost converter
float zeroPointOut; // zero point of the current sensor after the boost converter

void setup() {
  Serial.begin(9600);

  analogWriteFreq(PWMfrequency);

  // Automatic Calibration Process for current sensors //
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

  float dutyCycle;

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
  Serial.print("VoltageIn = ");
  Serial.println(voltageIn);
  Serial.print("VoltageOut = ");
  Serial.println(voltageOut);

  ///** Obtain the needed duty cycle for the boost converter using the perturb and observe algortihm **///
  dutyCycle = perturbAndObserve(voltageIn, voltageOut, currentIn, currentOut); // THIS IS A PERCENTAGE!
  Serial.print("DC = ");
  Serial.print(dutyCycle);
  Serial.println(" %");

  ///** Set the duty cycle of the boost converter to the desired frequency **///
  if (dutyCycle >= 0 && dutyCycle <= 100) {
    analogWrite(PWMPin, (dutyCycle/100 * 1024));
  }

  delay(500);

}

///* Function for obtaining voltage reading from the current sensors *///
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

///* MPPT perturb and observe algorithm *///
float perturbAndObserve(float Vin_new, float Vout, float Iin, float Iout) {

    float DC;
    float eff;
    float Vin_old;
    float Pin_old;
    
    float Pout = Vout*Iout;
    float Pin_new = Vin_new * Iin;
    
    if(Pin_new == Pin_old)
        {
        return DC;
        }
        else if (Pin_new > Pin_old)
        {
            if(Vin_new > Vin_old)
            {
                Vin_new = Vin_new + STEP_SIZE;
            }else
            {
                Vin_new = Vin_new - STEP_SIZE;
            }
        }else
        {
            if(Vin_new > Vin_old)
            {
                Vin_new = Vin_new - STEP_SIZE;
            }else
            {
                Vin_new = Vin_new + STEP_SIZE;
            }
        }
    DC = (1 - Vin_new/Vout) * 100; // Duty cycle in percentages
//    Serial.print("DC:");
//    Serial.println(DC);
   
    eff = (Pin_new - Pout)/Pin_new *100;
    Serial.print("eff:");
    Serial.println(eff);
    
    Vin_old = Vin_new;
    Pin_old = Pin_new;

    return DC;
}
