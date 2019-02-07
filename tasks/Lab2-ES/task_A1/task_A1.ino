#include <Arduino_FreeRTOS.h>

int led = 3;

void task1(void *param) {
  const char *out = "Task1 is running!\n";
  uint32_t i;
  for (;;) {
    Serial.print(out);
    digitalWrite(led, HIGH);
    delay(3000); 
    digitalWrite(led, LOW);
    delay(1000);
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);

  xTaskCreate(task1, "T1", 256, NULL, 0, NULL);
  

}

void loop() {

}
