#include "CameraSystem.h"

CameraSystem::CameraSystem()
{
    _camera_ok = false;

    _fail_count = 0;

    _retry_timer = 0.0;
    _retry_delay = 1.0;
}

bool CameraSystem::init()
{
    _cam.open(0, cv::CAP_V4L2);

    if (_cam.isOpened())
    {
        _cam.set(cv::CAP_PROP_FRAME_WIDTH, 640);
        _cam.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
        _cam.set(cv::CAP_PROP_BUFFERSIZE, 1);

        _camera_ok = true;

        return true;
    }

    return false;
}

bool CameraSystem::update(double dt)
{
    if (!_camera_ok)
    {
        _retry_timer += dt;

        if (_retry_timer >= _retry_delay)
        {
            _retry_timer = 0.0;

            init();
        }

        return false;
    }

    cv::Mat temp;

    if (!_cam.read(temp) || temp.empty())
    {
        _fail_count++;

        if (_fail_count >= 5)
        {
            _camera_ok = false;
        }

        return false;
    }

    _fail_count = 0;

    _frame = temp.clone();

    return true;
}

cv::Mat CameraSystem::get_frame()
{
    return _frame;
}

bool CameraSystem::is_ok() const
{
    return _camera_ok;
}
