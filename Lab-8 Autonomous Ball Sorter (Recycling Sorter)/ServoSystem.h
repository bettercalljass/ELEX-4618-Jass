#pragma once

#include "CControl.h"

class ServoSystem
{
private:
    int _ramp_angle;
    int _feed_angle;
    int _drop_angle;

public:
    ServoSystem();

    void update(CControl& board);

    void go_home();

    void sort_bin1();
    void sort_bin2();
    void sort_bin3();
    void sort_bin4();

    void feed_open();
    void feed_close();

    void drop_open();
    void drop_close();
};
