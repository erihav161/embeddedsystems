#include <Arduino_FreeRTOS.h>
#include <queue.h>

//int button = 2;
int intrPin = 2;
int led1 = 5;
int led2 = 6;

float inCommand = 0;

int qLength = 5;
int qSize = sizeof(float);

QueueHandle_t myQ1;
QueueHandle_t myQ2;

void consumer1(void *param) {
  float q1Value;
  BaseType_t xStatus;

  for (;;) {
    xStatus = xQueueReceive(myQ1, &(q1Value), portMAX_DELAY);
    if (xStatus == pdPASS) {
      int brightness = (q1Value - 1) * 255;
      analogWrite(led1, brightness);
      Serial.print("Brightness led 1: ");
      Serial.println(brightness);
    } else {
      //Serial.println("NO RECIEVE Q1");
    }
    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}


void consumer2(void *param) {
  float q2Value;
  BaseType_t xStatus;

  for (;;) {
    xStatus = xQueueReceive(myQ2, &(q2Value), portMAX_DELAY);
    if (xStatus == pdPASS) {
      int brightness = (q2Value - 2) * 255;
      analogWrite(led2, brightness);
      Serial.print("Brightness led 2: ");
      Serial.println(brightness);
    } else {
      //Serial.println("NO RECIEVE Q2");
    }
    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}

void producer(void *param) {
  const char *out = "Choose a floating number between 1.0 and 3.0: \n";
  Serial.print(out);
  BaseType_t xStatus;
  for (;;) {
    while (Serial.available() > 0) {
      inCommand = Serial.parseFloat();
      Serial.read();
      int queueRun = (int)inCommand;
      if (queueRun == 1) {
        xStatus = xQueueSend(myQ1, &inCommand, 0);
        if (xStatus != pdPASS) {
          Serial.println("Queue 1 full wait a bit.");
        } else {
          Serial.print("Successfully sent to the queue ");
          Serial.println((int)inCommand);
        }
      } else if (queueRun == 2) {
        xStatus = xQueueSend(myQ2, &inCommand, 0);
        if (xStatus != pdPASS) {
          Serial.println("Queue 2 full wait a bit.");
        } else {
          Serial.print("Successfully sent to the queue ");
          Serial.println((int)inCommand);
        }
      }
    }
    vTaskDelay(pdMS_TO_TICKS(100));
  }

}

void ISR1(void) {
  xQueueReset(myQ1);
  xQueueReset(myQ2);
  Serial.println("Kön är tömd");
}

void setup() {
  Serial.begin(9600);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(intrPin, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(intrPin), ISR1, RISING);

  myQ1 = xQueueCreate(qLength, qSize);
  myQ2 = xQueueCreate(qLength, qSize);
  if (myQ1 != NULL) {
    xTaskCreate(consumer1, (const portCHAR *)"consumer1", 128, NULL, 1, NULL);
    xTaskCreate(consumer2, (const portCHAR *)"consumer2", 128, NULL, 1, NULL);
    xTaskCreate(producer, (const portCHAR *)"producer", 128, NULL, 0, NULL);
  }
}

void loop() {

}
