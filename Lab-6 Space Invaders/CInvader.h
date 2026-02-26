#pragma once
#include "CGameObject.h"

/**
* @class CInvader
* @brief Invader enemy object
*
* Derived from CGameObject.
*
* @author Jass Saini
*
*/
class CInvader : public CGameObject
{
public:

    /**
    *
    * @brief Constructor for CInvader class.
    *
    * @param board Size of game board
    * @param pos Initial position
    *
    */
    CInvader(cv::Size board, cv::Point2f pos);

    /**
    *
    * @brief Moves invader object.
    *
    * @param dt Delta time between frames
    *
    * @return Nothing to return
    *
    */
    void move(double dt) override;

    /**
    *
    * @brief Draws invader on screen.
    *
    * @param im Reference to image
    *
    * @return Nothing to return
    *
    */
    void draw(cv::Mat& im) override;

private:
    cv::Mat _sprite_a; ///< First animation frame
    cv::Mat _sprite_b; ///< Second animation frame
    bool _frame_toggle; ///< Animation toggle flag
    float _anim_timer; ///< Animation timer
};