#pragma once
#include "mbed.h"

class SingleDigitDisp
{
    BusOut pins;
    enum Segment : int
    {
        A = 0b1 << 0,
        B = 0b1 << 1,
        C = 0b1 << 2,
        D = 0b1 << 3,
        E = 0b1 << 4,
        F = 0b1 << 5,
        G = 0b1 << 6
    };
    std::array<int, 10> digitPins
    {
        (A | B | C | D | E | F),
        (B | C),
        (A | B | D | E | G),
        (A | B | C | D | G),
        (B | C | F | G),
        (A | C | D | F | G),
        (A | C | D | E | G | F),
        (A | B | C),
        (A | B | C | D | E | F | G),
        (A | B | C | F | G)
    };

public:
    SingleDigitDisp(
            PinName a,
            PinName b,
            PinName c,
            PinName d,
            PinName e,
            PinName f,
            PinName g
    )
        : pins(a, b, c, d, e, f, g)
    {
        pins = 0x00;
    }

    void show(unsigned int num)
    {
        if (num < 0 || num > 9) return;

        pins = digitPins[num];
    }

    void show(char c)
    {
        switch (c)
        {
        case 'a':
            pins = A;
            break;
        case 'b':
            pins = B;
            break;
        case 'c':
            pins = C;
            break;
        case 'd':
            pins = D;
            break;
        case 'e':
            pins = E;
            break;
        case 'f':
            pins = F;
            break;
        case 'g':
            pins = G;
            break;
        default:
            if ('0' <= c && c <= '9')
            {
                pins = digitPins[c - '0'];
            }
        }
    }
};
