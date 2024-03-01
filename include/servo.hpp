#pragma once

#include "mbed.h"

constexpr float PW_MIN = 0.0005;
constexpr float PW_MAX = 0.0025;
constexpr float PW_RANGE = PW_MAX - PW_MIN;
constexpr float PW_MID = PW_MIN + PW_RANGE / 2;

class Servo
{
    PwmOut pin;
    float initialAngle;

public:
    Servo(PinName p_pin, float initialAngle=0.0f)
        : pin(p_pin), initialAngle(initialAngle)
    {
        pin.period(PW_MAX + 0.0020);
        reset();
    }

    void setAngle(float deg)
    {
        if (deg < -90.0 || deg > 90.0) return;
        deg += 90.0;
        pin.pulsewidth(deg * PW_RANGE / 180.0 + PW_MIN);
    }

    void reset()
    {
        setAngle(initialAngle);
    }
};
