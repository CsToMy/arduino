#include <DHT.h>

const char DHT_PIN = 2;
DHT dht22(DHT_PIN, 22);
/*
const char led1 = 5; // D1
const char led2 = 4; // D2
const char led3 = 0; // D3
const char led4 = 2; // D4

const char led5 = 16; // D0
const char led6 = 14; // D5
const char led7 = 12; // D6
const char led8 = 13; // D7
const char led9 = 15; // D8 
*/

float temp;
float humi;

void setup() {
  // put your setup code here, to run once:
  /*pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
  pinMode(led6, OUTPUT);
  pinMode(led7, OUTPUT);
  pinMode(led8, OUTPUT);
  pinMode(led9, OUTPUT);
  */
  temp = 0;
  humi = 0;
  pinMode(DHT_PIN, INPUT);
  dht22.begin();
  Serial.begin(115200);
  Serial.println("Started.");
  delay(3700);
}


void loop() {
  // put your main code here, to run repeatedly:
/*
  delay(600);
  digitalWrite(led1, HIGH);
  delay(600);
  digitalWrite(led1, LOW);
  
  delay(600);
  digitalWrite(led2, HIGH);
  delay(600);
  digitalWrite(led2, LOW);

  delay(600);
  digitalWrite(led3, HIGH);
  delay(600);
  digitalWrite(led3, LOW);
  
  delay(600);
  digitalWrite(led4, HIGH);
  delay(600);
  digitalWrite(led4, LOW);

  delay(600);
  digitalWrite(led5, HIGH);
  delay(600);
  digitalWrite(led5, LOW);

  delay(600);
  digitalWrite(led6, HIGH);
  delay(600);
  digitalWrite(led6, LOW);

  delay(600);
  digitalWrite(led7, HIGH);
  delay(600);
  digitalWrite(led7, LOW);

  delay(600);
  digitalWrite(led8, HIGH);
  delay(600);
  digitalWrite(led8, LOW);

  delay(600);
  digitalWrite(led9, HIGH);
  delay(600);
  digitalWrite(led9, LOW);
  */
  temp = dht22.readTemperature();
  humi = dht22.readHumidity();
  Serial.print("Temperature = ");
  Serial.println(temp);
  Serial.print("Humidity = ");
  Serial.println(humi);
  Serial.print("Light = ");
  Serial.println(analogRead(A0));
  Serial.println("###############");
  delay(4000);
}
