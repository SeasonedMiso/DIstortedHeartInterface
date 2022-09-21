#include <DS1804.h>
#include <EEPROM.h>
DS1804 pot1 = DS1804(6, 5, 4, DS1804_HUNDRED);

// Current preset
int preset = 1;
// Current Preset Variables from 0 to 100
int pot1Val = 0;

// EEProm address definitions
int AddrPre1pot1Val = 0;
int AddrPre2pot1Val = 1;
int AddrPre3pot1Val = 2;

// bitValue = ((percentageValue * 255) / 100);

void setup()
{
    Serial.begin(9600);
    pot1.unlock();
    EEPROM.get(AddrPre1pot1Val, pot1Val);
    //  assign those to the pots.
}

void loop()
{
    String inString = "";
    while (Serial.available() > 0)
    {
        digitalWrite(LED_BUILTIN, HIGH);
        inString = Serial.readString();
        Serial.write("Received Data:\n");
        Serial.println(inString);

        if (inString[0] == "p" && (int)inString[1] > 0 && (int)inString[0] < 4)
        {
            // load values for current preset from eeprom
            Serial.write("Changing Preset \n");
            preset = (int)inString[1];
            if (preset == 1)
            {
                EEPROM.get(AddrPre1pot1Val, pot1Val);
                pot1.setWiperPosition(pot1Val);
            }
            if (preset == 2)
            {
                EEPROM.get(AddrPre2pot1Val, pot1Val);
                pot1.setWiperPosition(pot1Val);
            }
            if (preset == 3)
            {
                EEPROM.get(AddrPre3pot1Val, pot1Val);
                pot1.setWiperPosition(pot1Val);
            }
        }
        if (inString[0] == "u" && preset == (int)inString[1])
        {
            // update the pot to the value
            Serial.write("Updating pot \n");
            pot1Val = ((int)(inString[3] * 255) / 100);
            pot1.setWiperPosition(pot1Val);
        }
        if (inString[0] == "s" && preset == (int)inString[1])
        {
            // write current vals to eeprom
            Serial.write("Saving preset to memory\n");
            if (preset == 1)
            {
                EEPROM.update(AddrPre1pot1Val, pot1Val);
                pot1.setWiperPosition(pot1Val);
            }
            if (preset == 2)
            {
                EEPROM.update(AddrPre2pot1Val, pot1Val);
                pot1.setWiperPosition(pot1Val);
            }
            if (preset == 3)
            {
                EEPROM.update(AddrPre3pot1Val, pot1Val);
                pot1.setWiperPosition(pot1Val);
            }
        }
    }
}
