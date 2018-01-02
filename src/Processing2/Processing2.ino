// Processing Unit #2
// Author: Kolin Guo, Andrew Ho, Hayaan Nishad Azeem, Bobby Leanio

#include <Keypad.h>

double tmpUpperLimit[] = {125, 125, 125, 125}, 
       UVIdxUpperLimit[] = {15, 15, 15, 15};
double tmp[4], UVIdx[4];
boolean isDetected[] = {false, false, false, false};
char buf[30];

// Setting up LEDs
const int LEDPins[] = {10, 11, 12, 13};   // A, B, C, D

// Setting up 4x4 Keypad
const byte ROWS = 4;  // 4 rows
const byte COLS = 4;  // 4 columns
const char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'.','0','#','D'}
};

const byte rowPins[ROWS] = {2, 3, 4, 5};  // row pinouts
const byte colPins[COLS] = {6, 7, 8, 9};  // column pinouts

Keypad keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  Serial.begin(9600);

  for(int i = 0; i < 4; i++)
    pinMode(LEDPins[i], OUTPUT);
    
  keypad.begin(makeKeymap(keys));
  keypad.addEventListener(keypadEvent);   // Add an event listener for this keypad
  keypad.setHoldTime(1000);
  delay(100);
}

void loop() {
  char key = keypad.getKey();
  readData();
  updateLEDs();
}

void keypadEvent(KeypadEvent key)
{
  // Take care of special events
  static bool isHolding = false;
  
  switch(keypad.getState())
  {
    case PRESSED:
      break;
    case RELEASED:
      if(!isHolding)
      {
        Serial.print("/");
        Serial.println(key);    // Send '/A' to indicate key 'A' is pressed
        Serial.flush();
      }
      else
        isHolding = false;
      break;
    case HOLD:
      if(key == 'A' || key == 'B' || key == 'C' || key == 'D' || 
         key == '#')
      {
        isHolding = true;
        Serial.print("/");
        Serial.print(key);
        Serial.println(key);    // Send '/AA' to indicate key 'A' is holded
        Serial.flush();
      }
      break;
  }
}

void readData()
{
  // Read data from Unit 1
  memset(buf, 0, sizeof(buf));
  
  if(Serial.available())
  {
    Serial.readBytesUntil('\n', buf, sizeof(buf));
    
    if(buf[0] == '#')           // if the data is updating the upper limits
    {
      int updateNode = (int)(buf[1] - 'A');
      
      if(buf[2] == 'T')         // if updating temperature
        tmpUpperLimit[updateNode] = atof(buf + 3 * sizeof(char));
      else                      // else updating UV index
        UVIdxUpperLimit[updateNode] = atof(buf + 3 * sizeof(char));

      // Flash corresponding LED to indicate the update
      digitalWrite(LEDPins[updateNode], HIGH);  
      delay(200);
      digitalWrite(LEDPins[updateNode], LOW);
    }
    else  // else normal data
    {
      Serial.println(buf);                            //FIXME: Delete
//      Serial.println(tmpUpperLimit[0]);
//      Serial.println(UVIdxUpperLimit[0]);
      
      for(int i = 0; i < 4; i++)
      {
        isDetected[i] = (buf[i] == 'T');
        
        if(isDetected[i])
        {
          Serial.readBytesUntil('\n', buf, sizeof(buf));
          tmp[i] = atof(buf);
          Serial.readBytesUntil('\n', buf, sizeof(buf));
          UVIdx[i] = atof(buf);
          
          Serial.println(tmp[i]);                     //FIXME: Delete
          Serial.println(UVIdx[i]);                   //FIXME: Delete
        }
      }
    }
  }
}

void updateLEDs()
{
  // Update the LEDs to indicate dangerous situations
  for(int i = 0; i < 4; i++)
    if(isDetected[i] && 
       (tmp[i] > tmpUpperLimit[i] || UVIdx[i] > UVIdxUpperLimit[i]))
      digitalWrite(LEDPins[i], HIGH);
    else
      digitalWrite(LEDPins[i], LOW);
}

