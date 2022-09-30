#include <LiquidCrystal.h>
#include <DS1804.h>
#include <SimpleSerialProtocol.h>

// void onError(uint8_t errorNum);
// void onReceivedValues();
// const long CHARACTER_TIMEOUT = 500; // wait max 500 ms between single chars to be received
// const byte COMMAND_ID_RECEIVE = '$';
// const byte COMMAND_ID_SEND = '@';
// SimpleSerialProtocol ssp(Serial, BAUDRATE, CHARACTER_TIMEOUT, onError, 'a', 'z'); // ASCII: 'a' - 'z' (26 byes of RAM is reserved)
// //https://gitlab.com/yesbotics/simple-serial-protocol/simple-serial-protocol-arduino

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const byte wiperPin = A5;
// DS1804 digipot = DS1804( 6, 5, 4, DS1804_FIFTY );
DS1804 digipot = DS1804(6, 5, 4, DS1804_TEN);

const long BAUDRATE = 9600; // speed of serial connection
int reading = 0;
unsigned long new_resistance = 0;
String inString;

byte freq;
unsigned long last = millis();

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  freq = 1;
  startUpWait();
  Serial.begin(BAUDRATE);

  // ssp.init();
  digipot.setToZero();
  // potTest();

  lcd.begin(16, 2);
  lcd.print("initArd");
  lcd.setCursor(0, 1);
  lcd.print("");
  Serial.println("Initialized");
  delay(100);
}

void loop()
{
  // delay(10);
  // // while (Serial.available() == 0)
  // // {
  // // }
  //  ssp.loop();
  if (Serial.available() > 0)
  {
    // delay(100);
    digitalWrite(LED_BUILTIN, HIGH);
    inString = Serial.readString();
    Serial.println(inString);
    inString = removeChar(inString.c_str(), '$');
    if (inString.length() > 0)
    {
      lcd.begin(16, 2);
      lcd.print(inString);
      lcd.setCursor(0, 1);
      lcd.print("");
      //     if(isValidNumber(inString)){
      //       new_resistance = inString.toFloat();
      //     if (inString.toInt() > 0 && inString.toInt() < 50)
      //     {
      //       freq = inString.toInt();
      //     }
      //     // digipot.setResistance(new_resistance);
      //     delay(100); // give DS1804 time to change, and for analogue reading to settle
      //     // reading = analogRead(wiperPin);
      //     // String wiperPositionText = "DS1804 Wiper at ";
      //     // Serial.println(wiperPositionText + digipot.getWiperPosition());
      //     // String resistanceText = "DS1804 Resistance set to ";
      //     // Serial.println(resistanceText + digipot.getResistance() + " ohms (requested " + new_resistance + " ohms)");
      //     // String wiperPinVoltageText = "Wiper pin voltage: ";
      //     // Serial.println(wiperPinVoltageText + 4.9 * reading + " mV");
      //     inString = Serial.readString();
    }
    Serial.println("@");
    lcd.setCursor(0, 1);
    // lcd.print("@fake");
    //     // Send done processing acknowledgement to rust
  }
}

// void onReceivedValues()
// {
//   const uint8_t stringBufferSize = 50;
//   String text1 = ssp.readString(stringBufferSize);
//   ssp.readEot(); // read and expect the end-of-transmission byte. important, don't forget!
//   lcd.begin(16, 2);
//   lcd.print(text1);
//   lcd.setCursor(0, 1);
//   lcd.print("");

//   //
//   // Immediately send back all received and interpreted values
//   //
//   ssp.writeCommand(COMMAND_ID_SEND); // start command with command id
//   ssp.writeString("");
//   ssp.writeEot(); // end command with end-of-transmission byte. important, don't forget!
// }

// void onError(uint8_t errorNum)
// {
//   digitalWrite(LED_BUILTIN, HIGH);
// }

void startUpWait()
{
  lcd.begin(16, 2);
  lcd.print(".");
  lcd.setCursor(0, 1);
  lcd.print("");
  delay(300);
  lcd.begin(16, 2);
  lcd.print("..");
  delay(300);
  lcd.begin(16, 2);
  lcd.print(".");
  delay(300);
  lcd.begin(16, 2);
  lcd.print("...");
  delay(100);
}
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
    digipot.setWiperPosition((byte)i);
    delay(10);
    reading = analogRead(wiperPin);
    String temp = "";
    Serial.println(temp + digipot.getWiperPosition() + "," + temp + digipot.getResistance() + "ohms" + "," + temp + 4.9 * reading + "mV");
  }
  Serial.println("~~");
}