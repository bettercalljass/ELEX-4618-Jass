#include "stdafx.h"
#include "CBase4618.h"
#include <thread>
#include <chrono>
#include <windows.h>
#include <mmsystem.h>
#include <iomanip>
#include <iostream>
#pragma comment(lib, "winmm.lib")

CBase4618::CBase4618()
{
    _last_time = cv::getTickCount();
    _dt = 0;
    _fps = 0;
}

CBase4618::~CBase4618()
{
}

CControl& CBase4618::board()
{
    return _board;
}

cv::Mat& CBase4618::canvas()
{
    return _canvas;
}

void CBase4618::run()
{
    timeBeginPeriod(1);
    using clock = std::chrono::high_resolution_clock;
    const double target_dt = 1.0 / 30.0;

    bool done = false;
    auto previous = clock::now();

    while (!done)
    {
        auto frame_start = clock::now();
        std::chrono::duration<double> elapsed = frame_start - previous;
        previous = frame_start;

        _dt = elapsed.count();

        if (_dt > 0)
            _fps = 1.0 / _dt;

        gpio();
        update();
        draw();

        if (cv::waitKey(1) == 'q')
            done = true;

        if (exit_requested())
            done = true;

        auto frame_end = clock::now();
        std::chrono::duration<double> work_time = frame_end - frame_start;

        double remaining = target_dt - work_time.count();

        if (remaining > 0)
        {
            std::this_thread::sleep_for(
                std::chrono::duration<double>(remaining));
        }
    }

    timeEndPeriod(1);

    cv::destroyAllWindows();
}

bool CBase4618::exit_requested()
{
    return false;
}