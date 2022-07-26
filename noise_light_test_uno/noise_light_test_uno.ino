void setup() {
  // put your setup code here, to run once:
  //pinMode(3, INPUT);
  Serial.begin(115200);
  Serial.println("Started.");
  delay(3200);
}

int noiseValue = 0;
int noiseValue2 = 0;
int noiseValue3 = 0;
int digitalNoiseValue = 0;
void loop() {
  noiseValue = analogRead(A0);
  //digitalNoiseValue = digitalRead(3);
  Serial.println(noiseValue);
  //Serial.println(digitalNoiseValue);
  Serial.println("###################");
  
  /*noiseValue2 = analogRead(A1);
  //Serial.print("A1 Noise value: ");
  Serial.println(noiseValue2);

  noiseValue3 = analogRead(A2);
  //Serial.print("A2 Noise value: ");
  Serial.println(noiseValue3);
  */
  delay(1000);
}
