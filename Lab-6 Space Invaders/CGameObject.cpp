#include "stdafx.h"
#include "CGameObject.h"

CGameObject::CGameObject()
{
    _position = { 0,0 };
    _velocity = { 0,0 };
    _shape = { 0,0,0,0 };
    _lives = 1;
}

void CGameObject::move(double dt)
{
    _position += _velocity * dt;
}

bool CGameObject::collide(CGameObject& obj)
{
    cv::Rect2f r1(_position.x, _position.y, _shape.width, _shape.height);
    cv::Rect2f r2(obj._position.x, obj._position.y,
        obj._shape.width, obj._shape.height);

    return (r1 & r2).area() > 0;
}

bool CGameObject::collide_wall(cv::Size board)
{
    if (_position.x < 0 ||
        _position.x + _shape.width > board.width ||
        _position.y < 0 ||
        _position.y + _shape.height > board.height)
        return true;

    return false;
}

void CGameObject::hit()
{
    _lives--;
}

void CGameObject::draw(cv::Mat& im)
{
    cv::rectangle(im,
        cv::Point(_position.x, _position.y),
        cv::Point(_position.x + _shape.width,
            _position.y + _shape.height),
        cv::Scalar(255, 255, 255),
        -1);
}