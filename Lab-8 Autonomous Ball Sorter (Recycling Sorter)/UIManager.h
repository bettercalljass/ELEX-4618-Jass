#pragma once

#include <opencv2/opencv.hpp>
#include <string>

class UIManager
{
public:
    void draw(
        cv::Mat& canvas,
        const cv::Mat& camera,
        const cv::Mat& mask,
        bool auto_mode,
        const std::string& status,
        const std::string& detected
    );
};
