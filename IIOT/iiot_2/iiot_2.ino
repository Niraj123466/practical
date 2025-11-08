#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT11
#define LED_PIN 2

DHT dht(DHTPIN, DHTTYPE);

float TEMP_THRESHOLD = 32.0;

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  Serial.println("DHT11 Temperature Monitoring System Initialized");
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Error: DHT11 sensor not responding");
    delay(2000);
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C | Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  if (temperature > TEMP_THRESHOLD) {
    Serial.println("Temperature high → LED blinking");
    digitalWrite(LED_PIN, HIGH);
    delay(500);
    digitalWrite(LED_PIN, LOW);
    delay(500);
  } 
  else {
    digitalWrite(LED_PIN, LOW);
  }

  delay(1500);
}
