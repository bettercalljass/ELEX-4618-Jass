#pragma once

#include <opencv2/opencv.hpp>

class CameraSystem
{
private:
    cv::VideoCapture _cam;
    bool _camera_ok;

    int _fail_count;
    double _retry_timer;
    double _retry_delay;

public:
    CameraSystem();

    bool init();
    bool update(double dt);

    cv::Mat get_frame();

    bool is_ok() const;
};
