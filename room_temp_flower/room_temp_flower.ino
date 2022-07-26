#include <DHT.h>
#include <ESP8266WiFi.h>
#include <MQTTPubSubClient.h>
#include <MQ2.h>

const char PIN_DHT = 14; // D5

const char PIN_A = 0; // D3
const char PIN_B = 4; // D2
const char PIN_C = 5; // D1

const char PIN_INH = 2; // D4

// Identifiers
const String DEVICE_XID = "WEMOSD1MINI";
const String MODULE_NAME = "LivingroomModule";

// WIFI
const char* NETWORK_SSID = "Anett_lak";
const char* NETWORK_PASSWD = "paprikaskrumpli";

// MQTT
const char* MQTT_BROKER = "192.168.0.32";
String MQTT_USER = "ROOM_UNDER_GALERY_1";
const char* MQTT_PASSWD = "almafa";
const int MQTT_PORT = 1883;

DHT dht(PIN_DHT, 22);
WiFiClient networkClient;
MQTTPubSubClient mqttClient;

MQ2 mq2(A0);

void setup() {
  Serial.begin(115200);
  pinMode(PIN_DHT, INPUT);
  pinMode(PIN_A, OUTPUT);
  pinMode(PIN_B, OUTPUT);
  pinMode(PIN_C, OUTPUT);
  pinMode(PIN_INH, OUTPUT);

  dht.begin();
  mq2.begin();
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
          //mqttClient.subscribe("SMARTHOME/CONFIG/#", msgArrived);
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

int readLightSensor() {

  // On chanel 3
  // A B C
  // 1 1 0
  digitalWrite(PIN_INH, HIGH);
  digitalWrite(PIN_A, HIGH);
  digitalWrite(PIN_B, HIGH);

  
  digitalWrite(PIN_C, LOW);
  digitalWrite(PIN_INH, LOW);
  delay(40);
  
  int lightLevel = 0;
  lightLevel = analogRead(A0);

  digitalWrite(PIN_INH, HIGH);
  return lightLevel;
}

int readNoiseSensor() {
  // On chanel 1
  // A B C
  // 1 0 0
  digitalWrite(PIN_INH, HIGH);
  digitalWrite(PIN_A, HIGH);
  digitalWrite(PIN_B, LOW);
  digitalWrite(PIN_C, LOW);
  digitalWrite(PIN_INH, LOW);
  delay(40);
  
  int noiseLevel = 0;
  noiseLevel = analogRead(A0);

  digitalWrite(PIN_INH, HIGH);
  return noiseLevel;  
}

float readLPGLevel()
{
  // On chanel 5
  // A B C
  // 1 0 1
  digitalWrite(PIN_INH, HIGH);
  digitalWrite(PIN_A, HIGH);
  digitalWrite(PIN_B, LOW);
  digitalWrite(PIN_C, HIGH);
  digitalWrite(PIN_INH, LOW);
  delay(40);
  float lpg = mq2.readLPG();
  digitalWrite(PIN_INH, HIGH);
  return lpg;
}

float readCOLevel()
{
  // On chanel 5
  // A B C
  // 1 0 1
  digitalWrite(PIN_INH, HIGH);
  digitalWrite(PIN_A, HIGH);
  digitalWrite(PIN_B, LOW);
  digitalWrite(PIN_C, HIGH);
  digitalWrite(PIN_INH, LOW);
  delay(40);
  float co = mq2.readCO();
  digitalWrite(PIN_INH, HIGH);
  return co;
}

float readSmokeLevel()
{
  // On chanel 5
  // A B C
  // 1 0 1
  digitalWrite(PIN_INH, HIGH);
  digitalWrite(PIN_A, HIGH);
  digitalWrite(PIN_B, LOW);
  digitalWrite(PIN_C, HIGH);
  digitalWrite(PIN_INH, LOW);
  delay(40);
  float smoke = mq2.readSmoke();
  digitalWrite(PIN_INH, HIGH);
  return smoke;
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
    measurement = readLightSensor();
  } else if(payload == "soil") {
    responseTopic = responseTopic + "soil/response";
    //measurement = readGroundSensor();
    response = response + measurement + "_";
    //measurement = read2ndGroundLevelSensor();
  } else if(payload == "noise") {
    responseTopic = responseTopic + "noise/response";
    measurement = readNoiseSensor();
  } else if(payload == "gas") {
    responseTopic = responseTopic + "gas/response";
    
    measurement = readLPGLevel();
    response = response + measurement;
    
    measurement = readCOLevel();
    response = response + "_" + measurement;
    
    measurement = readSmokeLevel();
    response = response + "_" + measurement;
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
  /*delay(1000);
  
  Serial.print("Light level: " );
  int light = readLightSensor();
  Serial.println(light);
  delay(200);
  
  float  humi = dht.readHumidity();
  float temp= dht.readTemperature();
  
  Serial.print("Temperature: ");
  Serial.println(temp);
  
  Serial.print("Humidity: ");
  Serial.println(humi);
  delay(300);

  float lpg = readLPGLevel();
  float co = readCOLevel();
  float smoke = readSmokeLevel();

  Serial.print("LPG: ");
  Serial.println(lpg);
  Serial.print("CO: ");
  Serial.println(co);
  Serial.print("Smoke: ");
  Serial.println(smoke);
  
  Serial.println();
  int noise = readNoiseSensor();
  Serial.print("Noise: ");
  Serial.println(noise);
  delay(200);
  Serial.println("#############################");*/
}
