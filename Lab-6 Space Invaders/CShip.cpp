#include "stdafx.h"
#include "CShip.h"

CShip::CShip(cv::Size board)
{
    _shape.width = board.width * 0.08f;
    _shape.height = board.height * 0.04f;

    _position = cv::Point2f(
        board.width / 2.0f - _shape.width / 2.0f,
        board.height * 0.96f
    );

    _lives = 3;
}