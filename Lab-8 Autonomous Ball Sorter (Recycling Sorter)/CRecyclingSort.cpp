#include "CRecyclingSort.h"
#include "cvui.h"

#define CANVAS_NAME "Display Image"

#define RAMP_SERVO_CHANNEL 18
#define DROP_SERVO_CHANNEL 19
#define FEED_SERVO_CHANNEL 20

// Pushbuttons
#define BTN_MODE     17
#define BTN_RELEASE  27
#define BTN_BIN1     22
#define BTN_BIN2     23
#define BTN_BIN3     24
#define BTN_BIN4     25

// Mode LED
#define MODE_LED      7

// Bin event LEDs
#define LED_BIN1      6
#define LED_BIN2     12
#define LED_BIN3     13
#define LED_BIN4     16

CRecyclingSort::CRecyclingSort()
{
    _ramp_servo = RAMP_SERVO_CHANNEL;
    _feed_servo = FEED_SERVO_CHANNEL;
    _drop_servo = DROP_SERVO_CHANNEL;

    _ramp_bin1 = 30;
    _ramp_bin2 = 70;
    _ramp_bin3 = 115;
    _ramp_bin4 = 150;
    _ramp_home = 85;

    _feed_closed = 90;
    _feed_open = 170;

    _drop_closed = 90;
    _drop_open = 40;

    _ramp_angle = _ramp_home;
    _feed_angle = _feed_closed;
    _drop_angle = _drop_closed;

    _last_ramp_angle_sent = -999;
    _last_feed_angle_sent = -999;
    _last_drop_angle_sent = -999;

    _startup_init_done = false;
    _startup_timer = 0.0;
    _startup_settle_time = 0.6;

    _auto_mode = false;
    _exit_requested = false;
    _sort_in_progress = false;
    _ball_present = false;
    _ball_present_prev = false;

    _release_active = false;
    _release_timer = 0.0;
    _release_time = 1.0;

    _restage_delay_active = false;
    _restage_delay_timer = 0.0;
    _restage_delay_time = 0.5;

    _feed_active = false;
    _feed_timer = 0.0;
    _feed_time = 0.25;

    _ball_staged = false;

    _sort_timer = 0.0;
    _sort_hold_time = 0.6;

    _status_text = "MANUAL";
    _detected_color = "NONE";
    _target_bin = 0;
    _largest_area = 0;
    _largest_box = cv::Rect(0, 0, 0, 0);

    _bin1_count = 0;
    _bin2_count = 0;
    _bin3_count = 0;
    _bin4_count = 0;

    _debug_color_index = 0;

    _green_h_low = 35;
    _green_h_high = 85;

    _blue_h_low = 90;
    _blue_h_high = 130;

    _pink_h_low = 140;
    _pink_h_high = 179;

    _yellow_h_low = 20;
    _yellow_h_high = 35;

    _camera_ok = false;
    _camera_fail_count = 0;
    _camera_retry_timer = 0.0;
    _camera_retry_delay = 1.0;

    _mode_btn_prev = false;
    _release_btn_prev = false;
    _bin1_btn_prev = false;
    _bin2_btn_prev = false;
    _bin3_btn_prev = false;
    _bin4_btn_prev = false;

    _bin1_led_timer = 0.0;
    _bin2_led_timer = 0.0;
    _bin3_led_timer = 0.0;
    _bin4_led_timer = 0.0;
    _bin_led_flash_time = 0.35;

    _auto_mode_prev = false;

    _auto_candidate_bin = 0;
    _auto_candidate_timer = 0.0;
    _auto_candidate_time = 1.5;
    _auto_drop_done = false;

    _auto_release_count = 0;
    _startup_release_count = 0;
    _release_time_startup = 0.0;
    _release_time_normal = 0.0;

    _detect_roi = cv::Rect(270, 160, 60, 40);

    canvas() = cv::Mat(760, 1280, CV_8UC3, cv::Scalar(40, 40, 40));
    cvui::init(CANVAS_NAME);

    _cam.open(0, cv::CAP_V4L2);

    if (_cam.isOpened())
    {
        _cam.set(cv::CAP_PROP_FRAME_WIDTH, 640);
        _cam.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
        _cam.set(cv::CAP_PROP_BUFFERSIZE, 1);

        _camera_ok = true;
        std::cout << "Camera opened on /dev/video0\n";
    }
    else
    {
        std::cout << "Camera failed to open on /dev/video0\n";
    }
}

