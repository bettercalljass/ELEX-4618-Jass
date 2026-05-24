#include "CRecyclingSort.h"

CRecyclingSort::CRecyclingSort()
{
    _auto_mode = false;
    _exit_requested = false;

    _status_text = "MANUAL";

    canvas() = cv::Mat(760, 1280, CV_8UC3, cv::Scalar(40, 40, 40));

    _camera.init();
}

CRecyclingSort::~CRecyclingSort()
{
}

void CRecyclingSort::gpio()
{
    _servos.update(board());

    _leds.update(dt(), board());

    handle_buttons();
}

void CRecyclingSort::update()
{
    _camera.update(dt());

    cv::Mat frame = _camera.get_frame();

    if (!frame.empty())
    {
        _ball_present = _detector.detect(
            frame,
            _detected_color,
            _target_bin,
            _detected_box,
            _mask_display
        );
    }

    update_auto_sort();

    update_staging();

    update_sorting_logic();
}

void CRecyclingSort::draw()
{
    _ui.draw(
        canvas(),
        _camera.get_frame(),
        _mask_display,
        _auto_mode,
        _status_text,
        _detected_color,
        _counters
    );
}

bool CRecyclingSort::exit_requested()
{
    return _exit_requested;
}
