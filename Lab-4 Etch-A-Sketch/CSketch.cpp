#include "stdafx.h"
#include "CSketch.h"
#include "cvui.h"

CSketch::CSketch(cv::Size size, int comport)
{
    canvas() = cv::Mat::zeros(size, CV_8UC3);
    board().auto_connect();

    _exit = false;

    cvui::init("Etch-A-Sketch");
}

CSketch::~CSketch()
{
}

void CSketch::update()
{
    // update sketch
}

void CSketch::draw()
{
    cvui::window(canvas(), 10, 10, 200, 120, "Etch-A-Sketch");

    if (cvui::button(canvas(), 30, 60, 80, 30, "Exit"))
    {
        _exit = true;
    }

    cvui::update();
    cv::imshow("Etch-A-Sketch", canvas());
}

bool CSketch::exit_requested()
{
    return _exit;
}
