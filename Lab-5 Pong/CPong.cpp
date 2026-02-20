#include "stdafx.h"
#include "CPong.h"
#include "cvui.h"
#include "CControl.h"
#include <sstream>
#include <iomanip>
#include <cmath>
#include <ctime>

#define WINDOW_NAME "Pong"
#define JOY_Y 4
#define DigitalPin 32

CPong::CPong()
{
    screen_w = 1100;
    screen_h = 700;

    cv::namedWindow(WINDOW_NAME, cv::WINDOW_NORMAL);
    cv::resizeWindow(WINDOW_NAME, screen_w, screen_h);

    canvas() = cv::Mat(screen_h, screen_w, CV_8UC3);

    player_y = screen_h / 2.0f;
    ai_y = screen_h / 2.0f;
    ball_pos = cv::Point2f(screen_w / 2.0f, screen_h / 2.0f);
    ai_x = 20.0f;
    player_x = screen_w - 20.0f - paddle_w;

    cvui::init(WINDOW_NAME);

    board().auto_connect();

    ball_vel = cv::Point2f(250.0f, 160.0f);

    srand((unsigned)time(NULL));
    serve((rand() % 2) ? 1 : -1);
}

void CPong::gpio()
{
    float percent;

    if (board().get_analog(JOY_Y, percent))
    {
        joystick_y = percent;
    }

    bool button_state = false;

    if (board().get_button(DigitalPin, button_state))
    {
        if (button_state && !prev_button_state)
        {
            reset_requested = true;
        }

        prev_button_state = button_state;
    }
}

void CPong::serve(int direction)
{
    ball_pos = cv::Point2f(screen_w / 2.0f, screen_h / 2.0f);

    float speed = (float)ui_speed * speed_scale;

    float angle = ((float)rand() / RAND_MAX) * 60.0f - 30.0f;
    angle *= 3.1415926f / 180.0f; // deg to rad

    ball_vel.x = direction * speed * cos(angle);
    ball_vel.y = speed * sin(angle);
}

void CPong::update()
{
    if (dt() > 0.1)
        return;

    if (reset_requested)
    {
        player_score = 0;
        ai_score = 0;

        ui_speed = 250.0;
        ui_radius = 20.0;

        game_over = false;
        winner_text = "";

        serve((rand() % 2) ? 1 : -1);

        reset_requested = false;
    }

    if (game_over)
        return;

    float len = sqrt(ball_vel.x * ball_vel.x + ball_vel.y * ball_vel.y);

    if (len > 0.001f)
    {
        float speed = (float)ui_speed * speed_scale;

        ball_vel.x = (ball_vel.x / len) * speed;
        ball_vel.y = (ball_vel.y / len) * speed;
    }

    float joy = (joystick_y - 0.5f) * 2.0f;

    const float deadzone = 0.2f; // DEADZONE scalar

    if (fabs(joy) < deadzone)
        joy = 0.0f;
    else
    {
        if (joy > 0)
            joy = (joy - deadzone) / (1.0f - deadzone);
        else
            joy = (joy + deadzone) / (1.0f - deadzone);
    }

    player_y -= joy * 650.0f * dt();

    float half = paddle_h / 2.0f;

    if (player_y < half)
        player_y = half;

    if (player_y > screen_h - half)
        player_y = screen_h - half;

    if (ball_vel.x < 0)
    {
        float target = ball_pos.y;
        float error = target - ai_y;

        float stiffness = 7.0f;   
        float damping = 0.90f;  

        ai_vel += error * stiffness * dt();
        ai_vel *= damping;
    }

    float max_speed = 250.0f; //difficulty level 250-easy, 280-medium, 350-hard
    if (ai_vel > max_speed)  ai_vel = max_speed;
    if (ai_vel < -max_speed) ai_vel = -max_speed;

    ai_y += ai_vel * dt();

    float ai_half = paddle_h / 2.0f;

    if (ai_y < ai_half)
    {
        ai_y = ai_half;
        if (ai_vel < 0) ai_vel = 0;
    }

    if (ai_y > screen_h - ai_half)
    {
        ai_y = screen_h - ai_half;
        if (ai_vel > 0) ai_vel = 0;
    }

    ball_pos += ball_vel * (float)dt();

    if (ball_pos.y - ui_radius <= 0)
    {
        ball_pos.y = ui_radius;
        ball_vel.y *= -1;
    }

    if (ball_pos.y + ui_radius >= screen_h)
    {
        ball_pos.y = screen_h - ui_radius;
        ball_vel.y *= -1;
    }

    //ball boundary
    float b_left = ball_pos.x - ui_radius;
    float b_right = ball_pos.x + ui_radius;
    float b_top = ball_pos.y - ui_radius;
    float b_bottom = ball_pos.y + ui_radius;

    // player paddle boundary
    float p_left = player_x;
    float p_right = player_x + paddle_w;
    float p_top = player_y - paddle_h / 2;
    float p_bottom = player_y + paddle_h / 2;

    // computer paddle boundary
    float a_left = ai_x;
    float a_right = ai_x + paddle_w;
    float a_top = ai_y - paddle_h / 2;
    float a_bottom = ai_y + paddle_h / 2;


    if (b_right >= p_left && b_left <= p_right &&
        b_bottom >= p_top && b_top <= p_bottom &&
        ball_vel.x > 0)
    {
        ball_pos.x = p_left - ui_radius;
        ball_vel.x *= -1;
    }

    if (b_left <= a_right && b_right >= a_left &&
        b_bottom >= a_top && b_top <= a_bottom &&
        ball_vel.x < 0)
    {
        ball_pos.x = a_right + ui_radius;
        ball_vel.x *= -1;
    }

    // computer miss
    if (ball_pos.x + ui_radius < 0)
    {
        player_score++;      
        serve(-1);            
        return;
    }

    // player miss
    if (ball_pos.x - ui_radius > screen_w)
    {
        ai_score++;          
        serve(1);           
        return;
    }

    if (player_score >= 5)
    {
        game_over = true;
        winner_text = "WINNER: PLAYER";
    }

    if (ai_score >= 5)
    {
        game_over = true;
        winner_text = "WINNER: COMPUTER";
    }
}

