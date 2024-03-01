/* mbed Microcontroller Library
 * Copyright (c) 2023 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "servo.hpp"
#include "ServoList.hpp"
#include "buzzer.hpp"
#include "sensor.hpp"
#include "SingleDigitDisp.hpp"
#include "Uart4DigitDisp.hpp"

Buzzer buzzer(p14, p15);

SingleDigitDisp lDisp(
        p9,     // a (device pin  7)
        p10,    // b (device pin  6)
        p13,    // c (device pin  4)
        p12,    // d (device pin  2)
        p11,    // e (device pin  1)
        p8,     // f (device pin  9)
        p7      // g (device pin 10)
);

Uart4DigitDisp rDisp(p28);

ServoList servos
({
    Servo(p21),
    Servo(p22, 27),
    Servo(p23, 1),
    Servo(p24),
    Servo(p25)
});

uint32_t aId, bId, cId, dId, eId;

int main()
{
    // SensorEvents::init();
    Sensor sensors[] = {
        Sensor(p20, 40000, aId),
        Sensor(p19, 40000, bId),
        Sensor(p18, 40000, cId),
        Sensor(p17, 40000, dId),
        Sensor(p16, 40000, eId)
    };

    for (auto &sensor : sensors)
    {
        sensor.start();
    }

    unsigned int score = 0;
    unsigned int moves = 0;

    lDisp.show(moves);
    rDisp.show(score);
    servos.reset();
    wait_us(1000000);
    auto curCmd = allHoles.begin();
    auto end = allHoles.end();
    while (true)
    {
        servos.execCommand(*curCmd);
        moves += curCmd->size();
        lDisp.show(moves);
        uint32_t triggered = SensorEvents::waitTriggered();
        if (triggered == aId)
        {
            score += 25;
            buzzer.playFreq(1658, 500ms);
        }
        else if (triggered == bId)
        {
            score += 30;
            buzzer.playFreq(2036, 500ms);
        }
        else if (triggered == cId)
        {
            score += 50;
            buzzer.playFreq(2500, 500ms);
        }
        else if (triggered == dId)
        {
            score += 20;
            buzzer.playFreq(1351, 500ms);
        }
        else if (triggered == eId)
        {
            score += 10;
            buzzer.playFreq(1100, 500ms);
        }
        rDisp.show(score);
        curCmd++;
        if (curCmd == end)
        {
            buzzer.playFreq(1100, 200000us);
            wait_us(200000);
            buzzer.playFreq(1650, 100000us);
            wait_us(100000);
            buzzer.playFreq(1375, 100000us);
            wait_us(100000);
            buzzer.playFreq(1650, 100000us);
            wait_us(100000);
            buzzer.playFreq(1375, 100000us);
            wait_us(100000);
            buzzer.playFreq(1100, 200000us);
            wait_us(200000);
            buzzer.playFreq(2200, 100000us);
            wait_us(200000);
            buzzer.playFreq(2200, 100000us);
            wait_us(200000);
            moves = 0;
            score = 0;
            curCmd = maxScore.begin();
            end = maxScore.end();
            servos.reset();
        }
        wait_us(500000);
    }
    return 0;
}
