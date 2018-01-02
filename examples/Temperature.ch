#include <arduino.h>

int sensorPin = A1, sensorVal;
double voltage, tmp;

while(1)
{
    sensorVal = analogRead(sensorPin);
    printf("Sensor Value: %d", sensorVal);
    voltage = sensorVal / 1024.0 * 5.0;
    printf(", Voltage: %.2lf", voltage);
    tmp = (voltage - 0.5) * 100;
    printf("Temperature: %.2lf degree.\n", tmp);

}
