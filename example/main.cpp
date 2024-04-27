#include <Arduino.h>
#include "FastPin.h"

FastPin button(18, INPUT_PULLUP, true);
FastPin led(17, OUTPUT);

void setup()
{
    button.set_debounce(100);
}

void loop()
{
    if (button.state_debounce())
        led.on();
    else
        led.off();
}
