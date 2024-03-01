#pragma once

#include "mbed.h"
#include <string>

class Uart4DigitDisp
{
    BufferedSerial serial;

public:
    Uart4DigitDisp(PinName txPin)
        : serial(txPin, NC)
    {
        // Clear the display, set brightness to 100%
        char c[] = {0x76, 0x7a, 100};
        serial.write(c, 3);
    }

    void show(unsigned int num)
    {
        char msg[6] =
        {
            0x79, // Cursor set command
            0x00, // Set cursor to pos 0
            ' ',  // To be filled with the digits L-R
            ' ',
            ' ',
            ' '
        };

        if (num == 0)
        {
            msg[5] = '0';
        }
        else
        {
            for (int i = 5; i >= 2 && num > 0; --i)
            {
                msg[i] = num % 10;
                num /= 10;
            }
        }

        serial.write(msg, 6);
    }
    
    void setBrightness(char brightness)
    {
        char msg[] = {0x7a, brightness};
        serial.write(msg, 2);
    }
};
