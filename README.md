# FastPin
Fast digital pin reading and writing to ESP32 on Arduino platform.
Alternative to pinMode, digitalWrite and digitalRead.
The active low level can be set for both input and output.
Debounce is possible in case of input.

### Usage
```
constructor, initialize FastPin object with optional active low setting:

FastPin(const uint8_t pin, const uint8_t mode, const bool active_low = false)

void on() makes the pin high or low if in active low mode
void off() makes the pin low or high if in active low mode
bool read() returns the state of the pin according to the active low setting
void set_debounce(const uint32_t times) setting the debounce test number
bool state() returns the debounced state of the pin according to the active low setting
```
### Example
```
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
    if (button.debounced_state())
        led.on();
    else
        led.off();
}
```
