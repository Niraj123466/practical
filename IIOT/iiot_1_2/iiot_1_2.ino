#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Samsung";
const char* password = "12345678";
const char* mqttServer = "test.mosquitto.org";
const uint16_t mqttPort = 1883;
const char* mqttClientId = "ESP32PIR";

#define PIRPIN 4

WiFiClient espClient;
PubSubClient client(espClient);

int lastPirState = LOW;
unsigned long pirWarmupMillis = 0;
const unsigned long PIR_WARMUP_TIME = 30000UL;

void setup() {
  Serial.begin(115200);
  pinMode(PIRPIN, INPUT);
  lastPirState = digitalRead(PIRPIN);

  WiFi.begin(ssid, password);
  Serial.print("Connecting WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");

  client.setServer(mqttServer, mqttPort);

  pirWarmupMillis = millis();
  Serial.println("PIR warming up (allow ~30s)...");
}

void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("MQTT connect...");
    if (client.connect(mqttClientId)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

void publishMotion(int motionState) {
  String payload = String("{\"motion\":") + motionState + "}";
  client.publish("IIOT/motion", payload.c_str());
  Serial.println("Published: " + payload);
}

void loop() {
  if (!client.connected()) {
    reconnectMQTT();
  }
  client.loop();

  if (millis() - pirWarmupMillis < PIR_WARMUP_TIME) {
    delay(200);
    return;
  }

  int pirState = digitalRead(PIRPIN);

  if (pirState != lastPirState) {
    lastPirState = pirState;
    publishMotion(pirState == HIGH ? 1 : 0);
  }

  delay(200);
}



//sudo ./mosquitto.sub -h test.mosquitto.org -p 1883 -t "IIOT/motion" -v
//mosquitto_sub -h test.mosquitto.org -p 1883 -t "IIOT/motion" -v
//mosquitto_sub.exe -h test.mosquitto.org -p 1883 -t "IIOT/motion" -v
//mosquitto_sub.exe -h broker.hivemq.com -p 1883 -t "IIOT/motion" -v

