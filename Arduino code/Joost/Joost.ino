double Vin_old, Vin_new, Vout, Iin, Iout, Pin_old, Pin_new, Pout, DC, eff;

void setup() {
  Serial.begin(9600);
}

void loop() {

   Vin_new = random();
   Vout = random();
   Iin = random();
   Iout = random();

  delay(1000);
   DC = dutyCycle();
   

}

double dutyCycle() {
  float STEP_SIZE = 0.1;
  
    Pout = Vout*Iout;
    Pin_new = Vin_new * Iin;
    
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
    DC = 1- Vin_new/Vout;
    Serial.print("DC:");
    Serial.println(DC);
   
    eff = (Pin_new - Pout)/Pin_new *100;
    Serial.print("eff:");
    Serial.println(eff);
    
    Vin_old = Vin_new;
    Pin_old = Pin_new;

    return DC;
}
