#include <arduino.h>

int sensorPin = A0, sensorVal;
double voltage, UVindex, UVAPower;

while(1)
{
  sensorVal = analogRead(sensorPin);
  printf("Sensor Value: %d", sensorVal);
  voltage = sensorVal / 1024.0 * 5.0;
  printf(", Voltage: %.2lf", voltage);
//PhotoCurrent(uA) = voltage / 4.3
//PhotoCurrent(uA) = 13/600 * UVI + 0.075
//PhotoCurrent(uA) = 1/9 * UVAP(mW/cm^2)
  UVindex = (voltage / 4.3 - 0.075) * 600 / 13;
  UVAPower = voltage / 4.3 * 9;
  printf(", UVindex: %.2lf", UVindex);
  printf(", UVAPower: %.2lf mW/cm^2.\n", UVAPower);
  delaySeconds(1);
}
