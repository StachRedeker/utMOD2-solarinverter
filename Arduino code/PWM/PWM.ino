int PWMPin = 9;  
double incomingByte = 0;

void setup() {
  Serial.begin(9600); //start serial connection
}

void loop() {
//  for (int PWMValue = 0 ; PWMValue <= 255; PWMValue += 5) {
//    analogWrite(PWMPin, PWMValue);
//    Serial.println(PWMValue);
//    delay(100);
//  }

  // send data only when you receive data:
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();

    // say what you got:
    Serial.print("I received: ");
    Serial.println(incomingByte, DEC);
  }

}
