int led = 3;
int brightness = 0;
int fadeAmount = 5;

void setup() {
  //settting baud speed (serial monitor speed)
  Serial.begin(9600);
  
  //setting pin 3 as output pin
  pinMode(led, OUTPUT);
}

void loop() {
  //set brightness to pin 3
  analogWrite(led, brightness);

  // change the brightness for the next through loop
  brightness = brightness + fadeAmount;

  // reverse the direction of the fading at the ends of the fade
  if(brightness <= 0 || brightness >= 255) {
    fadeAmount = -fadeAmount;
  }
  //wait 30 ms to see the fading
  delay(30);
}
