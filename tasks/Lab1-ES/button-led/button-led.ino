int b1 = 4;
int b2 = 3;
int b3 = 2;
int b1Led = 13;
int b2Led = 12;
int b3Led = 11;
int ledDelay = 100;

void setup() {
  pinMode(b1, INPUT_PULLUP);
  pinMode(b2, INPUT_PULLUP);
  pinMode(b3, INPUT_PULLUP);

  pinMode(b1Led, OUTPUT);
  pinMode(b2Led, OUTPUT);
  pinMode(b3Led, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(b1) == LOW) {
    if (digitalRead(b1Led) == HIGH) {
      digitalWrite(b1Led, LOW);
      delay(ledDelay);    
    }
    else {
      digitalWrite(b1Led, HIGH);
      delay(ledDelay);
    }
  }
  if (digitalRead(b2) == LOW) {
    if (digitalRead(b2Led) == HIGH) {
      digitalWrite(b2Led, LOW);
      delay(ledDelay);
    }
    else {
      digitalWrite(b2Led, HIGH);
      delay(ledDelay);
    }
  }
  if (digitalRead(b3) == LOW) {
    if (digitalRead(b3Led) == HIGH) {
      digitalWrite(b3Led, LOW);
      delay(ledDelay);
    }
    else {
      digitalWrite(b3Led, HIGH);
      delay(ledDelay);
    }
  }
  delay(100);
}
