#pragma once

#include "mbed.h"

class SensorEvents
{
    static EventFlags flags;
    constexpr static uint32_t pollFlag = 1UL << 0;
    static uint32_t top;
    static uint32_t sensors;
    uint32_t thisFlag;
    uint32_t &index;

public:
    SensorEvents(uint32_t &index)
        : index(index)
    {
        index = ++top;
        thisFlag = 1UL << index;
        sensors = sensors | thisFlag;
    }

    static void init()
    {
        top = 0;
        sensors = 0;
    }

    static void waitPoll()
    {
        flags.wait_any(pollFlag, osWaitForever, false);
        return;
    }

    static uint32_t waitTriggered()
    {
        flags.set(pollFlag);
        uint32_t detections = flags.wait_any(sensors);
        flags.clear();
        for (uint32_t i = 1; i <= top; i++)
        {
            if ((1UL << i) & detections)
            {
                return i;
            }
        }
        return -1;
    }

    void trigger()
    {
        flags.set(thisFlag);
    }
};

uint32_t SensorEvents::top = 0;
uint32_t SensorEvents::sensors = 0;
EventFlags SensorEvents::flags;

class Sensor
{
    AnalogIn pin;
    unsigned short threshold;
    Thread thread;
    SensorEvents eventHandler;
    uint32_t &index;

    void poll_thread()
    {
        while (true)
        {
            eventHandler.waitPoll();
            unsigned short reading = pin.read_u16();
            if (reading < threshold)
            {
                eventHandler.trigger();
            }
            wait_us(3000);
        }
    }

public:
    Sensor(PinName pin, unsigned short threshold, uint32_t &index)
        : pin(pin), threshold(threshold), eventHandler(index), index(index)
    {
    }
    
    void start()
    {
        thread.start(callback(this, &Sensor::poll_thread));
    }
};
