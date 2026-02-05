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
    cv::Point _cursor; ///< Current cursor position
    cv::Point _prevCursor; ///< Previous cursor position
    cv::Size  _size; ///< Canvas size

    float _prevX; ///< Stores previous X-coordinate
    float _prevY; ///< Stores previous Y-coordinate
    float _prevZ; ///< Stores previous Z-coordinate

    enum DrawColor { GREEN = 0, RED, BLUE }; ///< Enum holds the colors
    DrawColor _color;   ///< Current drawing color

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
    * @return Nothing to return
    * 
    */
    virtual ~CSketch();

    /**
    *
    * @brief Updates application state.
    *
    * @return Nothing to return
    * 
    */
    virtual void update() override;

    /**
    *
    * @brief Draws graphics and GUI to canvas.
    * 
    * @return Nothing to return
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
   
