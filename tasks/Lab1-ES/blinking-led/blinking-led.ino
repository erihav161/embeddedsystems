void setup() {
  //settting baud speed (serial monitor speed)
  Serial.begin(9600);
  
  //setting pin 2 as output pin
  pinMode(2, OUTPUT);
}

void loop() {
  digitalWrite(2, 1);
  //digitalWrite(2, HIGH);
  delay(1000);
  digitalWrite(2, 0);
  //digitalWrite(2, LOW);
  delay(1000);
}
