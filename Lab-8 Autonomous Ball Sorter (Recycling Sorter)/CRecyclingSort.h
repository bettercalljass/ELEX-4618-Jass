#pragma once

#include "CBase4618.h"

#include "CameraSystem.h"
#include "ColorDetector.h"
#include "ServoSystem.h"
#include "BinCounter.h"
#include "LedManager.h"
#include "UIManager.h"

class CRecyclingSort : public CBase4618
{
private:
    CameraSystem _camera;
    ColorDetector _detector;
    ServoSystem _servos;
    BinCounter _counters;
    LedManager _leds;
    UIManager _ui;

    bool _auto_mode;
    bool _exit_requested;

public:
    CRecyclingSort();

    void gpio() override;
    void update() override;
    void draw() override;
    bool exit_requested() override;
};