CRecyclingSort::~CRecyclingSort()
{
    board().set_data(Servo, _ramp_servo, _ramp_home);
    board().set_data(Servo, _feed_servo, _feed_closed);
    board().set_data(Servo, _drop_servo, _drop_closed);

    board().set_data(Digital, MODE_LED, 0);
    board().set_data(Digital, LED_BIN1, 0);
    board().set_data(Digital, LED_BIN2, 0);
    board().set_data(Digital, LED_BIN3, 0);
    board().set_data(Digital, LED_BIN4, 0);

    if (_cam.isOpened())
    {
        _cam.release();
    }
}

void CRecyclingSort::gpio()
{
    if (!_startup_init_done)
    {
        go_home();
        feed_close();
        drop_close();

        _feed_active = false;
        _feed_timer = 0.0;
        _release_active = false;
        _release_timer = 0.0;
        _sort_in_progress = false;
        _sort_timer = 0.0;
        _ball_staged = false;
        _restage_delay_active = false;
        _restage_delay_timer = 0.0;

        _status_text = "STARTUP HOMING";
        _startup_init_done = true;
    }

    if (_ramp_angle != _last_ramp_angle_sent)
    {
        board().set_data(Servo, _ramp_servo, _ramp_angle);
        _last_ramp_angle_sent = _ramp_angle;
    }

    if (_feed_angle != _last_feed_angle_sent)
    {
        board().set_data(Servo, _feed_servo, _feed_angle);
        _last_feed_angle_sent = _feed_angle;
    }

    if (_drop_angle != _last_drop_angle_sent)
    {
        board().set_data(Servo, _drop_servo, _drop_angle);
        _last_drop_angle_sent = _drop_angle;
    }

    board().set_data(Digital, MODE_LED, _auto_mode ? 1 : 0);

    board().set_data(Digital, LED_BIN1, _bin1_led_timer > 0.0 ? 1 : 0);
    board().set_data(Digital, LED_BIN2, _bin2_led_timer > 0.0 ? 1 : 0);
    board().set_data(Digital, LED_BIN3, _bin3_led_timer > 0.0 ? 1 : 0);
    board().set_data(Digital, LED_BIN4, _bin4_led_timer > 0.0 ? 1 : 0);

    bool mode_btn = false;
    bool release_btn = false;
    bool bin1_btn = false;
    bool bin2_btn = false;
    bool bin3_btn = false;
    bool bin4_btn = false;

    board().get_button(BTN_MODE, mode_btn);
    board().get_button(BTN_RELEASE, release_btn);
    board().get_button(BTN_BIN1, bin1_btn);
    board().get_button(BTN_BIN2, bin2_btn);
    board().get_button(BTN_BIN3, bin3_btn);
    board().get_button(BTN_BIN4, bin4_btn);

    if (_startup_timer >= _startup_settle_time)
    {
        if (mode_btn && !_mode_btn_prev)
        {
            _auto_mode = !_auto_mode;
            _status_text = _auto_mode ? "AUTO" : "MANUAL";
        }

        if (!_auto_mode && release_btn && !_release_btn_prev)
        {
            if (_ball_staged && !_release_active && !_feed_active)
                release_staged_ball();
        }

        if (!_auto_mode)
        {
            if (bin1_btn && !_bin1_btn_prev)
                trigger_bin1();

            if (bin2_btn && !_bin2_btn_prev)
                trigger_bin2();

            if (bin3_btn && !_bin3_btn_prev)
                trigger_bin3();

            if (bin4_btn && !_bin4_btn_prev)
                trigger_bin4();
        }
    }

    _mode_btn_prev = mode_btn;
    _release_btn_prev = release_btn;
    _bin1_btn_prev = bin1_btn;
    _bin2_btn_prev = bin2_btn;
    _bin3_btn_prev = bin3_btn;
    _bin4_btn_prev = bin4_btn;
}

