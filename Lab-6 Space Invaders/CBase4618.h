#pragma once

#include "stdafx.h"
#include "CControl.h"
#include <opencv2/opencv.hpp>

/**
* @class CBase4618
* @brief Base class for ELEX4618 OpenCV labs
*
* Provides framework loop, timing and hardware interface.
*
* @author Jass Saini
*
*/
class CBase4618
{
private:
    CControl _board; ///< Hardware control object
    cv::Mat  _canvas; ///< OpenCV drawing canvas

    double _last_time; ///< Previous frame timestamp
    double _dt; ///< Delta time between frames
    double _fps; ///< Frames per second

public:

    /**
    *
    * @brief Constructor for CBase4618 class.
    *
    * Initializes timing variables.
    *
    */
    CBase4618();

    /**
    *
    * @brief Destructor for CBase4618 class.
    *
    */
    virtual ~CBase4618();

    /**
    *
    * @brief Provides access to hardware board.
    *
    * @return Reference to CControl object
    *
    */
    CControl& board();

    /**
    *
    * @brief Provides access to drawing canvas.
    *
    * @return Reference to cv::Mat canvas
    *
    */
    cv::Mat& canvas();

    /**
    *
    * @brief Handles hardware IO step.
    *
    * Must be implemented by derived class.
    *
    * @return Nothing to return
    *
    */
    virtual void gpio() = 0;

    /**
    *
    * @brief Updates program logic.
    *
    * Must be implemented by derived class.
    *
    * @return Nothing to return
    *
    */
    virtual void update() = 0;

    /**
    *
    * @brief Renders frame to screen.
    *
    * Must be implemented by derived class.
    *
    * @return Nothing to return
    *
    */
    virtual void draw() = 0;

    /**
    *
    * @brief Checks if application should exit.
    *
    * @return True when exit requested, false otherwise
    *
    */
    virtual bool exit_requested();

    /**
    *
    * @brief Time between frames.
    *
    * @return Delta time in seconds
    *
    */
    double dt() const {
        return _dt;
    }

    /**
    *
    * @brief Current frames per second.
    *
    * @return FPS value
    *
    */
    double fps() const {
        return _fps;
    }

    /**
    *
    * @brief Runs main program loop.
    *
    * @return Nothing to return
    *
    */
    void run();
};