#include "printf.h"

void setup() {
  Serial.begin(9600);
}

char str[100];

void loop() {
  if(Serial.available())
  {
    Serial.readBytesUntil('\n', str, sizeof(str));
    Serial.println(str);
    delay(3000);
  }
}
