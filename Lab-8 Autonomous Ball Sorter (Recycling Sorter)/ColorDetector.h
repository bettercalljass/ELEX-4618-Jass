#pragma once

#include <opencv2/opencv.hpp>
#include <string>

class ColorDetector
{
private:
    int _green_low, _green_high;
    int _blue_low, _blue_high;
    int _pink_low, _pink_high;
    int _yellow_low, _yellow_high;

    std::string _detected_color;
    int _target_bin;

public:
    ColorDetector();

    bool detect(
        const cv::Mat& frame,
        std::string& color,
        int& bin,
        cv::Rect& box,
        cv::Mat& mask
    );

    bool detect_mask(
        const cv::Mat& hsv,
        const std::string& color_name,
        cv::Mat& out_mask,
        int& best_area,
        cv::Rect& best_box
    );
};
