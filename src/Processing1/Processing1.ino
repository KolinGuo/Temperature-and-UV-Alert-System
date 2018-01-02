// Processing Unit #1
// Author: Kolin Guo, Andrew Ho, Hayaan Nishad Azeem, Bobby Leanio

#include <SPI.h>
#include <LiquidCrystal.h>
#include "nRF24L01.h"
#include "RF24.h"

double tmp[4], UVIdx[4];
boolean isDetected[] = {false, false, false, false};
char buf[10];
String str = "";
byte curInputNode = 0;
boolean isShowDetail = false, isInputData = false, isInputTemp = false, isInputUV = false;
unsigned long startTime, showDetailTimeout = 5000, inputDataTimeout = 5000;
double tmpUpperLimit[] = {125, 125, 125, 125}, 
       UVIdxUpperLimit[] = {15, 15, 15, 15};

// Hardware configuration of nRF24L01: CE to pin 8, CSN to pin 7
RF24 radio(8, 7);
const uint64_t addresses[] = {0xB3B4B5B6AALL, 0xB3B4B5B6BBLL, 0xB3B4B5B6CCLL, 0xB3B4B5B6DDLL};

// Set Up LCD Panel
LiquidCrystal lcd(6, 9, 2, 3, 4, 5);  // RS, EN, D4, D5, D6, D7

void setup() {
  Serial.begin(9600);

  setUpRadio();
  setUpLCD();
  delay(100);
  updateLCD();
}

void loop() {
  readKeys();
  delay(100);
  readRadio();
}

void setUpRadio()
{
  // Setup and configure rf radio
  radio.begin();                          // Start up the radio
  radio.setAutoAck(1);                    // Ensure autoACK is enabled
  radio.setRetries(15, 15);               // Max delay between retries & number of retries
  radio.setPALevel( RF24_PA_MAX ) ;       // Max power
  radio.setDataRate( RF24_250KBPS ) ;     // Min speed (for better range I presume)
  radio.setCRCLength( RF24_CRC_8 ) ;      // 8 bits CRC
  radio.openReadingPipe(0, addresses[0]); // Open Pipe AA for reading
  radio.openReadingPipe(1, addresses[1]); // Open Pipe BB for reading
  radio.openReadingPipe(2, addresses[2]); // Open Pipe CC for reading
  radio.openReadingPipe(3, addresses[3]); // Open Pipe DD for reading

  radio.startListening();                 // Start listening
  //  radio.printDetails();                   // Dump the configuration of the rf unit for debugging
}

void setUpLCD()
{
  // Setup and configure LCD Panel
  lcd.begin(16, 2);
  lcd.print("Temperature & UV");
  lcd.setCursor(0, 1);  // column, row
  lcd.print("  Alert System");
  delay(3000);
}

void readKeys()
{
  // Read keys from Unit 2
  while(Serial.available())
  {
    while(Serial.peek() == '/')
    {
      buf[3] = '\0';  // reset the char buffer
      Serial.readBytesUntil('\n', buf, sizeof(buf));
      str = buf;
      if(str.length() == 3)   // if a key is pressed
        keyPressed(str.charAt(1));
      else if(str.length() == 4)  // if a key is held
        keyHeld(str.charAt(1));
    }
    Serial.read();
  }
}

void keyPressed(char key)
{
  /* A key is pressed on the keypad
   *  @key: the key that is pressed
   */
  switch(key)
  {
    case 'A':
      showDetailsLCD(0);
      break;
    case 'B':
      showDetailsLCD(1);
      break;
    case 'C':
      showDetailsLCD(2);
      break;
    case 'D':
      showDetailsLCD(3);
      break;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case '.':
    case '#':
      if(isInputData && isInputTemp)      // if currently is inputing temperature
        inputTempLCD(key);
      else if(isInputData && isInputUV)   // if currently is inputing UV index
        inputUVLCD(key);
      break;
  }
  
}

void keyHeld(char key)
{
  /* A key is held on the keypad
   *  @key: the key that is held
   */
  switch(key)
  {
    case 'A':
      inputDataLCD(0);
      break;
    case 'B':
      inputDataLCD(1);
      break;
    case 'C':
      inputDataLCD(2);
      break;
    case 'D':
      inputDataLCD(3);
      break;
    case '#':
      if(isInputData && isInputTemp)      // if currently is inputing temperature
        inputTempLCD('C');
      else if(isInputData && isInputUV)   // if currently is inputing UV index
        inputUVLCD('C');
      break;
  }
}

