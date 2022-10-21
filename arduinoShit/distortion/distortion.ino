#include <DS1804.h>
// #include <LiquidCrystal.h>
#include <EEPROM.h>
DS1804 pot1 = DS1804(6, 5, 4, DS1804_FIFTY);
DS1804 pot2 = DS1804(9, 8, 7, DS1804_FIFTY);
DS1804 pot3 = DS1804(12, 11, 10, DS1804_FIFTY);
const byte wiperPin = A5;

// screen setup
// const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
// LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Current preset
int preset = 1;
// Current Preset Variables from 0 to 100
int pot1Val = 0;
int pot2Val = 0;
int pot3Val = 0;
bool superDistortion = false;


// EEProm address definitions
int AddrPre1pot1Val = 0;
int AddrPre2pot1Val = 1;
int AddrPre3pot1Val = 2;

int AddrPre1pot2Val = 3;
int AddrPre2pot2Val = 4;
int AddrPre3pot2Val = 5;

int AddrPre1pot3Val = 6;
int AddrPre2pot3Val = 7;
int AddrPre3pot3Val = 8;

int AddrPre1SDVal = 9;
int AddrPre2SDVal = 10;
int AddrPre3SDVal = 11;

// bitValue = ((percentageValue * 255) / 100);
const long BAUDRATE = 9600; // speed of serial connection
unsigned long reading = 0;
unsigned long new_resistance = 0;
String inString;
byte freq;
unsigned long last = millis();
String tempValues;

void setup()
{
  Serial.begin(9600);
  pot1.unlock();
  pot2.unlock();
  pot3.unlock();
  pot1Val = EEPROM.read(AddrPre1pot1Val);
  pot2Val = EEPROM.read(AddrPre1pot2Val);
  pot3Val = EEPROM.read(AddrPre1pot3Val);
  superDistortion = EEPROM.read(AddrPre1SDVal);
  // ssp.init();
  // potTest();
  delay(100);
}

