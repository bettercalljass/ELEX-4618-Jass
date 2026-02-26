#pragma once
#include "CGameObject.h"

/**
* @class CMissile
* @brief Missile object for player and enemies
*
* Derived from CGameObject.
*
* @author Jass Saini
*
*/
class CMissile : public CGameObject
{
public:

    /**
    *
    * @brief Constructor for CMissile class.
    *
    * @param board Size of game board
    * @param pos Initial position
    * @param vel Initial velocity
    * @param color Missile color
    *
    */
    CMissile(cv::Size board,
        cv::Point2f pos,
        cv::Point2f vel,
        cv::Scalar color);

    /**
    *
    * @brief Moves missile object.
    *
    * @param dt Delta time between frames
    *
    * @return Nothing to return
    *
    */
    void move(float dt);

    /**
    *
    * @brief Draws missile on screen.
    *
    * @param im Reference to image
    *
    * @return Nothing to return
    *
    */
    void draw(cv::Mat& im) override;

private:
    cv::Size _board; ///< Game board size
    cv::Scalar _color; ///< Missile color
};