// Detector With Temperature and UV Sensor
// Author: Kolin Guo, Andrew Ho, Hayaan Nishad Azeem, Bobby Leanio

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"
// Change NODE for different detector
// 0 -> AA, 1 -> BB, 2 -> CC, 3 -> DD
#define NODE 1

int tmpPin = A0, UVPin = A1, sensorVal, detectInterval = 1 * 1000;
double voltage, tmp, UVIdx;

// Hardware configuration of nRF24L01: CE to pin 8, CSN to pin 7
RF24 radio(8, 7);
const uint64_t addresses[] = {0xB3B4B5B6AALL, 0xB3B4B5B6BBLL, 0xB3B4B5B6CCLL, 0xB3B4B5B6DDLL};

void setup() {
  Serial.begin(9600);
  printf_begin();

  setUpRadio();
}

void loop() {
  delay(5);
  radio.stopListening();                  // Stop listening

  // Read the current temperature
  sensorVal = analogRead(tmpPin);
  voltage = sensorVal / 1024.0 * 5.0;   // convert into voltage
  tmp = (voltage - 0.5) * 100;          // convert into temperature in Celsius

/*
 * printf("Sensor Value: %d, Voltage: ", sensorVal);
 * Serial.print(voltage);
 * printf(", ");
 */
  printf("Temperature: ");
  Serial.print(tmp);
  printf(" degree.\n\r");

  // Read the current UV level
  sensorVal = analogRead(UVPin);
  voltage = sensorVal / 1024.0 * 5.0;   // convert into voltage
  
//PhotoCurrent(uA) = voltage / 4.3
//PhotoCurrent(uA) = 13/600 * UVI + 0.075
//PhotoCurrent(uA) = 1/9 * UVAP(mW/cm^2)  
  UVIdx = (voltage / 4.3 - 0.075) * 600 / 13;   // Calculate UV Index

/*
 * printf("Sensor Value: %d, Voltage: ", sensorVal);
 * Serial.print(voltage);
 * printf(", ");
 */
  printf("UV Index: ");
  Serial.println(UVIdx);

  radio.write(&tmp, sizeof(tmp));
  radio.write(&UVIdx, sizeof(UVIdx));
  
  delay(detectInterval);
}

void setUpRadio()
{
  // Setup and configure rf radio
  radio.begin();                          // Start up the radio
  radio.setAutoAck(1);                    // Ensure autoACK is enabled
  radio.setRetries(15,15);                // Max delay between retries & number of retries
  radio.setPALevel( RF24_PA_MAX ) ;       // Max power 
  radio.setDataRate( RF24_250KBPS ) ;     // Min speed (for better range I presume)
  radio.setCRCLength( RF24_CRC_8 ) ;      // 8 bits CRC
  radio.openWritingPipe(addresses[NODE]);    // Open Pipe for writing
  radio.openReadingPipe(0,addresses[NODE]);  // Open Pipe for reading

//  radio.printDetails();                   // Dump the configuration of the rf unit for debugging
}
