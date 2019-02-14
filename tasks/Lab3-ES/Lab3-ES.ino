#include <Arduino_FreeRTOS.h>
#include <IRremote.h>


#define POWER 0x00FF629D
#define A 0x00FF22DD
#define B 0x00FF02FD
#define C 0x00FFC23D
#define UP 0x00FF9867
#define DOWN 0x00FF38C7
#define LEFT 0x00FF30CF
#define RIGHT 0x00FF7A85
#define SELECT 0x00FF18E7

// Right Motor
int motorRight = 6;
int motorRightFwd = 7;
int motorRightBwd = 8;

// Left Motor
int motorLeft = 5;
int motorLeftFwd = 4;
int motorLeftBwd = 2;

// Distanvce Sensor
int trig = 13;
int echo = 12;
unsigned int sensorDelay = 10;
int echoRead;

void sensorTask(void *param) {
  for(;;) {
    // Send pulse to Trig pin
    digitalWrite(trig, HIGH);
    delayMicroseconds(sensorDelay);
    digitalWrite(trig, LOW);

    echoRead = pulseIn(echo, HIGH);
    Serial.print("Echo read: ");
    Serial.println(echoRead);
  }
}

void setup() {
  Serial.begin(9600);
  
  // Motors
  pinMode(motorRight, OUTPUT);
  pinMode(motorRightFwd, OUTPUT);
  pinMode(motorRightBwd, OUTPUT);
  pinMode(motorLeft, OUTPUT);
  pinMode(motorLeftFwd, OUTPUT);
  pinMode(motorLeftBwd, OUTPUT);

  // Sensor
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  xTaskCreate(sensorTask, (const portCHAR *)"sensorTask", 128, NULL, 1, NULL);

}

void loop() {
  analogWrite(motorRight, 100);
  digitalWrite(motorRightFwd, HIGH);
  digitalWrite(motorRightBwd, LOW);

  analogWrite(motorLeft, 100);
  digitalWrite(motorLeftFwd, HIGH);
  digitalWrite(motorLeftBwd, LOW);

}
