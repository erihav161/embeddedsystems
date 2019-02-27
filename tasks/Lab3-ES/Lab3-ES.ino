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
int drive;

// Left Motor
int motorLeft = 5;
int motorLeftFwd = 4;
int motorLeftBwd = 2;

// Distance Sensor
int sensorTrig = 13;
int sensorEcho = 12;
unsigned int sensorDelay = 10;
int echoRead;
float distanceToObject;

// IR Remote
int irPin = 10;
IRrecv irReceiver(irPin);
decode_results irResults;

void driveForward(void *param) {
  for(;;) {
    // Serial.println("DriveForward");
    if (drive == 123) {
      analogWrite(motorRight, 100);
      digitalWrite(motorRightFwd, HIGH);
      digitalWrite(motorRightBwd, LOW);

      analogWrite(motorLeft, 100);
      digitalWrite(motorLeftFwd, HIGH);
      digitalWrite(motorLeftBwd, LOW);
    }
  }
}

void sensorTask(void *param) {
  for(;;) {
    // Send pulse to sensorTrig pin
    digitalWrite(sensorTrig, HIGH);
    delayMicroseconds(sensorDelay);
    digitalWrite(sensorTrig, LOW);

    echoRead = pulseIn(sensorEcho, HIGH);
    distanceToObject = echoRead/34.30;

    // Serial.print("Distance to object: ");
    // Serial.println(distanceToObject);
  }
}

void irTask(void *param) {
  for(;;) {
    if (irReceiver.decode(&irResults)) {
      drive = irResults.value;
      Serial.println(drive,HEX);

      irReceiver.resume(); // Receive the next value
    }
  }
}

void ISR1(void) {
  analogWrite(motorRight, 0);
  digitalWrite(motorRightFwd, LOW);
  digitalWrite(motorRightBwd, LOW);

  analogWrite(motorLeft, 0);
  digitalWrite(motorLeftFwd, LOW);
  digitalWrite(motorLeftBwd, LOW);
}
void setup() {
  Serial.begin(9600);

  // Distance sensor interrupt
  //attachInterrupt(digitalPinToInterrupt(intrPin), ISR1, RISING);

  // Motors
  pinMode(motorRight, OUTPUT);
  pinMode(motorRightFwd, OUTPUT);
  pinMode(motorRightBwd, OUTPUT);
  pinMode(motorLeft, OUTPUT);
  pinMode(motorLeftFwd, OUTPUT);
  pinMode(motorLeftBwd, OUTPUT);

  // Sensor
  pinMode(sensorTrig, OUTPUT);
  pinMode(sensorEcho, INPUT);

  // IR Receiver
  pinMode(irPin, INPUT);
  irReceiver.enableIRIn();

  xTaskCreate(sensorTask, (const portCHAR *)"sensorTask", 128, NULL, 0, NULL);
  xTaskCreate(irTask, (const portCHAR *)"irTask", 128, NULL, 0, NULL);
  xTaskCreate(driveForward, (const portCHAR *)"driveForward", 128, NULL, 0, NULL);
}

void loop() {
  analogWrite(motorRight, 100);
  digitalWrite(motorRightFwd, HIGH);
  digitalWrite(motorRightBwd, LOW);

  analogWrite(motorLeft, 100);
  digitalWrite(motorLeftFwd, HIGH);
  digitalWrite(motorLeftBwd, LOW);

}
