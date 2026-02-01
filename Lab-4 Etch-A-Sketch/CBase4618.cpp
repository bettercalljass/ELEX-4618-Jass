#include "stdafx.h"
#include "CBase4618.h"

CBase4618::CBase4618()
{
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
    bool done = false;

    while (!done)
    {
        update();
        draw();

        if (cv::waitKey(1) == 'q')
            done = true;

        if (exit_requested())
            done = true;
    }

    cv::destroyAllWindows();
}

bool CBase4618::exit_requested()
{
    return false;  
}
