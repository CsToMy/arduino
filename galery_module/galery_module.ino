#include <DHT.h>
#include <ESP8266WiFi.h>
#include <MQTTPubSubClient.h>
//#include <MQ2.h>

const char PIN_DHT = 2;
const char PIN_SHAKE = 14;

const char PIN_IHN = 0;
const char PIN_A = 16;
const char PIN_B = 5;
const char PIN_C = 4;

float temperature;
float humidity;

// Identifiers
const String DEVICE_XID = "ESP12E";
const String MODULE_NAME = "GaleryModule";

// WIFI
const char* NETWORK_SSID = "Anett_lak";
const char* NETWORK_PASSWD = "paprikaskrumpli";

// MQTT
const char* MQTT_BROKER = "192.168.0.32";
String MQTT_USER = "ROOM_GALERY_1";
const char* MQTT_PASSWD = "almafa";
const int MQTT_PORT = 1883;

DHT dht(PIN_DHT, 22);
WiFiClient networkClient;
MQTTPubSubClient mqttClient;

//MQ2 mq2(A0);

void setup() {
  Serial.begin(115200);
  
  pinMode(PIN_DHT, INPUT);
  pinMode(PIN_SHAKE, INPUT);
  pinMode(PIN_IHN, OUTPUT);
  pinMode(PIN_A, OUTPUT);
  pinMode(PIN_B, OUTPUT);
  pinMode(PIN_C, OUTPUT);
  Serial.println("Pin mode set.");

  temperature = 0;
  humidity = 0;
  
  dht.begin();
  //mq2.begin();
  delay(8000); 

  WiFi.begin(NETWORK_SSID, NETWORK_PASSWD);
  Serial.print("Connecting to WiFi.");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi network.");

  networkClient.connect(MQTT_BROKER, MQTT_PORT);
  
  mqttClient.begin(networkClient);
  
  while (!mqttClient.isConnected()) {
      String client_id = MQTT_USER+"_";
      client_id += String(WiFi.macAddress());
      Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
      if (mqttClient.connect(client_id.c_str(), MQTT_USER)) {
          Serial.println("Connected to the broker.");
          mqttClient.subscribe("smarthome/inside", sendMeasurement);
          mqttClient.subscribe("smarthome/mobileclient/inside", sendMeasurementToClient);
          mqttClient.publish("smarthome/inside/ping", "I am alive.");
      } else {
          Serial.print("Failed with state ");
          delay(1000);
      }
  }

  Serial.println("End of setup.");
}

int isShakening() {
  int isShaking = digitalRead(PIN_SHAKE);
  return isShaking;
}

int readLightLevel() {
  // On channel 5
  digitalWrite(PIN_IHN, HIGH);
  digitalWrite(PIN_A, HIGH);
  digitalWrite(PIN_B, LOW);
  digitalWrite(PIN_C, HIGH);
  digitalWrite(PIN_IHN, LOW);
  delay(40);

  int lightLevel = analogRead(A0);
  return lightLevel;
}

int readNoiseLevel() {
  // On channel 3
  digitalWrite(PIN_IHN, HIGH);
  digitalWrite(PIN_A, HIGH);
  digitalWrite(PIN_B, HIGH);
  digitalWrite(PIN_C, LOW);
  digitalWrite(PIN_IHN, LOW);
  delay(40);

  int noiseLevel = analogRead(A0);
  return noiseLevel;  
}

float measurement = 0;
String responseTopic = "smarthome/inside/";

void sendMeasurementToClient(const String& payload, const size_t size) {
    responseTopic = "smarthome/mobileclient/inside/";
    sendMeasurement(payload, size);    
}

void sendMeasurement(const String& payload, const size_t size) {
  String response = String(DEVICE_XID);
  response = response + "_" + MODULE_NAME + "_";
  measurement = 0;
  
  if(payload == "temperature") {
    responseTopic = responseTopic + "temperature/response";
    measurement = dht.readTemperature();
  } else if(payload == "humidity") {
    responseTopic = responseTopic + "humidity/response";
    measurement = dht.readHumidity();
  } else if(payload == "light") {
    responseTopic = responseTopic + "light/response";
    measurement = readLightLevel();
  } else if(payload == "noise") {
    responseTopic = responseTopic + "noise/response";
    measurement = readNoiseLevel();
  } else {
    responseTopic.concat("error/unknownpayload/");
    responseTopic.concat(payload);
    measurement = -1;
  }

  response = response + measurement;
  mqttClient.publish(responseTopic, response);

  responseTopic = "smarthome/inside/";
}

void loop() {
  mqttClient.update();
  delay(350);
  /*while (!mqttClient.isConnected()) {
      String client_id = MQTT_USER+"_";
      client_id += String(WiFi.macAddress());
      if (mqttClient.connect(client_id.c_str(), MQTT_USER)) {
          Serial.println("Connected to the broker.");
          mqttClient.subscribe("smarthome/inside", sendMeasurement);
          mqttClient.subscribe("smarthome/mobileclient/inside", sendMeasurementToClient);
          mqttClient.publish("smarthome/inside/ping", "I am alive.");
      } else {
          Serial.print("Failed with state ");
          delay(1000);
      }
  }
  delay(10000);-**/
  /*
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
  int light = readLightLevel();
  int noise = readNoiseLevel();
  Serial.print("Temperature: ");
  Serial.println(temperature);
  
  Serial.print("Humidity: ");
  Serial.println(humidity);
  
  Serial.print("Light: ");
  Serial.println(light);

  Serial.print("Noise: ");
  Serial.println(noise);

  delay(10000);*/
}