void showDetailsLCD(byte node)
{
  /* Show the details of the node on LCD panel
   *  @node: the node that is selected
   */
  lcd.clear();
  lcd.noCursor();
  lcd.setCursor(0, 0);
  lcd.print((char)('A' + node));
  lcd.print(" ");

  if(isDetected[node])    // if the Node is connected
  {
    lcd.print("TMP:");
    printTempDetail(tmp[node]);
    lcd.setCursor(0, 1);
    lcd.print("UV Index:");
    printUVIdxDetail(UVIdx[node]);
  }
  else  // else the Node is not connected
    lcd.print("Not Connected");

  isInputData = false;
  isShowDetail = true;
  startTime = millis();
}

void inputDataLCD(byte node)
{
  /* Input new data of the node on LCD panel
   *  @node: the node that needs input data
   */
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.cursor();
  lcd.print("Input Data For ");
  lcd.print((char)('A' + node));
  lcd.setCursor(0, 1);
  lcd.print(" TempC:");

  curInputNode = node;
  inputTempLCD('C');      // clear up static variables
  lcd.print(tmpUpperLimit[curInputNode]);
  lcd.setCursor(7, 1);
  isShowDetail = false;
  isInputData = true;
  isInputTemp = true;
  startTime = millis();
}

void inputTempLCD(char key)
{
  /* Input temperature value for the node
   *  @key: the pressed key
   */
  static String inputTemp = "";
  static boolean isNewTemp = false;

  if(key == '#')          // if user pressed ENTER
  {
    if(isNewTemp)         // if user did pressed any key
      updateTempUpperLimit(inputTemp.toDouble());
    
    inputTemp = "";       // clear the string buffer
    isNewTemp = false;    // reset boolean
    
    //set up to input UV
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(0, 1);
    lcd.print("UV Index:");
    inputUVLCD('C');      // clear up static variables
    lcd.print(UVIdxUpperLimit[curInputNode]);
    lcd.setCursor(9, 1);
    isInputTemp = false;
    isInputUV = true;
  }
  else if(key == 'C')     // if user pressed CLEAR
  {
    inputTemp = "";       // clear the string buffer
    isNewTemp = false;    // reset boolean
    lcd.setCursor(7, 1);
    lcd.print("                ");
    lcd.setCursor(7, 1);
  }
  else                    // else user pressed number keys
  {
    if(!isNewTemp)        // if user pressed the first key
    {
      lcd.print("                ");  // clear up LCD
      lcd.setCursor(7, 1);
      isNewTemp = true;
    }
    
    inputTemp += key;     // append to the string buffer
    lcd.print(key);
  }
  
  startTime = millis();
}

void inputUVLCD(char key)
{
  /* Input UV index value for the node
   *  @key: the pressed key
   */
  static String inputUV = "";
  static boolean isNewUV = false;

  if(key == '#')          // if user pressed ENTER
  {
    if(isNewUV)           // if user did pressed any key
      updateUVUpperLimit(inputUV.toDouble());
      
    inputUV = "";         // clear the string buffer
    isNewUV = false;    // reset boolean
    
    //set up to quit
    lcd.noCursor();
    isInputUV = false;
    isInputData = false;
    updateLCD();
  }
  else if(key == 'C')     // if user pressed CLEAR
  {
    inputUV = "";         // clear the string buffer
    isNewUV = false;    // reset boolean
    lcd.setCursor(9, 1);
    lcd.print("                ");
    lcd.setCursor(9, 1);
  }
  else                    // else user pressed number keys
  {
    if(!isNewUV)          // if user pressed the first key
    {
      lcd.print("                ");  // clear up LCD
      lcd.setCursor(9, 1);
      isNewUV = true;
    }
    
    inputUV += key;       // append to the string buffer
    lcd.print(key);
  }
  
  startTime = millis();
}

void updateTempUpperLimit(double tempUpperLimit)
{
  /* Update the upper limit of temperature to Unit 2
   *  @tempUpperLimit: the new upper limit of temperature
   */
  tmpUpperLimit[curInputNode] = tempUpperLimit; // update local variable
  Serial.print("#");                            // identifier
  Serial.print((char)('A' + curInputNode));     // updating this node
  Serial.print("T");                            // updating temperature
  Serial.println(tempUpperLimit);               // send the temperature
  Serial.flush();
  // Data Format: #BT30.6
}

void updateUVUpperLimit(double UVUpperLimit)
{
  /* Update the upper limit of UV index to Unit 2
   *  @UVUpperLimit: the new upper limit of UV index
   */
  UVIdxUpperLimit[curInputNode] = UVUpperLimit; // update local variable
  Serial.print("#");                            // identifier
  Serial.print((char)('A' + curInputNode));     // updating this node
  Serial.print("U");                            // updating UV index
  Serial.println(UVUpperLimit);                 // send the UV index
  Serial.flush();
  // Data Format: #CU3.3
}

