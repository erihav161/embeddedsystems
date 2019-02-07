// 0 64 128 192 255
int b1 = 4;
int b2 = 3;
int b3 = 2;
int b1Led = 11;
int b2Led = 10;
int b3Led = 9;
int ledDelay = 100;
int brightness1 = 0;
int brightness2 = 0;
int brightness3 = 0;
int fadeAmount = 63;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
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
    if (brightness1 < 252) {
      brightness1 = brightness1 + fadeAmount;
      analogWrite(b1Led, brightness1);
      delay(ledDelay); 
    }
    else {
      brightness1 = 0;
      analogWrite(b1Led, brightness1);
      delay(ledDelay); 
    }

  }
  if (digitalRead(b2) == LOW) {
    if (brightness2 < 252) {
      brightness2 = brightness2 + fadeAmount;
      analogWrite(b2Led, brightness2);
      delay(ledDelay); 
    }
    else {
      brightness2 = 0;
      analogWrite(b2Led, brightness2);
      delay(ledDelay); 
    }

  }
  if (digitalRead(b3) == LOW) {
    if (brightness3 < 252) {
      brightness3 = brightness3 + fadeAmount;
      analogWrite(b3Led, brightness3);
      delay(ledDelay); 
    }
    else {
      brightness3 = 0;
      analogWrite(b3Led, brightness3);
      delay(ledDelay); 
    }

  }
  delay(130);
  Serial.print(brightness1);
  Serial.print('\n');
}
