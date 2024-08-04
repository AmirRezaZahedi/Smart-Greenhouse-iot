// Data collection from the greenhouse environment
// cucumber
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif 

#include <PubSubClient.h>
#include "DHT.h"


#define DHT_PIN 2           // DHT11 connected to the GPIO2

#define sensorPin A0


#define DHT_TYPE   DHT11    
#define MSG_BUFFER_SIZE	(7)

DHT dht( DHT_PIN, DHT_TYPE);

char s[MSG_BUFFER_SIZE];

#define LedPin      2                          

float humidity;
float temperature;

const char* ssid = "HONOR 9X";                      // Change this to your Wifi SSID
const char* password = "Mohsen@2003";              // Change this to your Wifi Password
const char* mqtt_server = "broker.hivemq.com"; // Mosquitto Server URL


WiFiClient espClient;
PubSubClient client(espClient);


void setup_wifi()
{ 
    delay(10);
    Serial.println();
    Serial.print("Connecting to ");
    Serial.print(ssid);
    WiFi.begin(ssid, password);

    while(WiFi.status() != WL_CONNECTED) 
    { 
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) { }

void reconnect() 
{ 
  while(!client.connected()) 
  {
      Serial.println("Attempting MQTT connection...");

      if(client.connect("ESPClient")) 
      {
          Serial.println("Connected");
          client.subscribe("soilMoistureCucumber");
          client.subscribe("airHumidityCucumber");
          client.subscribe("airTemperatureCucumber");

      } 
      else 
      {
          Serial.print("Failed, rc=");
          Serial.print(client.state());
          Serial.println(" try again in 5 seconds");
          delay(5000);
      }
    }
}

void setup()
{    
    Serial.begin(115200);
    pinMode(LedPin, OUTPUT);
    setup_wifi(); 
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
}
void loop()
{
    int moisture = analogRead(sensorPin);
    Serial.println("moisture = ");
    Serial.println(moisture);



    float moisture_percent = map(moisture, 1023 , 730 , 0 , 100);
    snprintf(s , MSG_BUFFER_SIZE , "%lf" , moisture_percent);

    float humidity = dht.readHumidity();   // Read humidity
    float temperature = dht.readTemperature(); // Read temperature in Celsius

    char h[3] = {0};
    char t[3] = {0};


    h[0] = (uint8_t)humidity / 10 + '0';
    h[1] += (uint8_t)humidity % 10 + '0';
    t[0] += (uint8_t)temperature / 10 + '0';
    t[1] += (uint8_t)temperature % 10 + '0';

    /* Sending Data to Node-Red */
    client.publish("soilMoistureCucumber", s);
    Serial.println("s = ");
    Serial.println(s);
    client.publish("airHumidityCucumber", h);
    client.publish("airTemperatureCucumber", t); 


    delay(2000); 



    if(!client.connected()) { reconnect(); }
    client.loop();
}