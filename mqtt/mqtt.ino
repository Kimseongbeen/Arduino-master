#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <FastLED.h>
#include "ca.h"
#define NUM_LEDS 12
#define DATA_PIN D7

/****** WiFi Connection Details *******/
const char* ssid = "SK_WiFiGIGA";
const char* password = "190572";
/******* MQTT Broker Connection Details *******/
const char* mqtt_server = "hivemq URL";
const char* mqtt_username = "hivemq 유저네임";
const char* mqtt_password = "hivemq 이름";
const char* topic = "생성 토픽// 공용 mqtt 서버가 아니라 중복 상관 X";
const int mqtt_port =8883; //포트 확인 후 입력
/**** Secure WiFi Connectivity Initialisation *****/
WiFiClientSecure espClient;

/**** MQTT Client Initialisation Using WiFi Connection *****/
PubSubClient client(espClient);
CRGB leds[NUM_LEDS];

/************* Connect to WiFi ***********/
void setup_wifi() {
  delay(10);
  Serial.print("\nConnecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("\nWiFi connected\nIP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  String incomingMessage = "";
  for (int i = 0; i < length; i++) {
    incomingMessage += (char)payload[i];
  }
  Serial.println("Message arrived [" + String(topic) + "]" + incomingMessage);
  if (length == 3) { 
    byte r = payload[0];
    byte g = payload[1];
    byte b = payload[2];
    Serial.print("r:"+String(r));Serial.print(" g:"+String(g));Serial.println(" b:"+String(b));
    leds[0] = CRGB(r,g,b);
    FastLED.show();
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client-";   // Create a random client ID
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("connected");

      client.subscribe(topic);   // subscribe the topics here

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");   // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  setup_wifi();

   #ifdef ESP8266
    espClient.setInsecure();
  #else
    espClient.setCACert(root_ca);      // enable this line and the the "certificate" code for secure connection
  #endif
  
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  delay(10);
}
