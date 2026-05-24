#include "ColorDetector.h"

ColorDetector::ColorDetector()
{
    _green_low = 35;
    _green_high = 85;

    _blue_low = 90;
    _blue_high = 130;

    _pink_low = 140;
    _pink_high = 179;

    _yellow_low = 20;
    _yellow_high = 35;
}

bool ColorDetector::detect(
    const cv::Mat& frame,
    std::string& color,
    int& bin,
    cv::Rect& box,
    cv::Mat& mask)
{
    cv::Rect roi(270, 160, 60, 40);

    cv::Mat cropped = frame(roi).clone();

    cv::Mat hsv;

    cv::cvtColor(cropped, hsv, cv::COLOR_BGR2HSV);

    std::vector<std::string> colors =
    {
        "GREEN",
        "BLUE",
        "PINK",
        "YELLOW"
    };

    int best_area = 0;

    for (auto c : colors)
    {
        cv::Mat temp_mask;

        int area = 0;

        cv::Rect temp_box;

        if (detect_mask(hsv, c, temp_mask, area, temp_box))
        {
            if (area > best_area)
            {
                best_area = area;

                color = c;

                box = temp_box;

                mask = temp_mask.clone();
            }
        }
    }

    if (best_area > 400)
    {
        if (color == "GREEN") bin = 1;
        else if (color == "BLUE") bin = 2;
        else if (color == "PINK") bin = 3;
        else if (color == "YELLOW") bin = 4;
        else bin = 0;

        return true;
    }

    color = "NONE";

    bin = 0;

    return false;
}

bool ColorDetector::detect_mask(
    const cv::Mat& hsv,
    const std::string& color_name,
    cv::Mat& out_mask,
    int& best_area,
    cv::Rect& best_box)
{
    best_area = 0;

    if (color_name == "GREEN")
    {
        cv::inRange(
            hsv,
            cv::Scalar(_green_low, 60, 40),
            cv::Scalar(_green_high, 255, 255),
            out_mask
        );
    }

    else if (color_name == "BLUE")
    {
        cv::inRange(
            hsv,
            cv::Scalar(_blue_low, 60, 40),
            cv::Scalar(_blue_high, 255, 255),
            out_mask
        );
    }

    else if (color_name == "PINK")
    {
        cv::inRange(
            hsv,
            cv::Scalar(_pink_low, 50, 50),
            cv::Scalar(_pink_high, 255, 255),
            out_mask
        );
    }

    else if (color_name == "YELLOW")
    {
        cv::inRange(
            hsv,
            cv::Scalar(_yellow_low, 80, 60),
            cv::Scalar(_yellow_high, 255, 255),
            out_mask
        );
    }

    cv::erode(out_mask, out_mask, cv::Mat(), cv::Point(-1, -1), 1);

    cv::dilate(out_mask, out_mask, cv::Mat(), cv::Point(-1, -1), 2);

    std::vector<std::vector<cv::Point>> contours;

    cv::findContours(
        out_mask,
        contours,
        cv::RETR_EXTERNAL,
        cv::CHAIN_APPROX_SIMPLE
    );

    for (auto c : contours)
    {
        int area = (int)cv::contourArea(c);

        if (area > best_area)
        {
            best_area = area;

            best_box = cv::boundingRect(c);
        }
    }

    return best_area > 400;
}
