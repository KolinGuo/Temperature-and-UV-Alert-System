# Temperature & UV Alert System
An **_Arduino_** group project for ENG 3 @ UC-Davis

## Group Members: 
  * Andrew Ho
  * Bobby Leanio
  * Hayaan Nishad Azeem
  * Kolin Guo

## Introduction:
The hot weather and blazing sunlight in Davis create dangers to crops in farms. Also, it's difficult to know when the farmers need to take action to protect the crops.  
Our project is designed to prevent sunburn of crops by monitoring temperature and UV level, and then alert farmers when the values get too high. 

## Design Overview
In our design, there are two separate units: the detector unit and the processing unit. 
  * The detector unit serves to collect the sensor values, convert them into comprehensible version, and transmit them to the processing unit.  
    It consists of an **_Arduino Uno_**, a radio module, a temperature sensor, and a UV sensor. 

  * The processing unit receives data from the detector unit, displays the values on the LCD panel and alerts farmers using the LEDs. Also, farmers can use the 4x4 keypad to change the alert threshold of a particular crop.  
    It consists of two **_Arduino Unos_** connected in _Serial_, a radio module, an LCD panel, a 4x4 keypad, and four LEDs. 

## Features: 
  * Support up to 4 different crops (one detector unit for a particular crop)
  * Wireless transmission up to 1 km
  * Waterproofed units
  * Easy to change threshold value
  
## Materials Used
The following table corresponds to the materials used for a single detector unit and a processing unit.  
Adding one more detector unit would increase the quantity of **_Arduino Uno_**, _NRF24L01+_ RF module, _TMP36GZ_ temperature sensor, and _GUVA-S12SD_ UV sensor by one. 

  | Name | Quantity Used |
  |:----:|:-------------:|
  | _Arduino Uno_ |3|
  | _NRF24L01+_ RF Module |2|
  | _TMP36GZ_ Temperature Sensor |1|
  | _GUVA-S12SD_ UV Sensor |1|
  | _LCM1602C_ LCD Panel |1|
  | 4x4 Membrane Keypad |1|
  | LED |4|

## Future Works
  * Alert Function
    - Implement an SMS/Email alert functionality in addition to the LED board
  * Water Settling
    - Change small flat top so that water doesn't settle there, but UV rays can still pass through
  * Size Reduction
    - Shrink the size of the detector unit to make it more compact
  * Power System
    - Power the detector unit using solar power

## References:
  * [Arduino Using NRF24L01 Rf Module](http://www.instructables.com/id/Arduino-NRF24L01-USING-RF-Module/) by sezgingul
  * [NRF24L01 real life range test](http://hallard.me/nrf24l01-real-life-range-test/) by Charles
  * [Connecting a 4 X 4 Membrane Keypad to an Arduino](http://www.instructables.com/id/Connecting-a-4-x-4-Membrane-Keypad-to-an-Arduino/) by CRCibernetica
  
## Acknowledgements:
  * Professor Jennifer Mullin
  * TAs: Erin Kizer, Kyle Chueng
  * Studio: Richard C., Joy M. Dorf
  * [UC Davis Student Farm](http://asi.ucdavis.edu/programs/sf)
  * [UC Davis C-STEM Center](http://c-stem.ucdavis.edu/)
  * [UC Davis Student Startup Center](https://startup.ucdavis.edu/)
  * [UC Davis College of Engineering](http://engineering.ucdavis.edu/)
