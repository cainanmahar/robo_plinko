#pragma once
#include "mbed.h"

constexpr int ONE_SECOND = 1000000;

class Buzzer
{
    DigitalOut pinA, pinB;
    Timeout flipTO, stopTO;
    bool isStopped = true;
    int period;

    void flip()
    {
        if (isStopped) return;
        pinA = !pinA;
        pinB = !pinA;
        flipTO.attach(
                callback(this, &Buzzer::flip),
                std::chrono::microseconds(period)
        );
    }

public:
    Buzzer(PinName a, PinName b)
        : pinA(a), pinB(b)
    {
        setFreq(1100);
        stop();
    }

    void start()
    {
        CriticalSectionLock lock;
        if (isStopped)
        {
            isStopped = false;
            flip();
        }
    }

    void stop()
    {
        CriticalSectionLock lock;
        isStopped = true;
    }

    void setFreq(int freq)
    {
        // Filter unplayable frequencies
        if (freq < 1100 || freq > 2500)
        {
            pinA = pinB = false;
            return;
        }
        // Calculate new flip period. Flips occur twice per full period.
        int newPeriod = ONE_SECOND / (freq * 2);
        // Disable interrupts until the end of the function
        // *after* calculations
        CriticalSectionLock lock;
        // Assign to the internal variable safely
        period = newPeriod;
    }

    template <typename T>
    void playFreq(int freq, T duration)
    {
       stopTO.detach(); 
       setFreq(freq);
       start();
       stopTO.attach(
               callback(this, &Buzzer::stop),
               duration
       );
    }

    void playFreq(int freq)
    {
       stopTO.detach(); 
       setFreq(freq);
       start();
    }
};
