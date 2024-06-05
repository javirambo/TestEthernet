#include <Arduino.h>
#include "debug.h"
#include "Leds/Leds.h"

void setup()
{
    Serial.begin(115200);
    LedsInit();
    pinMode(0, INPUT_PULLUP);    // BUTTON BOOT
    //-----------------------------------------
    LogI("arrancanding...");
    delay(1000);
}

void loop()
{

}
