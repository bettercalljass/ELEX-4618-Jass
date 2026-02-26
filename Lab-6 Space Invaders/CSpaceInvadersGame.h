#pragma once
#include "CBase4618.h"
#include "CShip.h"
#include "CInvader.h"
#include "CMissile.h"
#include <vector>

/**
* @class CSpaceInvadersGame
* @brief Main Space Invaders game class
*
* Derived from CBase4618.
*
* Handles game logic, input and rendering.
*
* @author Jass Saini
*
*/
class CSpaceInvadersGame : public CBase4618
{
private:
    CShip* _ship; ///< Player ship
    std::vector<CInvader> _invaders; ///< Enemy invaders
    std::vector<CMissile> _missiles; ///< Player missiles

    int _score; ///< Player score
    bool _fire_flag; ///< Fire button flag
    bool _reset_flag; ///< Reset button flag
    int _enemy_dir; ///< Enemy movement direction
    cv::Point2f _joy; ///< Joystick position
    bool _show_fire; ///< GUI fire button state
    bool _show_reset; ///< GUI reset button state
    bool _fire_prev; ///< Previous fire state
    bool _reset_prev; ///< Previous reset state

    std::vector<CMissile> _enemy_missiles; ///< Enemy missiles

    float _enemy_fire_timer; ///< Enemy fire timer
    float _enemy_fire_interval; ///< Enemy fire interval

    bool _game_over; ///< Game over flag
    std::string _game_over_text; ///< Game over message
    bool _exit; ///< Indicates if exit was requested
    virtual bool exit_requested() override; ///< Exits the game
    
    int _last_key = -1; //< Stores the most recent keyboard key code detected
    bool _space_prev = false; //< Tracks previous frame spacebar state for single-shot firing

    /**
    *
    * @brief Mouse callback function.
    *
    * @return Nothing to return
    *
    */
    static void mouse_callback(int event, int x, int y, int flags, void* userdata);

public:

    /**
    *
    * @brief Constructor for CSpaceInvadersGame class.
    *
    */
    CSpaceInvadersGame();

    /**
    *
    * @brief Destructor for CSpaceInvadersGame class.
    *
    */
    ~CSpaceInvadersGame();

    /**
    *
    * @brief Handles hardware IO step.
    *
    * @return Nothing to return
    *
    */
    void gpio() override;

    /**
    *
    * @brief Updates game logic.
    *
    * @return Nothing to return
    *
    */
    void update() override;

    /**
    *
    * @brief Renders frame to screen.
    *
    * @return Nothing to return
    *
    */
    void draw() override;
};