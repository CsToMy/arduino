#include <DHT.h>
#include <ESP8266WiFi.h>
#include <MQTTPubSubClient.h>

const int PIN_DHT = 5;
DHT dht(PIN_DHT, 22);

// WIFI
const char* NETWORK_SSID = "Anett_lak";
const char* NETWORK_PASSWD = "paprikaskrumpli";

// MQTT
const char* MQTT_BROKER = "192.168.0.32";
String MQTT_USER = "TEST_NODE_MCU12";
const char* MQTT_PASSWD = "almafa";
const int MQTT_PORT = 1883;

WiFiClient networkClient;
MQTTPubSubClient mqttClient;

float humi;
float temp;

void setup() {
  pinMode(PIN_DHT, INPUT);
  
  dht.begin();
  
  temp = 0;
  humi = 0;
  
  WiFi.begin(NETWORK_SSID, NETWORK_PASSWD);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    
  }
  networkClient.connect(MQTT_BROKER, MQTT_PORT);
  
  mqttClient.begin(networkClient);
  
  while (!mqttClient.isConnected()) {
      String client_id = MQTT_USER+"_";
      client_id += String(WiFi.macAddress());
      if (mqttClient.connect(client_id.c_str(), MQTT_USER)) {
          //mqttClient.subscribe("SMARTHOME/CONFIG/#", msgArrived);
          mqttClient.subscribe("smarthome/test", sendMeasurement);
          mqttClient.publish("smarthome/inside/ping", "I am alive.");
      } else {
          delay(1000);
      }
  }
}

float measurement = 0;
String responseTopic = "smarthome/inside/";

void sendMeasurement(const String& payload, const size_t size) {
  responseTopic = "smarthome/test/response";
  String response = "TEST_";

  humi = dht.readHumidity();
  temp = dht.readTemperature();

  response = response + temp + "_" + humi;
  mqttClient.publish(responseTopic, response);
}


void loop() {
  //Serial.println("Waiting...");
  
  //Print temp and humidity values to serial monitor
  /*Serial.print("Humidity: ");
  Serial.print(humi);
  Serial.print(" %, Temp: ");
  Serial.print(temp);
  Serial.println(" Celsius.");
  */
  mqttClient.update();
  delay(500); //Delay 0.5 sec.
}
