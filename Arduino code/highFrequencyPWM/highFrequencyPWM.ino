int PWMPin = 6;

void setup() {

  TCCR0B = TCCR0B & B11111000 | B00000001;
  Serial.begin(9600);
  Serial.println("**** DUTY CYCLE SELECTOR, put in 1 to 255 ****");
}

void loop() {
  while (Serial.available() == 0) {
  }

  float dutyCycle = Serial.parseFloat();

  if (dutyCycle > 0 && dutyCycle < 255) {
  analogWrite(PWMPin, dutyCycle);

  Serial.print("Set to: ");
  Serial.println(dutyCycle);
  }

}