void CRecyclingSort::update()
{
    if (_startup_timer < _startup_settle_time)
        _startup_timer += dt();

    process_camera();

    if (_bin1_led_timer > 0.0) _bin1_led_timer -= dt();
    if (_bin2_led_timer > 0.0) _bin2_led_timer -= dt();
    if (_bin3_led_timer > 0.0) _bin3_led_timer -= dt();
    if (_bin4_led_timer > 0.0) _bin4_led_timer -= dt();

    if (_bin1_led_timer < 0.0) _bin1_led_timer = 0.0;
    if (_bin2_led_timer < 0.0) _bin2_led_timer = 0.0;
    if (_bin3_led_timer < 0.0) _bin3_led_timer = 0.0;
    if (_bin4_led_timer < 0.0) _bin4_led_timer = 0.0;

    if (_auto_mode != _auto_mode_prev)
    {
        _status_text = _auto_mode ? "AUTO" : "MANUAL";
        _sort_in_progress = false;
        _sort_timer = 0.0;
        _release_active = false;
        _release_timer = 0.0;
        _feed_active = false;
        _feed_timer = 0.0;
        feed_close();
        drop_close();
        go_home();

        _auto_candidate_bin = 0;
        _auto_candidate_timer = 0.0;
        _auto_drop_done = false;

        _auto_release_count = 0;
        _ball_staged = false;
        _auto_mode_prev = _auto_mode;

        _restage_delay_active = false;
        _restage_delay_timer = 0.0;
    }

    if (_feed_active)
    {
        _feed_timer += dt();

        if (_feed_timer >= _feed_time)
        {
            feed_close();
            _feed_active = false;
            _feed_timer = 0.0;
            _ball_staged = true;
            _status_text = "BALL STAGED";
        }
    }

    if (_release_active)
    {
        _release_timer += dt();

        if (_release_timer >= _release_time)
        {
            drop_close();
            _release_active = false;
            _release_timer = 0.0;
            _ball_staged = false;

            _restage_delay_active = true;
            _restage_delay_timer = 0.0;

            _status_text = _auto_mode ? "AUTO" : "MANUAL";
        }
    }

    if (_restage_delay_active)
    {
        _restage_delay_timer += dt();

        if (_restage_delay_timer >= _restage_delay_time)
        {
            _restage_delay_active = false;
            _restage_delay_timer = 0.0;
        }
    }

    if (_sort_in_progress)
    {
        _sort_timer += dt();

        if (_auto_mode && !_auto_drop_done && !_release_active && _sort_timer >= 0.25)
        {
            release_staged_ball();
            _auto_drop_done = true;
            _status_text = "AUTO DROP";
        }

        if (_sort_timer >= _sort_hold_time && !_release_active)
        {
            go_home();
            _sort_in_progress = false;
            _sort_timer = 0.0;
            _auto_drop_done = false;
            _auto_candidate_bin = 0;
            _auto_candidate_timer = 0.0;
            _status_text = _auto_mode ? "AUTO" : "MANUAL";
        }
    }

    if (!_ball_staged && !_feed_active && !_release_active && !_restage_delay_active)
    {
        stage_one();
        _status_text = "STAGING BALL";
    }

    if (_auto_mode && !_feed_active && !_release_active && !_sort_in_progress && _ball_staged)
    {
        if (_ramp_angle == _ramp_home)
        {
            if (_ball_present && _target_bin >= 1 && _target_bin <= 4)
            {
                if (_target_bin == _auto_candidate_bin)
                {
                    _auto_candidate_timer += dt();
                }
                else
                {
                    _auto_candidate_bin = _target_bin;
                    _auto_candidate_timer = 0.0;
                }

                _status_text = "AUTO STABILIZING";

                if (_auto_candidate_timer >= _auto_candidate_time)
                {
                    if (_auto_candidate_bin == 1)
                        trigger_bin1();
                    else if (_auto_candidate_bin == 2)
                        trigger_bin2();
                    else if (_auto_candidate_bin == 3)
                        trigger_bin3();
                    else if (_auto_candidate_bin == 4)
                        trigger_bin4();

                    _auto_candidate_bin = 0;
                    _auto_candidate_timer = 0.0;
                    _auto_drop_done = false;
                }
            }
            else
            {
                _auto_candidate_bin = 0;
                _auto_candidate_timer = 0.0;
                _status_text = "WAITING FOR STAGED BALL";
            }
        }
    }

    _ball_present_prev = _ball_present;
}

