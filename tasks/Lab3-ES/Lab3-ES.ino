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

// Motor settings
int speed = 130;
volatile int drive = 0;

// Right Motor
int motorRight = 5;
int motorRightFwd = 4;
int motorRightBwd = 2;

// Left Motor
int motorLeft = 6;
int motorLeftFwd = 7;
int motorLeftBwd = 8;

// Distance Sensor
int sensorTrig = 13;
int sensorEcho = 12;
unsigned int sensorDelay = 10;
int sensorFlag = 0;

// IR Remote
int irPin = 10;
IRrecv irReceiver(irPin);
decode_results irResults;

// General delay variables
int taskDelay = 50;

void driveTask(void *param)
{
  for (;;)
  {

    switch (drive)
    {
    case 0:
      analogWrite(motorRight, 0);
      analogWrite(motorLeft, 0);
      break;
    case POWER: // POWER
      if (speed > 0)
      {
        speed = 0;
        analogWrite(motorRight, speed);
        analogWrite(motorLeft, speed);
      }
      else
      {
        speed = 150;
      }
      break;
    case A: // A
      // Serial.println("33.33% power!");
      speed = 130;
      break;
    case B: // B
      // Serial.println("66.66% power!!");
      speed = 170;
      break;
    case C: // C
      // Serial.println("100% poweeer!!!!!");
      speed = 255;
      break;
    case UP: // UP
      if (!sensorFlag)
      {
        Serial.println("UP");
        analogWrite(motorRight, speed);
        digitalWrite(motorRightFwd, HIGH);
        digitalWrite(motorRightBwd, LOW);

        analogWrite(motorLeft, speed);
        digitalWrite(motorLeftFwd, HIGH);
        digitalWrite(motorLeftBwd, LOW);
        break;
      }
      else
      {
        Serial.println("DONT GO FORWARD MAN1");
        digitalWrite(motorRightFwd, LOW);
        digitalWrite(motorLeftFwd, LOW);
        break;
      }

    case DOWN:
      Serial.println("DOWN");
      analogWrite(motorRight, speed);
      digitalWrite(motorRightFwd, LOW);
      digitalWrite(motorRightBwd, HIGH);

      analogWrite(motorLeft, speed);
      digitalWrite(motorLeftFwd, LOW);
      digitalWrite(motorLeftBwd, HIGH);
      break;
    case LEFT:
      // Serial.println("LEFT");
      analogWrite(motorRight, speed - 20);
      digitalWrite(motorRightFwd, HIGH);
      digitalWrite(motorRightBwd, LOW);

      analogWrite(motorLeft, speed - 70);
      digitalWrite(motorLeftFwd, LOW);
      digitalWrite(motorLeftBwd, HIGH);
      break;
    case RIGHT:
      // Serial.println("RIGHT");
      analogWrite(motorRight, speed - 70);
      digitalWrite(motorRightFwd, LOW);
      digitalWrite(motorRightBwd, HIGH);

      analogWrite(motorLeft, speed - 20);
      digitalWrite(motorLeftFwd, HIGH);
      digitalWrite(motorLeftBwd, LOW);
      break;
    case SELECT:
      // Serial.println("SELECT");
      // Do something cool, then use "delay()" to busy wait while the cool stuff is happening
      //
      analogWrite(motorRight, 200);
      digitalWrite(motorRightFwd, LOW);
      digitalWrite(motorRightBwd, HIGH);
      analogWrite(motorLeft, 200);
      digitalWrite(motorLeftFwd, HIGH);
      digitalWrite(motorLeftBwd, LOW);
      delay(100);
      analogWrite(motorRight, 200);
      digitalWrite(motorRightFwd, HIGH);
      digitalWrite(motorRightBwd, LOW);
      analogWrite(motorLeft, 200);
      digitalWrite(motorLeftFwd, LOW);
      digitalWrite(motorLeftBwd, HIGH);
      delay(100);
    }
    vTaskDelay(pdMS_TO_TICKS(20));
  }
}

void sensorTask(void *param)
{
  int echoRead;
  float distanceToObject;
  for (;;)
  {
    // Send pulse to sensorTrig pin
    digitalWrite(sensorTrig, HIGH);
    delayMicroseconds(sensorDelay);
    digitalWrite(sensorTrig, LOW);

    echoRead = pulseIn(sensorEcho, HIGH);
    distanceToObject = (echoRead / 34.3) / 2;
    Serial.println(distanceToObject);
    switch (speed)
    {
    case 130:
      if (distanceToObject < 20)
      {
        sensorFlag = 1;
        // Serial.println("130 speed -> dTO < 20!");
      }
      else
      {
        sensorFlag = 0;
      }
      break;
    case 170:
      if (distanceToObject < 25)
      {
        sensorFlag = 1;
        Serial.println("170 speed -> dTO < 25!");
      }
      else
      {
        sensorFlag = 0;
      }
      break;
    case 255:
      if (distanceToObject < 35)
      {
        sensorFlag = 1;
        Serial.println("255 speed -> dTO < 35!");
      }
      else
      {
        sensorFlag = 0;
      }
      break;
    }
    vTaskDelay(pdMS_TO_TICKS(taskDelay));
  }
}

void irTask(void *param)
{
  for (;;)
  {
    if (irReceiver.decode(&irResults) == 1)
    {
      if (irResults.value == 0XFFFFFFFF)
      {
        // Do nothing just continue
      }
      else
      {
        drive = irResults.value;
      }
      irReceiver.resume();
    }
    else
    {
      drive = 0;
    }
    vTaskDelay(pdMS_TO_TICKS(150));
  }
}

// void ISR1(void) {
//   analogWrite(motorRight, 0);
//   digitalWrite(motorRightFwd, LOW);
//   digitalWrite(motorRightBwd, LOW);

//   analogWrite(motorLeft, 0);
//   digitalWrite(motorLeftFwd, LOW);
//   digitalWrite(motorLeftBwd, LOW);
// }

void setup()
{
  Serial.begin(9600);

  // Distance sensor interrupt
  // attachInterrupt(digitalPinToInterrupt(intrPin), ISR1, RISING);

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

  xTaskCreate(sensorTask, (const portCHAR *)"sensorTask", 128, NULL, 2, NULL);
  xTaskCreate(irTask, (const portCHAR *)"irTask", 128, NULL, 0, NULL);
  xTaskCreate(driveTask, (const portCHAR *)"driveTask", 128, NULL, 1, NULL);
}

void loop()
{
}
