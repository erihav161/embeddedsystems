#include <Arduino_FreeRTOS.h>
#include <semphr.h>

int led1 = 5;
int led2 = 6;

volatile float sharedIn = 0;
SemaphoreHandle_t mtx = NULL;

void consumer1(void *param) {
  const char *out = "Consumer 1 is running!\n";
  Serial.print(out);
  for(;;) {
    xSemaphoreTake(mtx, 0);
    float taskOneIn = sharedIn;
    int runTask = (int)taskOneIn;
    if(runTask == 1) {
      int brightness = (taskOneIn-1)*255;
      Serial.print("Brightness: ");
      Serial.println(brightness);
      analogWrite(led1, brightness);
    }
    xSemaphoreGive(mtx);
    vTaskDelay(pdMS_TO_TICKS(50));
  }
}

void consumer2(void *param) {
  const char *out = "Consumer 2 is running!\n";
  Serial.print(out);
  for(;;) {
    xSemaphoreTake(mtx, 0);
    float taskTwoIn = sharedIn;
    int runTask = (int)taskTwoIn;
    if(runTask == 2) {
      int brightness = (taskTwoIn-2)*255;
      Serial.print("Brightness: ");
      Serial.println(brightness);
      analogWrite(led2, brightness);
    }
    xSemaphoreGive(mtx);
    vTaskDelay(pdMS_TO_TICKS(50));
  }
}

void producer(void *param) {
  const char *out = "Choose a floating number between 1.0 and 3.0: \n";
  Serial.print(out);
  for(;;){
    while (Serial.available() > 0) {
      sharedIn = Serial.parseFloat();
      Serial.read();
      Serial.print("Producer: ");
      Serial.println(sharedIn);
    }
  
    vTaskDelay(pdMS_TO_TICKS(50));
  }

}

void setup() {
  Serial.begin(9600);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

  mtx = xSemaphoreCreateMutex();
  if(mtx != NULL) {
    xTaskCreate(consumer1, (const portCHAR *)"consumer1", 128, NULL, 1, NULL);
    xTaskCreate(consumer2, (const portCHAR *)"consumer2", 128, NULL, 1, NULL);
    xTaskCreate(producer, (const portCHAR *)"producer", 128, NULL, 0, NULL); 
  }
}

void loop() {

}
