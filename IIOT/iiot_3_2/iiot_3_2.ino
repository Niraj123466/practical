int pirPin = 13;
int pirState = LOW;
int val = 0;
int LEDPIN = 2;

void setup() {
    Serial.begin(115200);
    pinMode(pirPin, INPUT);
    pinMode(LEDPIN, OUTPUT);
    digitalWrite(LEDPIN, LOW);
}

void loop() {
    val = digitalRead(pirPin);

    if(val == HIGH) {
        if(pirState == LOW) {
            Serial.println("Motion detected");
            digitalWrite(LEDPIN, HIGH);
            delay(500);
            digitalWrite(LEDPIN, LOW);
            delay(500);
            pirState = HIGH;
        }
    }
    else {
        if(pirState == HIGH) {
            Serial.println("Motion ended");
            digitalWrite(LEDPIN, LOW);
            pirState = LOW;
        }
    }

    delay(100);
}
