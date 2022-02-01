//********************************//
//********** MPPT SYSTEM *********//
//************* v1.5 *************//
//********************************//

// Locations of the current sensors, voltage dividers, and other important pins //
const byte locationCurrentIn = A0;
const byte locationCurrentOut = A1;
const byte locationVoltageIn = A2;
const byte locationVoltageOut = A4;
int PWMPin = 6;

// Resolution of the current sensors //
float resolutionIn = 0.1838966;
float resolutionOut = 0.1836130;

// Voltage devider values //
float Vin_R1 = 10000;
float Vin_R2 = 1200;
float Vout_R1 = 10000;
float Vout_R2 = 1200;

// Other global variables that can be changed //
float resolution = 5.0/1024; // resolution of the ADC of the microcontroller/microcomputer
float STEP_SIZE = 1; // step size (= precision) of the MPPT algorithm
float MAX_VOLTAGE = 30.0;
float MIN_VOLTAGE = 10.0;

// Other global variables that cannot be changed //
float zeroPointIn; // zero point of the current sensor before the boost converter
float zeroPointOut; // zero point of the current sensor after the boost converter
float DC = 50; // GLOBAL DC VARIABLE

void setup() {
  Serial.begin(9600);

  // Set PWM frequency for D5 and D6 to 62 [kHz]
  TCCR0B = TCCR0B & B11111000 | B00000001;

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
  Serial.print(currentIn);
  Serial.println(" [A]");

  ///** CURRENT OUT **///
  // ask for an average sensor voltage of 100 measurments
  sensorVoltage = get_voltage(100, locationCurrentOut); 

  // print the voltage output of the sensor
  //Serial.print("Voltage: ");
  //Serial.println(sensorVoltage ,3);

  currentOut = ((sensorVoltage - zeroPointOut)/resolutionOut);
  Serial.print("currentOut = ");
  Serial.print(currentOut);  
  Serial.println(" [A]");

  ///** VOLTAGES **///
  // read  voltage from analog pins
  VinRead = analogRead(locationVoltageIn);
  VoutRead = analogRead(locationVoltageOut);

  // calculate real voltages
  voltageIn = VinRead * (resolution)*((Vin_R1 + Vin_R2)/Vin_R2);
  voltageOut = VoutRead * (resolution)*((Vout_R1 + Vout_R2)/Vout_R2);

  // print voltages
  Serial.print("VoltageIn = ");
  Serial.print(voltageIn);
  Serial.println(" [V]");
  Serial.print("VoltageOut = ");
  Serial.print(voltageOut);
  Serial.println(" [V]");

  ///** Obtain the needed duty cycle for the boost converter using the perturb and observe algortihm **///
  perturbAndObserve(voltageIn, voltageOut, currentIn, currentOut); // THIS IS A PERCENTAGE!
    ///** Set the duty cycle of the boost converter to the desired frequency **///
  if (DC >= 20 && DC <= 80) {
    analogWrite(PWMPin, (DC/100 * 255));
  }
  else {
    analogWrite(PWMPin, (30/100 * 255));
  }
  Serial.print("DC = ");
  Serial.print(DC);
  Serial.println(" [%]");



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
void perturbAndObserve(float Vin, float Vout_new, float Iin, float Iout) {
    float eff;
    float Vout_old;
    float Pout_old;
    
    float Pout_new = Vout_new*Iout;
    float Pin = Vin * Iin;
    
    if(Pout_new >= Pout_old*0.99 || Pout_new <= Pout_old*1.01)
        {
           eff = (Pout_new/Pin) *100;
           Serial.print("eff: ");
           Serial.print(eff);
           Serial.println(" [%]");
        return 0;
        }
        else if (Pout_new > Pout_old)
        {
            if(Vout_new > Vout_old)
            {
                DC = DC + STEP_SIZE;
            }else
            {
                DC = DC- STEP_SIZE;
            }
        }else
        {
            if(Vout_new > Vout_old)
            {
                DC = DC - STEP_SIZE;
            }else
            {
                DC = DC + STEP_SIZE;
            }
        }
   
//    Serial.print("DC:");
//    Serial.println(DC);
   
    eff = (Pout_new/Pin) *100;
    Serial.print("eff: ");
    Serial.print(eff);
    Serial.println(" [%]");
    
    Vout_old = Vout_new;
    Pout_old = Pout_new;

    //** SOME SAFETY CHECKS THAT MAY OVERWRITE THE FINAL VALUE FOR DC **//
    if(Vout_new >= MAX_VOLTAGE)
    {
      DC = DC - 2*STEP_SIZE;
      Serial.println("MAX VOLTAGE REACHED!!");
    }
//  if(Vout_new <= MIN_VOLTAGE)
//  {
//    DC = DC + 2*STEP_SIZE;
//    Serial.println("MIN VOLTAGE REACHED!!");
//  }

    return 0;
}
