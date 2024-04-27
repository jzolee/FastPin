#pragma once

#include <Arduino.h>

class FastPin {
public:
    FastPin(const uint8_t pin, const uint8_t mode, const bool active_low = false);

    void high() { (REG_WRITE(_s_reg, _bit)); }
    void low() { (REG_WRITE(_c_reg, _bit)); }
    void on() { _active_low ? low() : high(); }
    void off() { _active_low ? high() : low(); }
    void set_debounce(const uint32_t times) { _times = times; }
    bool read() { return (REG_GET_BIT(_r_reg, _bit)); }
    bool state() { return read() ^ _active_low; }
    bool debounced_read();
    bool debounced_state() { return debounced_read() ^ _active_low; }

private:
    uint32_t _s_reg;
    uint32_t _c_reg;
    uint32_t _r_reg;
    uint32_t _bit;

    uint32_t _times = 0;
    uint32_t _counter = 0;
    bool _debounced = false;
    bool _active_low = false;
};

FastPin::FastPin(const uint8_t pin, const uint8_t mode, const bool active_low)
{
    _active_low = active_low;

    pinMode(pin, mode);

    if (pin < 32) {
        _bit = 1 << pin;
        _s_reg = GPIO_OUT_W1TS_REG;
        _c_reg = GPIO_OUT_W1TC_REG;
        _r_reg = GPIO_IN_REG;
    } else {
        _bit = 1 << (pin - 32);
        _s_reg = GPIO_OUT1_W1TS_REG;
        _c_reg = GPIO_OUT1_W1TC_REG;
        _r_reg = GPIO_IN1_REG;
    }
}

inline bool FastPin::debounced_read()
{
    if (read()) {
        if (_counter < _times)
            _counter++;
        else
            _debounced = true;
    } else {
        if (_counter > 0)
            _counter--;
        else
            _debounced = false;
    }
    return _debounced;
}
