#pragma once

#include "CBase4618.h"
#include <string>
#include <vector>

/**
* @class CRecyclingSort
* @brief Recycling sorter class for Lab 8
*
* 4-color / 4-bin version:
* GREEN, BLUE, PINK, YELLOW
*
* @author Jass Saini
*
*/
class CRecyclingSort : public CBase4618
{
private:
    // Camera
    cv::VideoCapture _cam;
    cv::Mat _frame;
    cv::Mat _display;
    cv::Mat _roi;
    cv::Mat _hsv;
    cv::Mat _mask_display;
    cv::Rect _detect_roi;
    bool _camera_ok;

    // Camera recovery
    int _camera_fail_count;
    double _camera_retry_timer;
    double _camera_retry_delay;

    // Servo channels
    int _ramp_servo;
    int _feed_servo;
    int _drop_servo;

    // Ramp angles
    int _ramp_bin1;
    int _ramp_bin2;
    int _ramp_bin3;
    int _ramp_bin4;
    int _ramp_home;

    // Gate angles
    int _feed_closed;
    int _feed_open;
    int _drop_closed;
    int _drop_open;

    bool _feed_active;
    double _feed_timer;
    double _feed_time;

    bool _restage_delay_active;
    double _restage_delay_timer;
    double _restage_delay_time;

    bool _ball_staged;

    // Current output angles
    int _ramp_angle;
    int _feed_angle;
    int _drop_angle;

    // Last sent servo angles
    int _last_ramp_angle_sent;
    int _last_feed_angle_sent;
    int _last_drop_angle_sent;

    // Startup homing / settle
    bool _startup_init_done;
    double _startup_timer;
    double _startup_settle_time;

    // Modes / state
    bool _auto_mode;
    bool _exit_requested;
    bool _release_active;
    bool _sort_in_progress;
    bool _ball_present;
    bool _ball_present_prev;

    double _release_timer;
    double _release_time;
    double _sort_timer;
    double _sort_hold_time;

    // Detection result
    std::string _status_text;
    std::string _detected_color;
    int _target_bin;
    int _largest_area;
    cv::Rect _largest_box;

    // Counters
    int _bin1_count;
    int _bin2_count;
    int _bin3_count;
    int _bin4_count;

    // Debug colour selection
    int _debug_color_index;   // 0=green, 1=blue, 2=pink, 3=yellow

    // HSV hue thresholds
    int _green_h_low;
    int _green_h_high;

    int _blue_h_low;
    int _blue_h_high;

    int _pink_h_low;
    int _pink_h_high;

    int _yellow_h_low;
    int _yellow_h_high;

    // Physical button previous states
    bool _mode_btn_prev;
    bool _release_btn_prev;
    bool _bin1_btn_prev;
    bool _bin2_btn_prev;
    bool _bin3_btn_prev;
    bool _bin4_btn_prev;

    // LED timers
    double _bin1_led_timer;
    double _bin2_led_timer;
    double _bin3_led_timer;
    double _bin4_led_timer;
    double _bin_led_flash_time;

    void trigger_bin1();
    void trigger_bin2();
    void trigger_bin3();
    void trigger_bin4();

    bool _auto_mode_prev;

    int _auto_candidate_bin;
    double _auto_candidate_timer;
    double _auto_candidate_time;
    bool _auto_drop_done;

    int _auto_release_count;
    int _startup_release_count;
    double _release_time_startup;
    double _release_time_normal;

public:
    CRecyclingSort();
    virtual ~CRecyclingSort();

    void gpio() override;
    void update() override;
    void draw() override;
    bool exit_requested() override;

    void process_camera();
    bool detect_color_mask(const cv::Mat& hsv, const std::string& color_name,
        cv::Mat& out_mask, int& best_area, cv::Rect& best_box);
    int color_to_bin(const std::string& color_name);

    void set_ramp_angle(int angle);

    void sort_bin1();
    void sort_bin2();
    void sort_bin3();
    void sort_bin4();
    void go_home();

    void feed_open();
    void feed_close();

    void drop_open();
    void drop_close();

    void stage_one();
    void release_staged_ball();
    void reset_counts();

    int& current_h_low();
    int& current_h_high();
    std::string current_debug_color_name();
};
