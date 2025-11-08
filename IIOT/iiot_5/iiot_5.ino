#include <WiFi.h>
#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "Samsung";
const char* password = "12345678";
const char* apiKey = "GREANYTYQ2M0KG4I";

void setup() {
  Serial.begin(115200);
  dht.begin();

  WiFi.begin(ssid, password);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  Serial.println("\nWiFi Connected!");
}

void loop() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  if (isnan(t) || isnan(h)) return;

  String request = "GET /update?api_key=" + String(apiKey) +
                   "&field1=" + String(t) +
                   "&field2=" + String(h) +
                   " HTTP/1.1\r\nHost: api.thingspeak.com\r\nConnection: close\r\n\r\n";

  Serial.println(request);

  WiFiClient client;
  if (client.connect("api.thingspeak.com", 80)) {
    client.print(request);
    client.stop();
  }

  delay(20000);
}
