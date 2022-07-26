const char PIN_A = 2; // D1
const char PIN_B = 3; // D2
const char PIN_C = 4; // D3

const char PIN_INH = 7; // D7
void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_A, OUTPUT);
  pinMode(PIN_B, OUTPUT);
  pinMode(PIN_C, OUTPUT);
  pinMode(PIN_INH, OUTPUT);
  Serial.begin(115200);
  Serial.println("Begin.");
  delay(2000);
}

void loop() {
  // put your main code here, to run repeatedly:
  // module on ch 2
  digitalWrite(PIN_INH, HIGH);
  digitalWrite(PIN_A, LOW);
  digitalWrite(PIN_B, HIGH);
  digitalWrite(PIN_C, LOW);
  digitalWrite(PIN_INH, LOW);
  delay(300);
  Serial.print("Module: ");
  Serial.println(analogRead(A0));
  delay(1000);
  
  // sensor on ch 3
  digitalWrite(PIN_INH, HIGH);
  digitalWrite(PIN_A, HIGH);
  digitalWrite(PIN_B, HIGH);
  digitalWrite(PIN_C, LOW);
  digitalWrite(PIN_INH, LOW);
  delay(300);
  Serial.print("Sensor: ");
  Serial.println(analogRead(A0));
  delay(6000);

  Serial.println("###########################");
}
