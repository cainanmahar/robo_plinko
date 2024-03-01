#pragma once

#include "mbed.h"
#include "servo.hpp"

struct ServoCommand
{
    unsigned int servo;
    float angle;
};

class ServoList
{
    std::vector<Servo> servos;

public:
    ServoList(std::initializer_list<Servo> servos)
        : servos(servos)
    {
    }

    void execCommand(const ServoCommand &cmd)
    {
        servos.at(cmd.servo).setAngle(cmd.angle);
    }

    void execCommand(const std::initializer_list<ServoCommand> &cmds)
    {
        for (const ServoCommand &cmd : cmds)
        {
            execCommand(cmd);
        }
    }

    void reset()
    {
        for (auto &servo : servos)
        {
            servo.reset();
        }
    }
};

constexpr std::array<std::initializer_list<ServoCommand>, 5> allHoles =
{
    std::initializer_list{ServoCommand {1, -67}},
    std::initializer_list{ServoCommand {4, -60}},
    std::initializer_list{ServoCommand {1, -48}},
    std::initializer_list{ServoCommand {1, -25}},
    std::initializer_list{ServoCommand {1,  42}, {4, -75}}
};

constexpr std::array<std::initializer_list<ServoCommand>, 5> maxScore =
{
    std::initializer_list{ServoCommand {1, -67}, {4, -60}},
    std::initializer_list<ServoCommand>{},
    std::initializer_list<ServoCommand>{},
    std::initializer_list<ServoCommand>{},
    std::initializer_list<ServoCommand>{}
};