void readRadio()
{
  // Read data from nRF24L01
  byte pipeNum = 0;                       // which pipe is sending data
  static unsigned long curTime = millis();

  while (radio.available(&pipeNum))
  {
    for (int i = 0; i < 4; i++)             // Reset indicators
      isDetected[i] = false;
    isDetected[pipeNum] = true;
    radio.read(tmp + pipeNum, sizeof(double));
    delay(10);
    radio.read(UVIdx + pipeNum, sizeof(double));
    /*
        printf("Temperature: ");
        Serial.print(tmp);
        printf(" degree.\n\r");
        printf("UV Index: ");
        Serial.println(UVIdx);
    */
    updateLCD();
    sendData();
    curTime = millis();
  }

  if(millis() - curTime > 1000)   // if too long undetected
  {
    for (int i = 0; i < 4; i++)             // Reset indicators
      isDetected[i] = false;
    updateLCD();
    curTime = millis();
  }
}

void sendData()
{
  // Send data to Unit 2
  for (int i = 0; i < 4; i++)
    printBool(isDetected[i]);
  Serial.println();
  for (int i = 0; i < 4; i++)
    if (isDetected[i])
    {
      Serial.println(tmp[i]);
      Serial.println(UVIdx[i]);
    }
  Serial.flush();
}

void printBool(boolean b)
{
  /* Print a boolean
      @b: the boolean that needs to be printed
  */
  if (b)
    Serial.write("T");
  else
    Serial.write("F");
}

void updateLCD()
{
  // Update the LCD Panel
  if(isShowDetail && (millis() - startTime) <= showDetailTimeout   // if currently is showing details
     || isInputData && (millis() - startTime) <= inputDataTimeout) // if currently is inputing data
    return;
  else
  {
    isShowDetail = false;
    isInputData = false;
  }

  lcd.clear();
  lcd.noCursor();
  
  for (int i = 0; i < 4; i++)
  {
    switch (i)
    {
      case 0:
        lcd.setCursor(0, 0);
        break;
      case 1:
        lcd.setCursor(8, 0);
        break;
      case 2:
        lcd.setCursor(0, 1);
        break;
      case 3:
        lcd.setCursor(8, 1);
        break;
    }

    lcd.print((char)('A' + i));

    if (isDetected[i])
    {
      printTemp(tmp[i]);
      lcd.print(",");
      printUVIdx(UVIdx[i]);
    }
    else
      lcd.print(" NC");
  }
  /*
    lcd.setCursor(8, 0);
    lcd.print("A ");

    if(isDetected[0])
    {
      lcd.print(tmp[0], 0);
      lcd.print(",");
      lcd.print(UVIdx[0], 1);
    }
  */
}

void printTemp(double temp)
{
  /* Print out temperature
      @temp: the temperature that needs to be printed
  */
  if (temp < -40 || temp > 125)   // Temperature Range of TMP36GZ
    lcd.print(" ND");
  else if (temp < 0 || temp > 100)
    lcd.print(temp, 0);
  else
  {
    lcd.print(" ");
    lcd.print(temp, 0);
  }
}

void printUVIdx(double UVIdx)
{
  /* Print out UV Index
      @UVIdx: the UV Index that needs to be printed
  */
  if (UVIdx < 0 || UVIdx > 15)    // Normal UV Index Range
    lcd.print("ND");
  else if (UVIdx > 10)
    lcd.print(UVIdx, 0);
  else
    lcd.print(UVIdx, 1);
}

void printTempDetail(double tempC)
{
  /* Print out temperature in detail
      @temp: the temperature that needs to be printed
  */
  if (tempC < -40 || tempC > 125)   // Temperature Range of TMP36GZ
  {
    lcd.print("No Data");
    return;
  }
  else if (tempC < 0 || tempC > 100)
    lcd.print(tempC, 0);
  else
    lcd.print(tempC, 1);
  lcd.print("C");
  
  double tempF = Cels2Fahr(tempC);
  
  if (tempF < 0 || tempF > 100)
    lcd.print(tempF, 0);
  else
    lcd.print(tempF, 1);
  lcd.print("F");
}

void printUVIdxDetail(double UVIdx)
{
  /* Print out UV Index in detail
      @UVIdx: the UV Index that needs to be printed
  */
  if (UVIdx < 0 || UVIdx > 15)    // Normal UV Index Range
    lcd.print("No Data");
  else
    lcd.print(UVIdx, 3);
}

double Cels2Fahr(double celsius)
{
  /* Convert a temperature value from Celsius to Fahrenheit
   *  @celsius: the temperature in Celsius
   *  @return: the temperature in Fahrenheit
   */
  return celsius * 1.8 + 32.0;
}

