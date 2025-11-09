#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Samsung";
const char* password = "12345678";

const char* mqtt_server = "test.mosquitto.org";

WiFiClient espClient;
PubSubClient client(espClient);

int pirPin = 13;
int pirState = LOW;
int val = 0;

void setup_wifi() {
  delay(10);
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
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32ClientPIR")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  pinMode(pirPin, INPUT);
  Serial.begin(115200);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  val = digitalRead(pirPin);

  if (val == HIGH) {
    if (pirState == LOW) {
      Serial.println("Motion detected");
      client.publish("IIoT/motion", "Motion Detected");
      pirState = HIGH;
    }
  } else {
    if (pirState == HIGH) {
      Serial.println("Motion ended");
      client.publish("IIoT/motion", "No Motion");
      pirState = LOW;
    }
  }

  delay(100);
}



//sudo ./mosquitto.sub -h test.mosquitto.org -p 1883 -t "IIOT/motion" -v
//mosquitto_sub -h test.mosquitto.org -p 1883 -t "IIOT/motion" -v
//mosquitto_sub.exe -h test.mosquitto.org -p 1883 -t "IIOT/motion" -v
//mosquitto_sub.exe -h broker.hivemq.com -p 1883 -t "IIOT/motion" -v
