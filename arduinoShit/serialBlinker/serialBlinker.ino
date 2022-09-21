#include <EEPROM.h>

byte freq;
String inString;
unsigned long last = millis();
void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
    // start serial port at 9600 bps:
    Serial.begin(9600);
    while (!Serial)
    {
        ;
    }
    freq = 15;
}

void loop()
{
    while (Serial.available() == 0)
    {
        digitalWrite(LED_BUILTIN, LOW);
        if (last < millis() - 1000)
        {
            last = millis();
            digitalWrite(LED_BUILTIN, HIGH);
        }
        if (last < millis() - 500)
        {
            digitalWrite(LED_BUILTIN, LOW);
        }
        digitalWrite(LED_BUILTIN, HIGH); // turn the LED on (HIGH is the voltage level)

        delay(500 / freq);              // wait for a second
        digitalWrite(LED_BUILTIN, LOW); // turn the LED off by making the voltage LOW
        delay(500 / freq);              // wait for a second
    }
    while (Serial.available() > 0)
    {
        digitalWrite(LED_BUILTIN, HIGH);
        inString = Serial.readString();
        if (inString.toInt() > 0 && inString.toInt() < 20)
        {
            freq = inString.toInt();
        }
        if(inString=="save")
        {
            // EEPROM.write(address, value)
        }
    }
}

void establishContact()
{
    while (Serial.available() <= 0)
    {
        Serial.print("Contact Established");
        delay(300);
    }
}
