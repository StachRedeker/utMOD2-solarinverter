// pins and variables
int A0read = 0; // value read from A0
int A1read = 0; // value read from A1
float voltage1; // first voltage readout
float voltage2; // second voltage readout

// voltage devider values
float v1_R1 = 82000.0;
float v1_R2 = 10000.0;
float v2_R1 = 75000.0;
float v2_R2 = 1000.0;

void setup(){
  Serial.begin(9600); //start serial connection
}

void loop(){
  // read  voltage from analog pins
  A0read = analogRead(A0);
  A1read = analogRead(A0);

  // calculate real voltages
  voltage1 = A0read * (3.3/1024)*((v1_R1 + v1_R2)/v1_R2);
  voltage2 = A1read * (3.3/1024)*((v2_R1 + v2_R2)/v2_R2);

  // print voltages every 0.5 seconds
  Serial.print("Voltage1 =");
  Serial.println(voltage1);
  Serial.print("Voltage2 =");
  Serial.println(voltage2);
  delay(500);
}
