#include <DHT.h>

const char PIN_DHT = 4;
DHT dht(PIN_DHT, 22);
float temperature;
float humidity;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(PIN_DHT, INPUT);
  Serial.println("Pin mode set.");
  temperature = 0;
  humidity = 0;
  
  dht.begin();
  Serial.println("End of setup.");   
}

void loop() {
  // put your main code here, to run repeatedly:
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  Serial.print("Temperature: ");
  Serial.println(temperature);

  Serial.print("Humidity: ");
  Serial.println(humidity);

  Serial.println("I'm awake, but I'm going into deep sleep mode for 30 seconds");
  ESP.deepSleep(30e6);
}