void CRecyclingSort::draw()
{
    canvas() = cv::Scalar(40, 40, 40);

    if (!_display.empty())
    {
        cv::Mat frame_small;
        cv::resize(_display, frame_small, cv::Size(640, 480));
        frame_small.copyTo(canvas()(cv::Rect(20, 20, 640, 480)));
    }

    if (!_mask_display.empty())
    {
        cv::Mat mask_bgr;
        cv::cvtColor(_mask_display, mask_bgr, cv::COLOR_GRAY2BGR);
        cv::resize(mask_bgr, mask_bgr, cv::Size(280, 210));
        mask_bgr.copyTo(canvas()(cv::Rect(680, 20, 280, 210)));
    }

    cvui::window(canvas(), 680, 250, 280, 180, "System");
    cvui::checkbox(canvas(), 700, 285, "AUTO MODE", &_auto_mode);
    cvui::text(canvas(), 700, 320, "Status: " + _status_text);
    cvui::text(canvas(), 700, 350, "Detected: " + _detected_color);
    cvui::text(canvas(), 700, 380, "Camera: " + std::string(_camera_ok ? "OK" : "FAIL"));

    cvui::window(canvas(), 980, 20, 260, 210, "Manual Control");

    if (!_auto_mode && cvui::button(canvas(), 1000, 60, 100, 35, "BIN 1"))
        trigger_bin1();

    if (!_auto_mode && cvui::button(canvas(), 1120, 60, 100, 35, "BIN 2"))
        trigger_bin2();

    if (!_auto_mode && cvui::button(canvas(), 1000, 110, 100, 35, "BIN 3"))
        trigger_bin3();

    if (!_auto_mode && cvui::button(canvas(), 1120, 110, 100, 35, "BIN 4"))
        trigger_bin4();

    if (!_auto_mode && cvui::button(canvas(), 1000, 160, 100, 30, "Release"))
    {
        if (!_ball_staged)
            stage_one();
        else
            release_staged_ball();
    }

    if (cvui::button(canvas(), 1120, 160, 100, 30, "Home"))
    {
        go_home();
    }

    cvui::window(canvas(), 980, 250, 260, 180, "Counters");
    cvui::text(canvas(), 1000, 290, "BIN 1 (GREEN):  " + std::to_string(_bin1_count));
    cvui::text(canvas(), 1000, 320, "BIN 2 (BLUE):   " + std::to_string(_bin2_count));
    cvui::text(canvas(), 1000, 350, "BIN 3 (PINK):   " + std::to_string(_bin3_count));
    cvui::text(canvas(), 1000, 380, "BIN 4 (YELLOW): " + std::to_string(_bin4_count));

    if (cvui::button(canvas(), 1060, 400, 100, 28, "Reset"))
    {
        reset_counts();
    }

    cvui::window(canvas(), 20, 520, 940, 180, "Hue Tuning");

    cvui::text(canvas(), 40, 560, "Debug Color:");
    if (cvui::button(canvas(), 150, 545, 100, 30, "GREEN")) _debug_color_index = 0;
    if (cvui::button(canvas(), 260, 545, 100, 30, "BLUE")) _debug_color_index = 1;
    if (cvui::button(canvas(), 370, 545, 100, 30, "PINK")) _debug_color_index = 2;
    if (cvui::button(canvas(), 480, 545, 100, 30, "YELLOW")) _debug_color_index = 3;

    cvui::text(canvas(), 40, 610, "Selected: " + current_debug_color_name());
    cvui::text(canvas(), 40, 650, "Hue Low");
    cvui::trackbar(canvas(), 140, 635, 300, &current_h_low(), 0, 179);

    cvui::text(canvas(), 470, 650, "Hue High");
    cvui::trackbar(canvas(), 580, 635, 300, &current_h_high(), 0, 179);

    if (cvui::button(canvas(), 1140, 700, 80, 30, "Exit"))
    {
        _exit_requested = true;
    }

    cvui::update();
    cv::imshow(CANVAS_NAME, canvas());
}

bool CRecyclingSort::exit_requested()
{
    return _exit_requested;
}

