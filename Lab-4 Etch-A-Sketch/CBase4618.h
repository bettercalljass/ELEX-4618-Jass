#pragma once

#include "stdafx.h"
#include "CControl.h"
#include <opencv2/opencv.hpp>

/**
* @class CBase4618
* @brief Base class for ELEX4618 OpenCV labs
*
* This class provides a common framework for
* OpenCV based applications using the CControl
* hardware interface.
*
* @author Jass Saini
*
*/
class CBase4618
{
private:

    CControl _board; ///< Hardware control object.
    cv::Mat  _canvas; ///< OpenCV drawing canvas.

public:
    /**
    *
    * @brief Constructor for CBase4618 class.
    *
    * Initializes internal variables.
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
    * @brief Provides access to CControl object.
    *
    * @return Reference to CControl object
    *
    */
    CControl& board();

    /**
    *
    * @brief Provides access to OpenCV canvas.
    *
    * @return Reference to cv::Mat canvas
    *
    */
    cv::Mat& canvas();

    /**
    *
    * @brief Update internal state.
    *
    * Must be implemented by derived class.
    *
    */
    virtual void update() = 0;

    /**
    *
    * @brief Draw to canvas.
    *
    * Must be implemented by derived class.
    *
    */
    virtual void draw() = 0;

    /**
    *
    * @brief Checks if application requested exit.
    *
    * @return True when exit requested, false otherwise
    *
    */
    virtual bool exit_requested();

    /**
    *
    * @brief Runs update and draw loop.
    *
    */
    void run();
};
