#include "stdafx.h"
#include "CSpaceInvadersGame.h"
#include <sstream>
#include <iomanip>
#include <windows.h>

#define WINDOW_NAME "Space Invaders by Jass Saini"
#define JOY_X 11
#define FIRE_PIN 32
#define RESET_PIN 33

CSpaceInvadersGame::CSpaceInvadersGame()
{
    cv::namedWindow(WINDOW_NAME);
    canvas() = cv::Mat(800, 1200, CV_8UC3);

    board().auto_connect();

    cv::Size board = canvas().size();

    _ship = new CShip(board);

    _joy = cv::Point2f(0.5f, 0.5f);

    _score = 0;
    _fire_flag = false;
    _reset_flag = false;
    _enemy_dir = 1;

    _show_fire = false;
    _show_reset = false;
    _fire_prev = false;
    _reset_prev = false;

    float spacing_x = board.width * 0.08f;
    float spacing_y = board.height * 0.07f;
    float start_x = board.width * 0.1f;
    float start_y = board.height * 0.1f;

    _enemy_fire_timer = 0.0f;
    _enemy_fire_interval = 2.0f;  

    _game_over = false;
    _game_over_text = "";

    for (int r = 0; r < 4; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            cv::Point2f pos(
                start_x + c * spacing_x,
                start_y + r * spacing_y
            );

            _invaders.push_back(CInvader(board, pos));
        }
    }

    _exit = false;
    cv::setMouseCallback(WINDOW_NAME, mouse_callback, this);
}

CSpaceInvadersGame::~CSpaceInvadersGame()
{
    delete _ship;
}

void CSpaceInvadersGame::mouse_callback(int event, int x, int y, int flags, void* userdata)
{
    if (event != cv::EVENT_LBUTTONDOWN)
        return;

    CSpaceInvadersGame* game =
        reinterpret_cast<CSpaceInvadersGame*>(userdata);

    if (x >= 20 && x <= 140 && y >= 50 && y <= 90)
        game->_reset_flag = true;

    if (x >= 160 && x <= 280 && y >= 50 && y <= 90)
        game->_exit = true;
}

bool CSpaceInvadersGame::exit_requested()
{
    return _exit;
}

void CSpaceInvadersGame::gpio()
{
    float percent;

    if (board().get_analog(JOY_X, percent))
        _joy.x = percent;

    bool pressed;
    if (board().get_button(FIRE_PIN, pressed))
    {
        if (pressed && !_fire_prev)  
        {
            _fire_flag = true;
            std::cout << "FIRE\n";
        }

        _fire_prev = pressed;
    }

    if (board().get_button(RESET_PIN, pressed))
    {
        if (pressed && !_reset_prev)   
        {
            _reset_flag = true;
            std::cout << "RESET\n";
        }

        _reset_prev = pressed;
    }
}