void CPong::draw()
{
    canvas().setTo(cv::Scalar(0, 0, 0));

    cv::line(canvas(),
        cv::Point(screen_w / 2, 0),
        cv::Point(screen_w / 2, screen_h),
        cv::Scalar(255, 255, 255), 2);

    cv::rectangle(canvas(),
        cv::Point(ai_x, ai_y - paddle_h / 2),
        cv::Point(ai_x + paddle_w, ai_y + paddle_h / 2),
        cv::Scalar(255, 255, 255), -1);

    cv::rectangle(canvas(),
        cv::Point(player_x, player_y - paddle_h / 2),
        cv::Point(player_x + paddle_w, player_y + paddle_h / 2),
        cv::Scalar(255, 255, 255), -1);

    cv::circle(canvas(), ball_pos, (int)ui_radius, cv::Scalar(255, 255, 255), -1);

    int x = 60;
    int y = 20;

    cvui::window(canvas(), x, y, 220, 300, "Pong");

    int yy = y + 30;

    std::ostringstream ss;
    ss << std::fixed << std::setprecision(3) << fps();
    cvui::text(canvas(), x + 10, yy, "FPS: " + ss.str());

    yy += 25;
    cvui::text(canvas(), x + 10, yy,
        "Player: " + std::to_string(player_score) +
        "   Computer: " + std::to_string(ai_score));

    yy += 40;
    cvui::text(canvas(), x + 10, yy, "Radius");
    yy += 15;
    cvui::trackbar(canvas(), x + 10, yy, 200, &ui_radius, 5.0, 100.0);

    yy += 60;
    cvui::text(canvas(), x + 10, yy, "Speed");
    yy += 15;
    cvui::trackbar(canvas(), x + 10, yy, 200, &ui_speed, 100.0, 400.0);

    yy += 60;
    if (cvui::button(canvas(), x + 10, yy, 90, 30, "Reset"))
        reset_requested = true;

    if (cvui::button(canvas(), x + 110, yy, 90, 30, "Exit"))
        exit_requested_flag = true;

    if (game_over)
    {
        int w = 320;
        int h = 140;
        int cx = screen_w / 2 - w / 2;
        int cy = screen_h / 2 - h / 2;

        cvui::window(canvas(), cx, cy, w, h, "Game Over");

        cvui::text(canvas(), cx + 40, cy + 60, winner_text, 0.8);
        cvui::text(canvas(), cx + 40, cy + 95, "Press RESET to play again", 0.5);
    }

    cvui::update();
    
    std::string title = "Pong by Jass Saini";
    cv::setWindowTitle(WINDOW_NAME, title);

    cv::imshow(WINDOW_NAME, canvas());
}

bool CPong::exit_requested()
{
    return exit_requested_flag;
}
