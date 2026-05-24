#pragma once

#include "CControl.h"

class LedManager
{
private:
    double _timers[4];
    double _flash_time;

public:
    LedManager();

    void trigger(int bin);

    void update(double dt, CControl& board);
};
