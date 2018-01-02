

void setup() {
  Serial.begin(9600);
  Serial.println("Starting...");
}

char str[] = "ABCD";

void loop() {
  Serial.write(str, sizeof(str));
  Serial.println();
//  Serial.flush();
  str[3] += 1;
  delay(3000);
}
