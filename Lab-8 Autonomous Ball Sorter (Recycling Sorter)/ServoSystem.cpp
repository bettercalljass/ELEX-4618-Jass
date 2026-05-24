#include "ServoSystem.h"

ServoSystem::ServoSystem()
{
    _ramp_angle = 85;

    _feed_angle = 90;

    _drop_angle = 90;
}

void ServoSystem::update(CControl& board)
{
    board.set_data(Servo, 18, _ramp_angle);

    board.set_data(Servo, 19, _drop_angle);

    board.set_data(Servo, 20, _feed_angle);
}

void ServoSystem::sort_to_bin(int bin)
{
    switch (bin)
    {
    case 1:
        _ramp_angle = 30;
        break;

    case 2:
        _ramp_angle = 70;
        break;

    case 3:
        _ramp_angle = 115;
        break;

    case 4:
        _ramp_angle = 150;
        break;
    }
}

void ServoSystem::go_home()
{
    _ramp_angle = 85;
}

void ServoSystem::feed_open()
{
    _feed_angle = 170;
}

void ServoSystem::feed_close()
{
    _feed_angle = 90;
}

void ServoSystem::drop_open()
{
    _drop_angle = 40;
}

void ServoSystem::drop_close()
{
    _drop_angle = 90;
}
