#pragma once
#include "CBase4618.h"

/**
* @class CPong
* @brief Pong game derived from Base4618 framework
*
* Implements a playable Pong game using joystick input,
* OpenCV rendering and GUI controls.
*
* @author Jass Saini
*
*/
class CPong : public CBase4618
{
public:

    /**
    *
    * @brief Constructor for CPong class.
    *
    * Initializes window, objects and game state.
    *
    */
    CPong();

    /**
    *
    * @brief Reads hardware inputs.
    *
    * Reads joystick and buttons from the controller.
    *
    * @return Nothing to return
    *
    */
    void gpio() override;

    /**
    *
    * @brief Serves the ball.
    *
    * Places ball in center and launches toward player or AI.
    *
    * @param direction Direction of serve (-1 left, 1 right)
    * @return Nothing to return
    *
    */
    void serve(int direction);

    /**
    *
    * @brief Updates game logic.
    *
    * Handles movement, collisions and scoring.
    *
    * @return Nothing to return
    *
    */
    void update() override;

    /**
    *
    * @brief Draws game to screen.
    *
    * Renders paddles, ball, score and GUI.
    *
    * @return Nothing to return
    *
    */
    void draw() override;

    /**
    *
    * @brief Checks if exit was requested.
    *
    * @return True when exit requested, false otherwise
    *
    */
    bool exit_requested() override;

private:
    int screen_w; ///< Window width in pixels
    int screen_h; ///< Window height in pixels

    float paddle_h = 140.0f; ///< Paddle height
    float paddle_w = 18.0f;  ///< Paddle width
    float player_y; ///< Player paddle vertical position
    float ai_y; ///< Computer paddle vertical position

    float ball_radius = 20.0f; ///< Ball radius
    cv::Point2f ball_pos; ///< Ball position
    cv::Point2f ball_vel; ///< Ball velocity

    int player_score = 0; ///< Player score
    int ai_score = 0; ///< Computer score

    double ui_radius = 20.0; ///< GUI controlled ball radius
    double ui_speed = 250.0; ///< GUI controlled ball speed
    float joystick_y = 0.5f; ///< Joystick vertical position (0-1)

    bool reset_requested = false; ///< Reset button pressed
    bool exit_requested_flag = false; ///< Exit button pressed

    float player_x; ///< Player paddle X position
    float ai_x; ///< Computer paddle X position
    float ai_vel = 0.0f; ///< Computer paddle velocity
    float speed_scale = 1.75f; ///< Internal speed multiplier

    bool game_over = false; ///< Game finished state
    std::string winner_text = ""; ///< Winner message

    bool prev_button_state = false; ///< Detecting previous state of pushbutton
};
