#include <WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

const char* ssid = "Samsung";
const char* password = "12345678";
const char* mqttServer = "test.mosquitto.org";

#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("Connecting WiFi");
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  Serial.println("\nWiFi connected");
  
  client.setServer(mqttServer, 1883);
  dht.begin();
}

void loop() {
  if (!client.connected()) {
    while (!client.connected()) {
      Serial.print("MQTT connect...");
      if (client.connect("ESP32DHT")) Serial.println("connected");
      else { Serial.print("failed, rc="); Serial.println(client.state()); delay(2000); }
    }
  }
  client.loop();

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (!isnan(h) && !isnan(t)) {
    String msg = String("{\"temp\":") + t + ",\"hum\":" + h + "}";
    client.publish("IIOT/motion", msg.c_str());
    Serial.println("Published: " + msg);
  }

  delay(5000);
}


//sudo ./mosquitto.sub -h test.mosquitto.org -p 1883 -t "IIOT/motion" -v
//mosquitto_sub -h test.mosquitto.org -p 1883 -t "IIOT/motion" -v
//mosquitto_sub.exe -h test.mosquitto.org -p 1883 -t "IIOT/motion" -v
//mosquitto_sub.exe -h broker.hivemq.com -p 1883 -t "IIOT/motion" -v