void loop()
{
  String inString = "";
  while (Serial.available() > 0)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    inString = Serial.readString();
    inString = removeChar(inString.c_str(), '$');
    if (inString.length() > 0)
    {
      if (inString[0] == 'p' && inString[1] - '0' > 0 && inString[1] - '0' < 4)
      {
        // load values for current preset from eeprom
        Serial.println(inString[1]);
        preset = inString[1] - '0';
        if (preset == 1)
        {
          pot1Val = EEPROM.read(AddrPre1pot1Val);
          pot2Val = EEPROM.read(AddrPre1pot2Val);
          pot3Val = EEPROM.read(AddrPre1pot3Val);
          superDistortion = EEPROM.read(AddrPre1SDVal);
          pot1.setWiperPosition(pot1Val);
          pot2.setWiperPosition(pot2Val);
          pot3.setWiperPosition(pot3Val);
          //trigger relay for distortion
          tempValues = "q1 ";
          tempValues += String(byteToInt(pot1Val)) + " ";
          tempValues += String(byteToInt(pot2Val)) + " ";
          tempValues += String(byteToInt(pot3Val)) + " ";
          if(superDistortion){
            tempValues += "t";
          }else{
            tempValues += "f";            
          }
          tempValues += "/";
          Serial.println(tempValues);
        }
        if (preset == 2)
        {

          pot1Val = EEPROM.read(AddrPre2pot1Val);
          pot2Val = EEPROM.read(AddrPre2pot2Val);
          pot3Val = EEPROM.read(AddrPre2pot3Val);
          superDistortion = EEPROM.read(AddrPre2SDVal);
          pot1.setWiperPosition(pot1Val);
          pot2.setWiperPosition(pot2Val);
          pot3.setWiperPosition(pot3Val);
           //trigger relay for distortion
          tempValues = "q2 ";
          tempValues += String(byteToInt(pot1Val)) + " ";
          tempValues += String(byteToInt(pot2Val)) + " ";
          tempValues += String(byteToInt(pot3Val)) + " ";
          if(superDistortion){
            tempValues += "t";
          }else{
            tempValues += "f";            
          }
          tempValues += "/";
          Serial.println(tempValues);
        }
        if (preset == 3)
        {
          pot1Val = EEPROM.read(AddrPre3pot1Val);
          pot2Val = EEPROM.read(AddrPre3pot2Val);
          pot3Val = EEPROM.read(AddrPre3pot3Val);
          superDistortion = EEPROM.read(AddrPre3SDVal);
          pot1.setWiperPosition(pot1Val);
          pot2.setWiperPosition(pot2Val);
          pot3.setWiperPosition(pot3Val);
           //trigger relay for distortion
          tempValues = "q3 ";
          tempValues += String(byteToInt(pot1Val)) + " ";
          tempValues += String(byteToInt(pot2Val)) + " ";
          tempValues += String(byteToInt(pot3Val)) + " ";
          if(superDistortion){
            tempValues += "t";
          }else{
            tempValues += "f";            
          }
          tempValues += "/";
          Serial.println(tempValues);
        }
      }
      if (inString[0] == 'u' && preset == inString[1] - '0')
      {
        // update the pot to the value
        String tempString = inString.substring(3);
        pot1Val = intToByte((tempString.substring(0,inString.indexOf(' ')).toInt()));     
        tempString = tempString.substring(tempString.indexOf(' ')+1,tempString.length());
        pot2Val = intToByte((tempString.substring(0, inString.indexOf(' ')).toInt()));
        tempString = tempString.substring(tempString.indexOf(' ')+1,tempString.length());
        pot3Val = intToByte((tempString.substring(0, inString.indexOf(' ')).toInt()));
        tempString = tempString.substring(tempString.indexOf(' ')+1,tempString.length());
        
        Serial.println(tempString.charAt(tempString.indexOf(' ')+1));
        if (tempString == "t"){
          superDistortion = true;
          Serial.println("t");          
        }else{
          superDistortion=false;
          Serial.println("f");
        }
        pot1.setWiperPosition(pot1Val);
        pot1.setWiperPosition(pot2Val);
        pot1.setWiperPosition(pot3Val);
        //trigger relay
      }
      if (inString[0] == 's' && preset == inString[1] - '0')
      {
        // write current vals to eeprom
        if (preset == 1)
        {
          EEPROM.update(AddrPre1pot1Val, pot1Val);
          EEPROM.update(AddrPre1pot2Val, pot2Val);
          EEPROM.update(AddrPre1pot3Val, pot3Val);
          EEPROM.update(AddrPre1SDVal, superDistortion);
        }
        if (preset == 2)
        {
          EEPROM.update(AddrPre2pot1Val, pot1Val);
          EEPROM.update(AddrPre2pot2Val, pot2Val);
          EEPROM.update(AddrPre2pot3Val, pot3Val);
          EEPROM.update(AddrPre2SDVal, superDistortion);
        }
        if (preset == 3)
        {
          EEPROM.update(AddrPre3pot1Val, pot1Val);
          EEPROM.update(AddrPre3pot2Val, pot2Val);
          EEPROM.update(AddrPre3pot3Val, pot3Val);
          EEPROM.update(AddrPre3SDVal, superDistortion);
        }
      }
      Serial.println("@");
      // lcd.setCursor(0, 1);
    }
  }
}

// void startUpWait()
// {
//   lcd.begin(16, 2);
//   lcd.print(".");
//   lcd.setCursor(0, 1);
//   lcd.print("");
//   delay(300);
//   lcd.begin(16, 2);
//   lcd.print("..");
//   delay(300);
//   lcd.begin(16, 2);
//   lcd.print(".");
//   delay(300);
//   lcd.begin(16, 2);
//   lcd.print("...");
//   delay(100);
// }

String removeChar(char *str, char c)
{

  int i, j;

  for (i = 0, j = 0; str[i]; i++)
  {
    if (str[i] == c)
      continue;      // skip c do not copy it
    str[j] = str[i]; // shift characters left
    j++;
  }
  str[j] = 0; // terminate the string

  return String(str); // return the actual size
}

bool isValidNumber(String str)
{
  bool isNum = false;
  for (byte i = 0; i < str.length(); i++)
  {
    isNum = isDigit(str.charAt(i));
    if (!isNum)
      return false;
  }
  return isNum;
}

void potTest()
{
  Serial.println("~~ testing all positions:");
  for (int i = 0; i < 100; i++)
  {
    pot1.setWiperPosition((byte)i);
    delay(10);
    reading = analogRead(wiperPin);
    String temp = "";
    Serial.println(temp + pot1.getWiperPosition() + "," + temp + pot1.getResistance() + "ohms" + "," + temp + 4.9 * reading + "mV");
  }
  Serial.println("~~");
}
int intToByte(int byte)
{
  return ((byte * 255) / 100);
}
int byteToInt(int inter)
{
  return ((inter * 100) / 255);
}