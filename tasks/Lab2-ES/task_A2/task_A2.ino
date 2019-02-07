#include <Arduino_FreeRTOS.h>

int led = 3;
int led2 = 2;

void task1(void *param) {
  const char *out = "Task1 is running!\n";
  uint32_t i;
  for (;;) {
    Serial.print(out);
    digitalWrite(led2, LOW);
    digitalWrite(led, HIGH);
    vTaskDelay(pdMS_TO_TICKS(3000));
    digitalWrite(led, LOW);
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void task2(void *param) {
  const char *out = "Task2 is running!\n";
  uint32_t i;
  for (;;) {
    Serial.print(out);
    digitalWrite(led, LOW);
    digitalWrite(led2, HIGH);
    vTaskDelay(pdMS_TO_TICKS(3000));
    digitalWrite(led2, LOW);
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  pinMode(led2, OUTPUT);

  xTaskCreate(task1, "T1", 256, NULL, 0, NULL);
  xTaskCreate(task2, "T2", 256, NULL, 1, NULL);

}

void loop() {

}
