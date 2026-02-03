#include "stdafx.h"
#include "CSketch.h"
#include "cvui.h"
#include <cmath>

#define JOY_X 11
#define JOY_Y 4
#define RESET_BUTTON 33
#define SET_BUTTON 32
#define LED_BLUE  37
#define LED_GREEN 38
#define LED_RED 39

CSketch::CSketch(cv::Size size, int comport)
{
    _size = size;
    canvas() = cv::Mat::zeros(size, CV_8UC3);
    board().auto_connect();

    _cursor = cv::Point(size.width / 2, size.height / 2);
    _prevCursor = _cursor;
    _color = GREEN;

    board().set_data(Digital, LED_GREEN, 1);
    board().set_data(Digital, LED_RED, 0);
    board().set_data(Digital, LED_BLUE, 0);
    _exit = false;

    cvui::init("Etch-A-Sketch");

    _prevX = 0.0f;
    _prevY = 0.0f;
    _prevZ = 0.0f;
}

CSketch::~CSketch()
{
}

void CSketch::update()
{ 
    bool colorPressed = false;
    board().get_button(SET_BUTTON, colorPressed);

    if (colorPressed)
    {
        if (_color == GREEN) _color = RED;
        else if (_color == RED) _color = BLUE;
        else _color = GREEN;

        board().set_data(Digital, LED_RED, 0);
        board().set_data(Digital, LED_GREEN, 0);
        board().set_data(Digital, LED_BLUE, 0);

        if (_color == GREEN)
            board().set_data(Digital, LED_GREEN, 1);
        else if (_color == RED)
            board().set_data(Digital, LED_RED, 1);
        else
            board().set_data(Digital, LED_BLUE, 1);

        Sleep(100);
    }

    bool resetPressed = false;
    board().get_button(RESET_BUTTON, resetPressed);

    if (resetPressed)
    {
        canvas() = cv::Mat::zeros(_size, CV_8UC3);
    }

    float xPct = 0;
    float yPct = 0;

    board().get_analog(JOY_X, xPct);
    board().get_analog(JOY_Y, yPct);

    const float DEADZONE = 0.05f;
    float joy_x = xPct - 0.5f;
    float joy_y = yPct - 0.5f;

    if (fabs(joy_x) < DEADZONE) dx = 0;
    if (fabs(joy_y) < DEADZONE) dy = 0;

    float speed = 30.0f;
    _prevCursor = _cursor;
    _cursor.x += (int)(joy_x * speed);
    _cursor.y += (int)(-joy_y * speed);   

    if (_cursor.x < 0) _cursor.x = 0;
    if (_cursor.y < 0) _cursor.y = 0;
    if (_cursor.x >= _size.width)  _cursor.x = _size.width - 1;
    if (_cursor.y >= _size.height) _cursor.y = _size.height - 1;
}

void CSketch::draw()
{
    cvui::window(canvas(), 10, 10, 200, 90, "Etch-A-Sketch Jass Saini");
    std::string colorText = "Color: ";

    if (_color == GREEN) colorText += "GREEN";
    else if (_color == RED) colorText += "RED";
    else colorText += "BLUE";

    cvui::text(canvas(), 25, 35, colorText);

    if (cvui::button(canvas(), 20, 60, 80, 30, "Exit"))
        _exit = true;

    if (cvui::button(canvas(), 120, 60, 80, 30, "Reset"))
    {
        canvas() = cv::Mat::zeros(_size, CV_8UC3);
    }

    bool inGui =
        (_cursor.x >= 10 && _cursor.x <= 200 &&
            _cursor.y >= 10 && _cursor.y <= 90);

    cv::Scalar drawColor;

    if (_color == GREEN) drawColor = cv::Scalar(0, 255, 0);
    else if (_color == RED) drawColor = cv::Scalar(0, 0, 255);
    else drawColor = cv::Scalar(255, 0, 0);

    if (!inGui)
    {
        cv::line(canvas(), _prevCursor, _cursor, drawColor, 2);
    }

    cvui::update();
    cv::imshow("Etch-A-Sketch", canvas());
}

bool CSketch::exit_requested()
{
    return _exit;
}
}
    