void CRecyclingSort::process_camera()
{
    if (!_camera_ok)
    {
        _camera_retry_timer += dt();

        if (_camera_retry_timer >= _camera_retry_delay)
        {
            _camera_retry_timer = 0.0;

            if (_cam.isOpened())
                _cam.release();

            _cam.open(0, cv::CAP_V4L2);

            if (_cam.isOpened())
            {
                _cam.set(cv::CAP_PROP_FRAME_WIDTH, 640);
                _cam.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
                _cam.set(cv::CAP_PROP_BUFFERSIZE, 1);
                _camera_ok = true;
                _camera_fail_count = 0;
                _status_text = "CAMERA RECOVERED";
            }
        }

        return;
    }

    cv::Mat new_frame;

    if (!_cam.read(new_frame) || new_frame.empty())
    {
        _camera_fail_count++;

        if (_camera_fail_count >= 5)
        {
            _camera_ok = false;
            _status_text = "CAMERA LOST - RETRYING";
        }

        return;
    }

    _camera_fail_count = 0;
    _frame = new_frame;
    _display = _frame.clone();

    if (_detect_roi.x < 0 || _detect_roi.y < 0 ||
        _detect_roi.x + _detect_roi.width > _frame.cols ||
        _detect_roi.y + _detect_roi.height > _frame.rows)
    {
        _detect_roi = cv::Rect(0, 0, _frame.cols, _frame.rows);
    }

    cv::rectangle(_display, _detect_roi, cv::Scalar(0, 255, 255), 2);

    _roi = _frame(_detect_roi).clone();
    cv::cvtColor(_roi, _hsv, cv::COLOR_BGR2HSV);

    std::vector<std::string> colors;
    colors.push_back("GREEN");
    colors.push_back("BLUE");
    colors.push_back("PINK");
    colors.push_back("YELLOW");

    int best_area_all = 0;
    cv::Rect best_box_all;
    std::string best_color_all = "NONE";
    cv::Mat best_mask_all;

    for (size_t i = 0; i < colors.size(); i++)
    {
        cv::Mat test_mask;
        int area = 0;
        cv::Rect box;

        if (detect_color_mask(_hsv, colors[i], test_mask, area, box))
        {
            if (area > best_area_all)
            {
                best_area_all = area;
                best_box_all = box;
                best_color_all = colors[i];
                best_mask_all = test_mask.clone();
            }
        }
    }

    _largest_area = best_area_all;
    _largest_box = best_box_all;
    _detected_color = best_color_all;
    _target_bin = color_to_bin(_detected_color);

    if (_largest_area > 400)
    {
        _ball_present = true;

        cv::Rect draw_box(_largest_box.x + _detect_roi.x,
            _largest_box.y + _detect_roi.y,
            _largest_box.width,
            _largest_box.height);

        cv::rectangle(_display, draw_box, cv::Scalar(0, 255, 0), 2);
        cv::putText(_display, _detected_color,
            cv::Point(draw_box.x, draw_box.y - 10),
            cv::FONT_HERSHEY_SIMPLEX, 0.7,
            cv::Scalar(0, 255, 0), 2);

        _mask_display = best_mask_all.clone();
    }
    else
    {
        _ball_present = false;
        _detected_color = "NONE";
        _target_bin = 0;

        if (!_roi.empty())
            _mask_display = cv::Mat::zeros(_roi.size(), CV_8UC1);
    }
}

