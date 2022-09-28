#include <LiquidCrystal.h>
#include <DS1804.h>
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const byte wiperPin = A5;
// DS1804 digipot = DS1804( 6, 5, 4, DS1804_FIFTY );
DS1804 digipot = DS1804(6, 5, 4, DS1804_TEN);

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
  Serial.begin(115200);
  digipot.setToZero();
  // potTest();
  lcd.begin(16, 2);
  lcd.print("initArd");
  lcd.setCursor(0, 1);
  lcd.print("");
}

void loop()
{
  while (Serial.available() == 0)
  {

    digitalWrite(LED_BUILTIN, HIGH); // turn the LED on (HIGH is the voltage level)
    delay(500 / freq);               // wait for a second
    digitalWrite(LED_BUILTIN, LOW);  // turn the LED off by making the voltage LOW
    delay(500 / freq);               // wait for a second
  }

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

      new_resistance = inString.toFloat();
      if (inString.toInt() > 0 && inString.toInt() < 50)
      {
        freq = inString.toInt();
      }
      digipot.setResistance(new_resistance);
      delay(100); // give DS1804 time to change, and for analogue reading to settle
      reading = analogRead(wiperPin);
      String wiperPositionText = "DS1804 Wiper at ";
      Serial.println(wiperPositionText + digipot.getWiperPosition());
      String resistanceText = "DS1804 Resistance set to ";
      Serial.println(resistanceText + digipot.getResistance() + " ohms (requested " + new_resistance + " ohms)");
      String wiperPinVoltageText = "Wiper pin voltage: ";
      Serial.println(wiperPinVoltageText + 4.9 * reading + " mV");
      inString = Serial.readString();
      Serial.println(inString);
    }
  }
}

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