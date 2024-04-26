#include <Arduino.h>
#include "FastPin.h"

// FastPin(const uint8_t pin, const uint8_t mode, const bool active_low = false);

FastPin button(18, INPUT_PULLUP, true);
FastPin led(36, OUTPUT);

void setup()
{
    Serial.begin(115200);

    button.set_debounce(100);
}

void loop()
{
    if (button.state())
        led.on();
    else
        led.off();
}
