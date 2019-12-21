#include "MQTTClient.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>


#include "DHT.h"

#define DHTPIN 14     // what digital pin the DHT22 is conected to
#define DHTTYPE DHT11   // there are multiple kinds of DHT sensors

DHT dht(DHTPIN, DHTTYPE);

//wifi
const char* ssid     = "ROY";
const char* password = "30624700";
const char* mqtt_server = "broker.hivemq.com";
const char* mqtt_user = "jaco";
const char* mqtt_password= " ";
const char* clientId="80ab6e70-f72f-45fd-a20a-8d74936be902";
 
WiFiClient WiFiclient;
MQTTClient client;
 
unsigned long lastMillis = 0;
float temperature;
int delayInterval =20;

void setup() {
  //Serial.begin(115200);
  Serial.begin(9600);
  Serial.setTimeout(2000);
  delay(10);

  // Wait for serial to initialize.
  while(!Serial) { }
  
  Serial.println();
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
 delay(100);
 
 Serial.print("connecting to MQTT broker...");
 client.begin(mqtt_server, WiFiclient);
 connect();
 
}

int timeSinceLastRead = 0;

void loop() {

  // Report every 2 seconds.
  if(timeSinceLastRead > 2000) {
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    
    temperature=getTemperature(temperature);
    

    if (!client.connected()) connect();
    client.publish("jaco/breathinglight", (String)temperature);
    Serial.println((String)temperature);
    timeSinceLastRead = 0;
  }

  timeSinceLastRead += delayInterval;
  delay(delayInterval);
  
}

float getTemperature(float lastTemperature)
{
    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    float f = dht.readTemperature(true);

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t) || isnan(f)) {
      Serial.println("Failed to read from DHT sensor!");
      timeSinceLastRead = 0;
      return (lastTemperature);
    }

    // Compute heat index in Celsius (isFahreheit = false)
    float hic = dht.computeHeatIndex(t, h, false);
    
  // Get temperature from sensor using sensor's API

    return hic;
}


void connect() {
 while (!client.connect(clientId,mqtt_user,mqtt_password)) {
   //while (!client.connect(clientId)) {
   Serial.print(".");
 }
 
 Serial.println("\nconnected!");
  client.subscribe("jaco");
}
 
