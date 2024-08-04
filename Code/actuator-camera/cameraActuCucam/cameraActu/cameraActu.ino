#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif 

#define RED_PIN 16 // D0
#define GREEN_PIN 5 // D1
#define BLUE_PIN 4 // D2

#include <PubSubClient.h>

const char* ssid = "HONOR 9X"; // Change this to your Wifi SSID
const char* password = "Mohsen@2003"; // Change this to your Wifi Password
const char* mqtt_server = "broker.hivemq.com"; // Mosquitto Server URL

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() { 
    delay(10);
    Serial.println();
    Serial.print("Connecting to ");
    Serial.print(ssid);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) { 
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

void handleCucumberIrrigationPercent(char* msg) {
    if (strcmp(msg, "0") == 0) { 
        Serial.println("Soil Moisture is 0%");
        analogWrite(RED_PIN, 153);
        analogWrite(GREEN_PIN, 50); 
        analogWrite(BLUE_PIN, 204); 
    } else if (strcmp(msg, "60") == 0) { 
        Serial.println("Soil Moisture is 60%");
        analogWrite(RED_PIN, 0);
        analogWrite(GREEN_PIN, 254); 
        analogWrite(BLUE_PIN, 132); 
    } else if (strcmp(msg, "70") == 0) { 
        Serial.println("Soil Moisture is 70%");
        analogWrite(RED_PIN, 0);
        analogWrite(GREEN_PIN, 0); 
        analogWrite(BLUE_PIN, 139); 
    } else if (strcmp(msg, "80") == 0) { 
        Serial.println("Soil Moisture is 80%");
        analogWrite(RED_PIN, 255);
        analogWrite(GREEN_PIN, 140); 
        analogWrite(BLUE_PIN, 0); 
    }
}

void handleCucumberIrrigation(char* msg) {
    if (strcmp(msg, "1") == 0) { 
        Serial.println("Turn on water");
        analogWrite(RED_PIN, 0);
        analogWrite(GREEN_PIN, 0); 
        analogWrite(BLUE_PIN, 255); 
    } else if (strcmp(msg, "2") == 0) { 
        Serial.println("Turn off water");
        analogWrite(RED_PIN, 255);
        analogWrite(GREEN_PIN, 0); 
        analogWrite(BLUE_PIN, 0); 
    }
}

void handleCucumberTemperatureAdjustment(char* msg) {
    int value = atoi(msg);
    if (value < 25) { 
        Serial.println("Temperature is < 25");
        analogWrite(RED_PIN, 0);
        analogWrite(GREEN_PIN, 255); 
        analogWrite(BLUE_PIN, 0); 
    } else if (value >= 25) { 
        Serial.println("Temperature is >= 25");
        analogWrite(RED_PIN, 255);
        analogWrite(GREEN_PIN, 0); 
        analogWrite(BLUE_PIN, 0); 
    }
}

void handleAirHumidityCucumberLevel(char* msg) {
    if (strcmp(msg, "1") == 0) { 
        Serial.println("Very dry");
        analogWrite(RED_PIN, 153);
        analogWrite(GREEN_PIN, 50); 
        analogWrite(BLUE_PIN, 204); 
    } else if (strcmp(msg, "2") == 0) { 
        Serial.println("Dry");
        analogWrite(RED_PIN, 0);
        analogWrite(GREEN_PIN, 254); 
        analogWrite(BLUE_PIN, 132); 
    } else if (strcmp(msg, "3") == 0) { 
        Serial.println("Moderate");
        analogWrite(RED_PIN, 0);
        analogWrite(GREEN_PIN, 0); 
        analogWrite(BLUE_PIN, 139); 
    } else if (strcmp(msg, "4") == 0) { 
        Serial.println("High");
        analogWrite(RED_PIN, 140);
        analogWrite(GREEN_PIN, 255); 
        analogWrite(BLUE_PIN, 0); 
    } else if (strcmp(msg, "5") == 0) { 
        Serial.println("Very Humid");
        analogWrite(RED_PIN, 255);
        analogWrite(GREEN_PIN, 140); 
        analogWrite(BLUE_PIN, 0); 
    }
}
void handlecucumberIlluminancePercentage(char* msg) {
    if (strcmp(msg, "1") == 0) { 
        Serial.println("0-20");
        analogWrite(RED_PIN, 100);
        analogWrite(GREEN_PIN, 50); 
        analogWrite(BLUE_PIN, 204); 
    } 
    else if (strcmp(msg, "2") == 0) { 
        Serial.println("21-40");
        analogWrite(RED_PIN, 0);
        analogWrite(GREEN_PIN, 254); 
        analogWrite(BLUE_PIN, 0); 
    } 
    else if (strcmp(msg, "3") == 0) { 
        Serial.println("41-60");
        analogWrite(RED_PIN, 0);
        analogWrite(GREEN_PIN, 0); 
        analogWrite(BLUE_PIN, 240); 
    } 
    else if (strcmp(msg, "4") == 0) { 
        Serial.println("61-80");
        analogWrite(RED_PIN, 140);
        analogWrite(GREEN_PIN, 200); 
        analogWrite(BLUE_PIN, 0); 
    } 
    else if (strcmp(msg, "5") == 0) { 
        Serial.println("81-100");
        analogWrite(RED_PIN, 200);
        analogWrite(GREEN_PIN, 140); 
        analogWrite(BLUE_PIN, 0); 
    }
}



void callback(char* topic, byte* payload, unsigned int length) { 
    char msg[length + 1]; // Create a buffer to hold the payload plus a null terminator
    strncpy(msg, (char*)payload, length); // Copy payload into msg
    msg[length] = '\0'; // Null-terminate the string

    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("]: ");
    Serial.println(msg);

    if (strcmp(topic, "CucumberIrrigationPercent") == 0) {
        handleCucumberIrrigationPercent(msg);
    } else if (strcmp(topic, "CucumberIrrigation") == 0) {
        handleCucumberIrrigation(msg);
    } else if (strcmp(topic, "CucumberTemperatureAdjustment") == 0) {
        handleCucumberTemperatureAdjustment(msg);
    } else if (strcmp(topic, "AirHumidityCucumberLevel") == 0) {
        handleAirHumidityCucumberLevel(msg);
    }
    else if (strcmp(topic, "cucumberIlluminancePercentage") == 0) {
        handlecucumberIlluminancePercentage(msg);
    }
}

void reconnect() { 
    while (!client.connected()) {
        Serial.println("Attempting MQTT connection...");

        if (client.connect("ESPClient")) {
            Serial.println("Connected");
            client.subscribe("CucumberIrrigationPercent");
            client.loop();
            client.subscribe("CucumberIrrigation");
            client.loop();
            client.subscribe("CucumberTemperatureAdjustment");
            client.loop();
            client.subscribe("AirHumidityCucumberLevel");
            client.loop();
            client.subscribe("cucumberIlluminancePercentage");
            client.loop();
        } else {
            Serial.print("Failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

void setup() {    
    Serial.begin(115200);
    setup_wifi(); 
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);
}

void loop() {
    delay(1000); // 1 second delay

    if (!client.connected()) {
        reconnect();
    }
    client.loop();
}
