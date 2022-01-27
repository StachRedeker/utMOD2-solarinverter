int PWMPin = 13; //pin D7 on an ESP8266
int PWMfrequency = 20000;  // in Hz

void setup() {

  analogWriteFreq(PWMfrequency);
  Serial.begin(9600);
  Serial.println("**** DUTY CYCLE SELECTOR, put in a percentage ****");
}

void loop() {
  while (Serial.available() == 0) {
  }

  float userInputPercentage = Serial.parseFloat();
  float dutyCycle = userInputPercentage/100 * 1024;

  if (dutyCycle > 0 && dutyCycle < 1024) {
  analogWrite(PWMPin, dutyCycle);

  Serial.print("Set to: ");
  Serial.println(dutyCycle);
  }

}