void CSpaceInvadersGame::update()
{
    if (_reset_flag)
    {
        cv::Size board = canvas().size();

        _invaders.clear();
        _missiles.clear();
        _enemy_missiles.clear();

        _score = 0;
        _enemy_dir = 1;
        _enemy_fire_timer = 0.0f;

        _ship->set_lives(3);

        _game_over = false;
        _game_over_text = "";

        float spacing_x = board.width * 0.08f;
        float spacing_y = board.height * 0.07f;
        float start_x = board.width * 0.1f;
        float start_y = board.height * 0.1f;

        for (int r = 0; r < 4; r++)
            for (int c = 0; c < 8; c++)
                _invaders.push_back(CInvader(board,
                    cv::Point2f(start_x + c * spacing_x,
                        start_y + r * spacing_y)));

        _reset_flag = false;
    }

    if (_game_over)
        return;

    float delta = (float)dt();
    cv::Size boardSize = canvas().size();

    // Ship movement with joystick & keyboard
    int key = cv::waitKey(1);

    float ship_speed = boardSize.width * 0.6f;
    cv::Point2f ship_pos = _ship->get_pos();

    // Joystick 
    float joy = (_joy.x - 0.5f) * 2.0f;
    const float deadzone = 0.15f;
    if (fabs(joy) < deadzone)
        joy = 0.0f;

    ship_pos.x += joy * ship_speed * delta;

    // Keyboard continuous movement
    if (GetAsyncKeyState('A') & 0x8000)
        ship_pos.x -= ship_speed * delta;

    if (GetAsyncKeyState('D') & 0x8000)
        ship_pos.x += ship_speed * delta;

    // Spacebar firing 
    bool space_now = (GetAsyncKeyState(VK_SPACE) & 0x8000) != 0;

    if (space_now && !_space_prev)
    {
        _fire_flag = true;   
    }
    _space_prev = space_now;

    float ship_width = _ship->get_width();

    if (ship_pos.x < 0)
        ship_pos.x = 0;

    if (ship_pos.x + ship_width > boardSize.width)
        ship_pos.x = boardSize.width - ship_width;

    _ship->set_pos(ship_pos);
    
    // Enemy movement
    float enemy_speed = boardSize.width * 0.1f;

    for (auto& inv : _invaders)
    {
        cv::Point2f pos = inv.get_pos();
        pos.x += _enemy_dir * enemy_speed * delta;
        inv.set_pos(pos);
    }

    bool hitEdge = false;

    for (auto& inv : _invaders)
        if (inv.collide_wall(boardSize))
            hitEdge = true;

    if (hitEdge)
    {
        _enemy_dir *= -1;
        for (auto& inv : _invaders)
        {
            cv::Point2f pos = inv.get_pos();
            pos.y += boardSize.height * 0.05f;
            inv.set_pos(pos);
        }
    }

    for (auto& inv : _invaders)
        inv.move(delta);

    // Enemy fire
    _enemy_fire_timer += 5.0f * delta;

    if (_enemy_fire_timer >= _enemy_fire_interval && !_invaders.empty())
    {
        _enemy_fire_timer = 0.0f;

        int index = rand() % _invaders.size();
        cv::Point2f pos = _invaders[index].get_pos();
        cv::Point2f vel(0, boardSize.height * 0.4f);

        _enemy_missiles.push_back(
            CMissile(boardSize, pos, vel, cv::Scalar(0, 0, 255))
        );
    }

    // Player fire
    if (_fire_flag)
    {
        cv::Point2f pos = _ship->get_pos();
        cv::Point2f vel(0, -boardSize.height * 0.6f);

        _missiles.push_back(
            CMissile(boardSize, pos, vel, cv::Scalar(255, 255, 255))
        );

        _fire_flag = false;
    }

    // Missile movement 
    for (auto& m : _missiles) m.move(delta);
    for (auto& m : _enemy_missiles) m.move(delta);

    // Collisions
    for (auto& m : _missiles)
        for (auto& inv : _invaders)
            if (m.collide(inv))
            {
                m.hit();
                inv.hit();
                _score += 10;
            }

    for (auto& m : _enemy_missiles)
        if (m.collide(*_ship))
        {
            m.hit();
            _ship->hit();
        }

    _missiles.erase(std::remove_if(_missiles.begin(), _missiles.end(),
        [](CMissile& m) { return m.get_lives() <= 0; }),
        _missiles.end());

    _invaders.erase(std::remove_if(_invaders.begin(), _invaders.end(),
        [](CInvader& i) { return i.get_lives() <= 0; }),
        _invaders.end());

    _enemy_missiles.erase(std::remove_if(_enemy_missiles.begin(), _enemy_missiles.end(),
        [](CMissile& m) { return m.get_lives() <= 0; }),
        _enemy_missiles.end());

    // Game Logic 
    if (_ship->get_lives() <= 0 && !_game_over)
    {
        _game_over = true;
        _game_over_text = "YOU LOST!";
    }

    if (_invaders.empty() && !_game_over)
    {
        _game_over = true;
        _game_over_text = "YOU WIN!";
    }

    float ship_top = _ship->get_pos().y;

    for (auto& inv : _invaders)
    {
        float inv_bottom = inv.get_pos().y + inv.get_height();
        if (inv_bottom >= ship_top - 10)
        {
            _game_over = true;
            _game_over_text = "YOU LOST!";
            break;
        }
    }
}

void CSpaceInvadersGame::draw()
{
    canvas().setTo(cv::Scalar(0, 0, 0));

    std::ostringstream ss;
    ss << std::fixed << std::setprecision(2) << fps();

    std::string info =
        "FPS: " + ss.str() +
        "   Score: " + std::to_string(_score) +
        "   Lives: " + std::to_string(_ship->get_lives()) +
        "   Missiles: " + std::to_string(_missiles.size());

    cv::putText(canvas(),
        info,
        cv::Point(20, 30),
        cv::FONT_HERSHEY_SIMPLEX,
        0.8,
        cv::Scalar(255, 255, 255),
        2);

    _ship->draw(canvas());

    for (auto& inv : _invaders)
        inv.draw(canvas());

    for (auto& m : _missiles)
        m.draw(canvas());
    
    for (auto& m : _enemy_missiles)
        m.draw(canvas());

    if (_game_over)
    {
        int w = 400;
        int h = 150;

        int cx = canvas().cols / 2 - w / 2;
        int cy = canvas().rows / 2 - h / 2;

        cv::rectangle(canvas(),
            cv::Rect(cx, cy, w, h),
            cv::Scalar(50, 50, 50),
            -1);

        cv::rectangle(canvas(),
            cv::Rect(cx, cy, w, h),
            cv::Scalar(255, 255, 255),
            2);

        cv::putText(canvas(),
            _game_over_text,
            cv::Point(cx + 100, cy + 60),
            cv::FONT_HERSHEY_SIMPLEX,
            1.0,
            cv::Scalar(255, 255, 255),
            2);

        cv::putText(canvas(),
            "Press RESET to play again",
            cv::Point(cx + 60, cy + 100),
            cv::FONT_HERSHEY_SIMPLEX,
            0.6,
            cv::Scalar(255, 255, 255),
            1);
    }

    // Reset button
    cv::Rect resetBtn(20, 50, 120, 40);
    cv::rectangle(canvas(), resetBtn, cv::Scalar(80, 80, 80), -1);
    cv::putText(canvas(), "RESET",
        cv::Point(40, 75),
        cv::FONT_HERSHEY_SIMPLEX,
        0.6,
        cv::Scalar(255, 255, 255), 2);

    // Exit button
    cv::Rect exitBtn(160, 50, 120, 40);
    cv::rectangle(canvas(), exitBtn, cv::Scalar(80, 80, 80), -1);
    cv::putText(canvas(), "EXIT",
        cv::Point(190, 75),
        cv::FONT_HERSHEY_SIMPLEX,
        0.6,
        cv::Scalar(255, 255, 255), 2);

    cv::imshow(WINDOW_NAME, canvas());

    _show_fire = false;
    _show_reset = false;

    int key = cv::waitKey(1);
    _last_key = key;
}