#include <WiFi.h>
#include <PubSubClient.h>
// local 서버 일대

// WiFi 설정
const char* ssid = "mecha1203";
const char* password = "mecha1203";

// MQTT 설정
const char* mqtt_server = "192.168.0.63";
const char* inTopic = "/i2r/inTopic";
const char* outTopic = "/i2r/outTopic";
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to WiFi");

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  while (!client.connected()) {
    if (client.connect("esp32_client")) {
      Serial.println("Connected to MQTT Broker");
      client.subscribe(inTopic);
    }
    else {
      Serial.print("Failed to connect to MQTT Broker with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void loop() {
  client.loop();

  // 시리얼에서 입력을 받아 MQTT 메시지로 전송
  if (Serial.available() > 0) {
    String message = Serial.readStringUntil('\n');
    client.publish(outTopic, message.c_str());
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.printf("Received %d bytes from topic '%s': ", length, topic);
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}
