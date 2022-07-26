
const int PIN_VOLTAGE = 12;

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_VOLTAGE, OUTPUT);
  Serial.begin(115200);
  Serial.println("begin");
  
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(PIN_VOLTAGE, HIGH);
  delay(15);
  
  Serial.println(analogRead(A0));
  digitalWrite(PIN_VOLTAGE, LOW);
  delay(2500);
}
