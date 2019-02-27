#include <Arduino_FreeRTOS.h>
#include <IRremote.h>
#include <semphr.h>


#define POWER 0x00FF629D
#define A 0x00FF22DD
#define B 0x00FF02FD
#define C 0x00FFC23D
#define UP 0x00FF9867
#define DOWN 0x00FF38C7
#define LEFT 0x00FF30CF
#define RIGHT 0x00FF7A85
#define SELECT 0x00FF18E7

// Semaphores
SemaphoreHandle_t mtx = NULL;

// Right Motor
int motorRight = 5;
int motorRightFwd = 4;
int motorRightBwd = 2;
int drive;

// Left Motor
int motorLeft = 6;
int motorLeftFwd = 7;
int motorLeftBwd = 8;

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

void driveTask(void *param) {
  for(;;) {
    xSemaphoreTake(mtx, 0);
    int driveSetting = drive;

    switch(irResults.value){
      case 0:
        Serial.println("IDLE");
        analogWrite(motorRight, 0);
        analogWrite(motorLeft, 0);
        break;
      case POWER: // POWER
        Serial.println("POWER");
        break;
      case A: // A
        Serial.println("33.33% power!");
        break;
      case B: // B
        Serial.println("66.66% power!!");
        break;
      case C: // C
        Serial.println("100% poweeer!!!!!");
        break;
      case UP: // UP
        Serial.println("UP");
        analogWrite(motorRight, 100);
        digitalWrite(motorRightFwd, HIGH);
        digitalWrite(motorRightBwd, LOW);

        analogWrite(motorLeft, 100);
        digitalWrite(motorLeftFwd, HIGH);
        digitalWrite(motorLeftBwd, LOW);
        break ;  
      case DOWN:
        Serial.println("DOWN");
        analogWrite(motorRight, 100);
        digitalWrite(motorRightFwd, LOW);
        digitalWrite(motorRightBwd, HIGH);

        analogWrite(motorLeft, 100);
        digitalWrite(motorLeftFwd, LOW);
        digitalWrite(motorLeftBwd, HIGH);
        break ;               
      case LEFT:
        Serial.println("LEFT");
        analogWrite(motorRight, 100);
        digitalWrite(motorRightFwd, HIGH);
        digitalWrite(motorRightBwd, LOW);

        analogWrite(motorLeft, 0);
        digitalWrite(motorLeftFwd, LOW);
        digitalWrite(motorLeftBwd, LOW);
        break ;  
      case RIGHT:
        Serial.println("RIGHT");
        analogWrite(motorRight, 0);
        digitalWrite(motorRightFwd, LOW);
        digitalWrite(motorRightBwd, LOW);

        analogWrite(motorLeft, 100);
        digitalWrite(motorLeftFwd, HIGH);
        digitalWrite(motorLeftBwd, LOW);
        break ;  
      case SELECT:
      Serial.println("SELECT");
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
      if (irResults.value == 0XFFFFFFFF)
        irResults.value = drive;

      drive = irResults.value;
      irReceiver.resume(); 
    }
    else {
      drive = 0;
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

  // Mutex initialiser
  mtx = xSemaphoreCreateMutex();

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

  if (mtx != NULL) {
    xTaskCreate(sensorTask, (const portCHAR *)"sensorTask", 128, NULL, 0, NULL);
    xTaskCreate(irTask, (const portCHAR *)"irTask", 128, NULL, 0, NULL);
    xTaskCreate(driveTask, (const portCHAR *)"driveTask", 128, NULL, 0, NULL);
  } 

}

void loop() {
  // analogWrite(motorRight, 100);
  // digitalWrite(motorRightFwd, HIGH);
  // digitalWrite(motorRightBwd, LOW);

  // analogWrite(motorLeft, 100);
  // digitalWrite(motorLeftFwd, HIGH);
  // digitalWrite(motorLeftBwd, LOW);

}
