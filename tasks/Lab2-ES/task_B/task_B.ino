#include <Arduino_FreeRTOS.h>

int led1 = 5;
int led2 = 6;
float in;

void task1(void *param) {
  const char *out = "Task1 is running!\n";
  if (in > 0 && in <= 25) {
    analogWrite(led1, 64);
  }
  if (in >= 26 && in <= 50) {
    analogWrite(led1, 128);
  }
  if (in >= 51 && in <= 75) {
    analogWrite(led1, 192);
  }
  if (in >= 76 && in <= 100) {
    analogWrite(led1, 255);
  }
}

void task2(void *param) {
  const char *out = "Task2 is running!\n";
  if (in > 0 && in <= 25) {
    analogWrite(led2, 64);
  }
  if (in >= 26 && in <= 50) {
    analogWrite(led2, 128);
  }
  if (in >= 51 && in <= 75) {
    analogWrite(led2, 192);
  }
  if (in >= 76 && in <= 100) {
    analogWrite(led2, 255);
  }

}

void task3(void *param) {
  const char *out = "Choose a floating number between 1.0 and 3.0: \n";
  String inString = "";

  for(;;) {
    Serial.print(out);
    int inChar = Serial.read();
    if (inChar != '\n') {
      inString += (char)inChar;
      Serial.print(inString);
    }
    else {
      float in = inString.toFloat();
      inString = "";
    }
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

  xTaskCreate(task1, "T1", 256, NULL, 0, NULL);
  xTaskCreate(task2, "T2", 256, NULL, 0, NULL);
  xTaskCreate(task3, "T3", 256, NULL, 0, NULL);

}

void loop() {

}
