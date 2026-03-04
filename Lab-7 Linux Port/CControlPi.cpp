#include "CControlPi.h"
#include <pigpio.h>
#include <iostream>
#include <thread>
#include <chrono>

#define JOYSTICK_MAX 4095.0f

CControl::CControl()
{
    if (gpioInitialise() < 0)
    {
        std::cout << "pigpio init failed\n";
    }

    _connected = true;
    _lastDebounceTime = std::chrono::steady_clock::now();
}

CControl::~CControl()
{
    gpioTerminate();
}

void CControl::init_com(int comport)
{
    // Not used on Raspberry Pi
}

bool CControl::get_data(int type, int channel, int& result)
{
    if (type == Digital)
    {
        gpioSetMode(channel, PI_INPUT);
        result = gpioRead(channel);
        return true;
    }

    if (type == Servo)
    {
        // Not readable
        return false;
    }

    if (type == Analog)
    {
        // Placeholder (SPI MCP3008 added later)
        result = 0;
        return false;
    }

    return false;
}

bool CControl::set_data(int type, int channel, int value)
{
    if (type == Digital)
    {
        gpioSetMode(channel, PI_OUTPUT);
        gpioWrite(channel, value);
        return true;
    }

    if (type == Servo)
    {
        //convertv 0-180 degrees to pulse width (500-2500us)
        int puslewidth = 500 + (value*2000/180);
        gpioServo(channel, puslewidth);
        return true;
    }

    return false;
}

bool CControl::get_analog(int channel, float& percentFullScale)
{
    int raw = 0;

    if (!get_data(Analog, channel, raw))
        return false;

    percentFullScale = raw / JOYSTICK_MAX;
    return true;
}

bool CControl::get_button(int channel, bool& pressed)
{
    int val;
    if (!get_data(Digital, channel, val))
        return false;

    pressed = (val == 0);
    return true;
}

bool CControl::auto_connect()
{
    return true; // Always connected on Pi
}

bool CControl::ensure_connection()
{
    return true;
}
