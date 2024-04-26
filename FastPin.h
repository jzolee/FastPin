#pragma once

#include <Arduino.h>

class FastPin {
public:
    FastPin(const uint8_t pin, const uint8_t mode, const bool active_low = false);
    void on() { _inverse ? (REG_WRITE(_c_reg, _bit)) : (REG_WRITE(_s_reg, _bit)); }
    void off() { _inverse ? (REG_WRITE(_s_reg, _bit)) : (REG_WRITE(_c_reg, _bit)); }
    bool read() { return (REG_GET_BIT(_r_reg, _bit) ? (_inverse ? false : true) : (_inverse ? true : false)); } // (REG_READ(_r_reg) & _bit)
    void set_debounce(const uint32_t times) { _times = times; }
    bool state();

private:
    uint32_t _s_reg;
    uint32_t _c_reg;
    uint32_t _r_reg;
    uint32_t _bit;

    uint32_t _times = 0;
    uint32_t _counter = 0;
    bool _state = false;
    bool _inverse = false;
};

FastPin::FastPin(const uint8_t pin, const uint8_t mode, const bool active_low)
{
    _inverse = active_low;

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

inline bool FastPin::state()
{
    if (read()) {
        if (_counter < _times)
            _counter++;
        else
            _state = true;
    } else {
        if (_counter > 0)
            _counter--;
        else
            _state = false;
    }
    return _state;
}
