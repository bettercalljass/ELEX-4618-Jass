#include "LedManager.h"

LedManager::LedManager()
{
    _flash_time = 0.35;

    for (int i = 0; i < 4; i++)
    {
        _timers[i] = 0.0;
    }
}

void LedManager::trigger(int bin)
{
    if (bin >= 1 && bin <= 4)
    {
        _timers[bin - 1] = _flash_time;
    }
}

void LedManager::update(double dt, CControl& board)
{
    int led_pins[4] = { 6, 12, 13, 16 };

    for (int i = 0; i < 4; i++)
    {
        if (_timers[i] > 0.0)
        {
            _timers[i] -= dt;
        }

        board.set_data(
            Digital,
            led_pins[i],
            _timers[i] > 0.0 ? 1 : 0
        );
    }
}
