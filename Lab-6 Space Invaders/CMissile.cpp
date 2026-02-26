#include "stdafx.h"
#include "CMissile.h"

CMissile::CMissile(cv::Size board,
    cv::Point2f pos,
    cv::Point2f vel,
    cv::Scalar color)
{
    _board = board;
    _color = color;

    _shape.width = board.width * 0.01f;
    _shape.height = board.height * 0.03f;

    _position = pos;
    _velocity = vel;
    _lives = 1;
}

void CMissile::move(float dt)
{
    _position += _velocity * dt;

    if (_position.y + _shape.height < 0 ||
        _position.y > _board.height)
    {
        hit();  
    }
}

void CMissile::draw(cv::Mat& im)
{
    cv::rectangle(im,
        cv::Rect(_position.x,
            _position.y,
            _shape.width,
            _shape.height),
        _color,
        -1);
}