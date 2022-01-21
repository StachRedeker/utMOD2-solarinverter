int PWMPin = 13;  

void setup() {

  analogWriteFreq(10000);
  Serial.begin(9600);
  Serial.println("**** DUTY CYCLE SELECTOR, put in 1 to 255 ****");
}

void loop() {
  while (Serial.available() == 0) {
  }

  float dutyCycle = Serial.parseFloat();

  if (dutyCycle > 0 && dutyCycle < 1024) {
  analogWrite(PWMPin, dutyCycle);

  Serial.print("Set to: ");
  Serial.println(dutyCycle);
  }

}