bool CRecyclingSort::detect_color_mask(const cv::Mat& hsv, const std::string& color_name,
    cv::Mat& out_mask, int& best_area, cv::Rect& best_box)
{
    best_area = 0;
    best_box = cv::Rect(0, 0, 0, 0);

    if (color_name == "GREEN")
    {
        cv::inRange(hsv,
            cv::Scalar(_green_h_low, 60, 40),
            cv::Scalar(_green_h_high, 255, 255),
            out_mask);
    }
    else if (color_name == "BLUE")
    {
        cv::inRange(hsv,
            cv::Scalar(_blue_h_low, 60, 40),
            cv::Scalar(_blue_h_high, 255, 255),
            out_mask);
    }
    else if (color_name == "PINK")
    {
        cv::inRange(hsv,
            cv::Scalar(_pink_h_low, 50, 50),
            cv::Scalar(_pink_h_high, 255, 255),
            out_mask);
    }
    else if (color_name == "YELLOW")
    {
        cv::inRange(hsv,
            cv::Scalar(_yellow_h_low, 80, 60),
            cv::Scalar(_yellow_h_high, 255, 255),
            out_mask);
    }
    else
    {
        return false;
    }

    cv::erode(out_mask, out_mask, cv::Mat(), cv::Point(-1, -1), 1);
    cv::dilate(out_mask, out_mask, cv::Mat(), cv::Point(-1, -1), 2);

    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::Mat temp = out_mask.clone();

    cv::findContours(temp, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    for (size_t i = 0; i < contours.size(); i++)
    {
        int area = (int)cv::contourArea(contours[i]);

        if (area > best_area)
        {
            best_area = area;
            best_box = cv::boundingRect(contours[i]);
        }
    }

    return best_area > 400;
}

int CRecyclingSort::color_to_bin(const std::string& color_name)
{
    if (color_name == "GREEN") return 1;
    if (color_name == "BLUE") return 2;
    if (color_name == "PINK") return 3;
    if (color_name == "YELLOW") return 4;
    return 0;
}

void CRecyclingSort::set_ramp_angle(int angle)
{
    if (angle < 0) angle = 0;
    if (angle > 180) angle = 180;
    _ramp_angle = angle;
}

void CRecyclingSort::sort_bin1()
{
    set_ramp_angle(_ramp_bin1);
}

void CRecyclingSort::sort_bin2()
{
    set_ramp_angle(_ramp_bin2);
}

void CRecyclingSort::sort_bin3()
{
    set_ramp_angle(_ramp_bin3);
}

void CRecyclingSort::sort_bin4()
{
    set_ramp_angle(_ramp_bin4);
}

void CRecyclingSort::go_home()
{
    set_ramp_angle(_ramp_home);
}

void CRecyclingSort::feed_open()
{
    _feed_angle = _feed_open;
}

void CRecyclingSort::feed_close()
{
    _feed_angle = _feed_closed;
}

void CRecyclingSort::drop_open()
{
    _drop_angle = _drop_open;
}

void CRecyclingSort::drop_close()
{
    _drop_angle = _drop_closed;
}

void CRecyclingSort::stage_one()
{
    drop_close();

    feed_open();
    _feed_active = true;
    _feed_timer = 0.0;
    _status_text = "STAGING";
}

void CRecyclingSort::release_staged_ball()
{
    drop_open();
    _release_active = true;
    _release_timer = 0.0;
    _status_text = "RELEASING";
}

void CRecyclingSort::reset_counts()
{
    _bin1_count = 0;
    _bin2_count = 0;
    _bin3_count = 0;
    _bin4_count = 0;
    _status_text = "COUNTERS RESET";
}

void CRecyclingSort::trigger_bin1()
{
    sort_bin1();
    _bin1_count++;
    _bin1_led_timer = _bin_led_flash_time;
    _status_text = (_auto_mode ? "AUTO -> BIN 1" : "MANUAL -> BIN 1");

    if (_auto_mode)
    {
        _sort_in_progress = true;
        _sort_timer = 0.0;
    }
    else
    {
        _sort_in_progress = false;
    }
}

void CRecyclingSort::trigger_bin2()
{
    sort_bin2();
    _bin2_count++;
    _bin2_led_timer = _bin_led_flash_time;
    _status_text = (_auto_mode ? "AUTO -> BIN 2" : "MANUAL -> BIN 2");

    if (_auto_mode)
    {
        _sort_in_progress = true;
        _sort_timer = 0.0;
    }
    else
    {
        _sort_in_progress = false;
    }
}

void CRecyclingSort::trigger_bin3()
{
    sort_bin3();
    _bin3_count++;
    _bin3_led_timer = _bin_led_flash_time;
    _status_text = (_auto_mode ? "AUTO -> BIN 3" : "MANUAL -> BIN 3");

    if (_auto_mode)
    {
        _sort_in_progress = true;
        _sort_timer = 0.0;
    }
    else
    {
        _sort_in_progress = false;
    }
}

void CRecyclingSort::trigger_bin4()
{
    sort_bin4();
    _bin4_count++;
    _bin4_led_timer = _bin_led_flash_time;
    _status_text = (_auto_mode ? "AUTO -> BIN 4" : "MANUAL -> BIN 4");

    if (_auto_mode)
    {
        _sort_in_progress = true;
        _sort_timer = 0.0;
    }
    else
    {
        _sort_in_progress = false;
    }
}

int& CRecyclingSort::current_h_low()
{
    if (_debug_color_index == 0) return _green_h_low;
    if (_debug_color_index == 1) return _blue_h_low;
    if (_debug_color_index == 2) return _pink_h_low;
    return _yellow_h_low;
}

int& CRecyclingSort::current_h_high()
{
    if (_debug_color_index == 0) return _green_h_high;
    if (_debug_color_index == 1) return _blue_h_high;
    if (_debug_color_index == 2) return _pink_h_high;
    return _yellow_h_high;
}

std::string CRecyclingSort::current_debug_color_name()
{
    if (_debug_color_index == 0) return "GREEN";
    if (_debug_color_index == 1) return "BLUE";
    if (_debug_color_index == 2) return "PINK";
    return "YELLOW";
}