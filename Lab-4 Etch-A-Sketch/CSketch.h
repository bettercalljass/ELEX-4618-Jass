#pragma once

#include "CBase4618.h"

/**
* @class CSketch
* @brief Etch-A-Sketch application class
*
* Implements an Etch-A-Sketch style drawing
* application using joystick input and OpenCV.
*
* @author Jass Saini
*
*/
class CSketch : public CBase4618
{
private:

    bool _exit; ///< Indicates if exit was requested.

public:
    /**
    *
    * @brief Constructor for CSketch class.
    *
    * @param size Size of OpenCV drawing canvas
    * @param comport Target COM port
    *
    */
    CSketch(cv::Size size, int comport);

    /**
    *
    * @brief Destructor for CSketch class.
    *
    */
    virtual ~CSketch();

    /**
    *
    * @brief Updates application state.
    *
    */
    virtual void update() override;

    /**
    *
    * @brief Draws graphics and GUI to canvas.
    *
    */
    virtual void draw() override;

    /**
    *
    * @brief Checks if exit button was pressed.
    *
    * @return True when exit requested, false otherwise
    *
    */
    virtual bool exit_requested() override;
};
