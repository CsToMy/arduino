
int pinInput = 23;

void setup() {
  // put your setup code here, to run once:
  pinMode(pinInput, INPUT);
  Serial.begin(115200);
  Serial.println("Begin...");
  Serial.println("############################");
}

void loop() {
  // put your main code here, to run repeatedly:
  int movement = digitalRead(pinInput);
  Serial.println(movement);
  delay(5000);
}
