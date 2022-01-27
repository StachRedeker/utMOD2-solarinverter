int averageArray[10];
char j, k;
long result;

void setup() {
    Serial.begin(9600);
}

void loop() {  

  for (j = 0; j < 10; j++) {
    averageArray[j] = random();
    Serial.println(averageArray[j]);
  }

  // loop through the array and average
  result = 0;
  for (j = 0; j < 10; j++) {
    result = result + averageArray[j];
  }
  result = result / 10;
  
  delay(500);
  Serial.print("Result:");
  Serial.println(result);
}
