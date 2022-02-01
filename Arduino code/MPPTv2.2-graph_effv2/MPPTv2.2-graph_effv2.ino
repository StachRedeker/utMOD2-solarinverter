//********************************//
//********** MPPT SYSTEM *********//
//************* v2.2 *************//
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
float Vin_R1 = 19945.0;
float Vin_R2 = 1193.0;
float Vout_R1 = 19923.0;
float Vout_R2 = 1194.1;

// Other global variables that can be changed //
float resolution = 5.0/1024; // resolution of the ADC of the microcontroller/microcomputer
float STEP_SIZE = 0.2; // step size (= precision) of the MPPT algorithm
float MAX_VOLTAGE = 45.0; //used to be 88, but current diode can only handle max 50 [V]
float MIN_VOLTAGE = 25.0;

// Other global variables that cannot be changed //
float zeroPointIn; // zero point of the current sensor before the boost converter
float zeroPointOut; // zero point of the current sensor after the boost converter
float DC; // GLOBAL DC VARIABLE
float Vin_old;
float Vout_old;
float Pout_old;
float Pin_old;

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

  if (currentIn < 0) 
  {
    currentIn = 0.01;
  }
//  Serial.print("currentIn = ");
  Serial.print(currentIn);
  Serial.print(" ");
//  Serial.println(" [A]");

  ///** CURRENT OUT **///
  // ask for an average sensor voltage of 100 measurments
  sensorVoltage = get_voltage(100, locationCurrentOut); 

  // print the voltage output of the sensor
  //Serial.print("Voltage: ");
  //Serial.println(sensorVoltage ,3);

  currentOut = ((sensorVoltage - zeroPointOut)/resolutionOut);

  if (currentOut < 0) 
  { 
    currentOut = 0.01;
  }
// Serial.print("currentOut = ");
  Serial.print(currentOut); 
  Serial.print(" "); 
//  Serial.println(" [A]");

  ///** VOLTAGES **///
  voltageIn = calculate_voltage(100, locationVoltageIn);
  voltageOut = calculate_voltage(100, locationVoltageOut);

  // print voltages
// Serial.print("VoltageIn = ");
Serial.print(voltageIn);
Serial.print(" ");
//  Serial.println(" [V]");
// Serial.print("VoltageOut = ");
 Serial.print(voltageOut);
 Serial.print(" ");
//  Serial.println(" [V]");

  ///** Obtain the needed duty cycle for the boost converter using the perturb and observe algortihm **///
  perturbAndObserve(voltageIn, voltageOut, currentIn, currentOut); // THIS IS A PERCENTAGE!
    ///** Set the duty cycle of the boost converter to the desired frequency **///
   if (DC < 20)
    {
    DC = 20;
    analogWrite(PWMPin, (DC/100 * 255));
    }
  else if(DC > 70) 
    {
    DC = 50;
    analogWrite(PWMPin, (DC/100 * 255));
    }
  else
    {
    analogWrite(PWMPin, (DC/100 * 255));
    }
//Serial.print("DC = ");
  Serial.print(DC);
  Serial.print(" ");
//  Serial.println(" [%]");

  //delay(20000);

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

///* Function for obtaining voltage readings from the voltage dividers *///
float calculate_voltage(int n_datapoints, const byte pin)
{
  float voltage = 0;

  for(int i=0; i < n_datapoints; i++)
  {
    voltage = voltage + analogRead(pin) * (resolution)*((Vin_R1 + Vin_R2)/Vin_R2);
  }

  voltage = voltage/n_datapoints;
  return(voltage);
}


///* MPPT perturb and observe algorithm *///
void perturbAndObserve(float Vin_new, float Vout_new, float Iin, float Iout) 
  {
    float eff;
    
    float Pout_new = Vout_new*Iout;
    float Pin_new = Vin_new *Iin;

 //   Serial.print("Pout_new = ");
   Serial.print(Pout_new);
   Serial.print(" ");
//   Serial.print("Pin = ");
   Serial.print(Pin_new);
   Serial.print(" ");

    //** SOME SAFETY CHECKS THAT MAY OVERWRITE THE FINAL VALUE FOR DC **//
    if(Vout_new >= MAX_VOLTAGE)
    {
      DC = DC - 10*STEP_SIZE;
    //  Serial.println("MAX VOLTAGE REACHED!!");
    Serial.println("");
      return 0;
    }
    if(Vout_new <= MIN_VOLTAGE)
    {
      DC = DC + 10*STEP_SIZE;
  //    Serial.println("MIN VOLTAGE REACHED!!");
  Serial.println("");
      return 0;
    }
    
        if (Pin_new > Pin_old)
        {
            if(Vout_new > Vout_old)
            {
                DC = DC + STEP_SIZE;
            }else
            {
                DC = DC - STEP_SIZE;
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
  
   
    eff = (Pout_new/Pin_new) *100;
 //   Serial.print("eff = ");
   Serial.println(eff);
 //   Serial.println(" [%]");
  
    Vin_old = Vin_new;
    Vout_old = Vout_new;
    Pout_old = Pout_new;
    Pin_old = Pin_new;
    
  return 0;
}
