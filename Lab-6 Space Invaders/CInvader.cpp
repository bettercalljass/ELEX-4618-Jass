#include "stdafx.h"
#include "CInvader.h"

CInvader::CInvader(cv::Size board, cv::Point2f pos)
{
    _shape.width = board.width * 0.06f;
    _shape.height = board.height * 0.04f;

    _sprite_a = cv::imread("Sprite1.png", cv::IMREAD_UNCHANGED);
    _sprite_b = cv::imread("Sprite2.png", cv::IMREAD_UNCHANGED);

    _frame_toggle = false;
    _anim_timer = 0.0f;

    _position = pos;
    _lives = 1;
}

void CInvader::move(double dt)
{
    _anim_timer += dt;

    if (_anim_timer > 0.7f)
    {
        _frame_toggle = !_frame_toggle;
        _anim_timer = 0.0f;
    }
}

void CInvader::draw(cv::Mat& im)
{
    cv::Mat& sprite = _frame_toggle ? _sprite_a : _sprite_b;

    if (sprite.empty())
        return;

    cv::Rect roi(
        (int)_position.x,
        (int)_position.y,
        sprite.cols,
        sprite.rows
    );

    if (roi.x >= 0 && roi.y >= 0 &&
        roi.x + roi.width <= im.cols &&
        roi.y + roi.height <= im.rows)
    {
        if (sprite.channels() == 4)
        {
            std::vector<cv::Mat> channels;
            cv::split(sprite, channels);

            cv::Mat rgb;
            cv::merge(std::vector<cv::Mat>{channels[0], channels[1], channels[2]}, rgb);

            cv::Mat alpha = channels[3];

            rgb.copyTo(im(roi), alpha);
        }
        else
        {
            sprite.copyTo(im(roi));
        }
    }
}