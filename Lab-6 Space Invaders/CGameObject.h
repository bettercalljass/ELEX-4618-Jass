#pragma once
#include "stdafx.h"
#include <opencv2/opencv.hpp>

/**
* @class CGameObject
* @brief Base class for all game objects
*
* Provides position, velocity and collision handling.
*
* @author Jass Saini
*
*/
class CGameObject
{
protected:
    cv::Point2f _position; ///< Object position
    cv::Point2f _velocity; ///< Object velocity
    cv::Rect2f  _shape;    ///< Object shape for collision
    int _lives;            ///< Number of lives

public:

    /**
    *
    * @brief Constructor for CGameObject class.
    *
    */
    CGameObject();

    /**
    *
    * @brief Destructor for CGameObject class.
    *
    */
    virtual ~CGameObject() {}

    /**
    *
    * @brief Moves object based on velocity.
    *
    * @param dt Delta time between frames
    *
    * @return Nothing to return
    *
    */
    virtual void move(double dt);

    /**
    *
    * @brief Checks collision with another object.
    *
    * @param obj Reference to other game object
    *
    * @return True if collision occurs, false otherwise
    *
    */
    bool collide(CGameObject& obj);

    /**
    *
    * @brief Checks collision with wall.
    *
    * @param board Size of game board
    *
    * @return True if wall collision occurs, false otherwise
    *
    */
    bool collide_wall(cv::Size board);

    /**
    *
    * @brief Reduces object lives when hit.
    *
    * @return Nothing to return
    *
    */
    void hit();

    /**
    *
    * @brief Gets number of lives.
    *
    * @return Lives value
    *
    */
    int get_lives() const {
        return _lives;
    }

    /**
    *
    * @brief Sets number of lives.
    *
    * @param l Lives value
    *
    */
    void set_lives(int l) {
        _lives = l;
    }

    /**
    *
    * @brief Sets object position.
    *
    * @param pos Position value
    *
    */
    void set_pos(cv::Point2f pos) {
        _position = pos;
    }

    /**
    *
    * @brief Gets object position.
    *
    * @return Position value
    *
    */
    cv::Point2f get_pos() const {
        return _position;
    }

    /**
    *
    * @brief Gets object width.
    *
    * @return Width value
    *
    */
    float get_width() const {
        return _shape.width;
    }

    /**
    *
    * @brief Gets object height.
    *
    * @return Height value
    *
    */
    float get_height() const {
        return _shape.height;
    }

    /**
    *
    * @brief Draws object on screen.
    *
    * @param im Reference to image
    *
    * @return Nothing to return
    *
    */
    virtual void draw(cv::Mat& im);
};