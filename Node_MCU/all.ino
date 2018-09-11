/*
 Basic ESP8266 MQTT example

 This sketch demonstrates the capabilities of the pubsub library in combination
 with the ESP8266 board/library.

 It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic" every two seconds
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
  - If the first character of the topic "inTopic" is an 1, switch ON the ESP Led,
    else switch it off

 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.

 To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"

*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <string.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_ADS1015.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <MoistureSensor.h>

#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DallasTemperature DS18B20sensor(&oneWire);


#define DHTPIN            0        // Pin which is connected to the DHT sensor.
#define DHTTYPE           DHT11     // DHT 11
#define ONE_WIRE_BUS 2

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;
// Update these with values suitable for your network.

//const char* ssid = "DJ-the-Cool";
//const char* password = "2kl08ecD";
//const char* mqtt_server = "192.168.0.107"; 
const char* ssid = "ram";
const char* password = "birju1234";
const char* mqtt_server = "192.168.43.248";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[100], tmsg[50];
String payload, humid, temp;

Adafruit_ADS1115 ads1115;
float voltage = 0.0;

float soilTemp = 0.0;
char temperatureCString[6];

int numReadings = 50;
MoistureSensor ms(A0, numReadings);

const int DELAY = 1000;

void setup() {
  //pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  //client.setCallback(callback);
  dht.begin();
  pinMode(30, OUTPUT);
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);  
  delayMS = sensor.min_delay / 1000;
  ads1115.begin();
  DS18B20sensor.begin();
  sensors.begin();
}

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {

    digitalWrite(30, HIGH);
    delay(1000);

    for (int i = 0; i < numReadings; i++) {
    ms.reset();
    ms.read();
    delay(1);
  }
 
  
  
  if (!client.connected()) {
    reconnect();
  }
 // client.loop();
  int16_t adc0, adc1, adc2, adc3;
  adc0 = ads1115.readADC_SingleEnded(0);
  Serial.print("AIN0: "); Serial.println(adc0);
  Serial.print("voltage = "); Serial.println(voltage);
  voltage = (adc0 * 0.1875)/1000;
  
  sensors.requestTemperatures();
  soilTemp=sensors.getTempCByIndex(0);
  //soilTemp = DS18B20sensor.getTempCByIndex(0);
  dtostrf(soilTemp, 2, 2, temperatureCString);
  Serial.print("Soil Temperature is: ");
  Serial.print(soilTemp);
  Serial.println(sensors.getTempCByIndex(0));
  delay(1000);
  
  // Get temperature event and print its value.
  sensors_event_t event;  
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println("Error reading temperature!");
  }
  else {
    Serial.print("Temperature: ");
    temp = String(event.temperature);
    Serial.print(temp);
    Serial.println(" *C");
    payload = "{\"temperature\":"+temp+"}";
    payload.toCharArray(tmsg, payload.length()+1);
    client.publish("topic1", tmsg);
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println("Error reading humidity!");
  }
  else {
    Serial.print("Humidity: ");
    humid = String(event.relative_humidity);
    Serial.print(humid);
    Serial.println("%");
    payload = "{\"AirHumidity\":"+humid+","+"\"SoilMoisture\":"+ms.getSmoothedReading()+","+"\"SoilTemperature\":"+temperatureCString+"}";
    payload.toCharArray(msg, payload.length()+1);
    
    client.publish("mypub", msg);
    digitalWrite(30, LOW);
      delay(DELAY);

  }
 }
