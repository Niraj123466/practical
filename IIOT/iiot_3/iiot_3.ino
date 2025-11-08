int pirPin = 13;
int pirState = LOW;
int val = 0;

void setup() {
  Serial.begin(115200);
  pinMode(pirPin, INPUT);
}

void loop() {
  val = digitalRead(pirPin);
  
  if (val == HIGH) {
    if (pirState == LOW) {
      Serial.println("Motion detected");
      pirState = HIGH;
    }
  } else {
    if (pirState == HIGH) {
      Serial.println("Motion ended");
      pirState = LOW;
    }
  }

  delay(100);
}
