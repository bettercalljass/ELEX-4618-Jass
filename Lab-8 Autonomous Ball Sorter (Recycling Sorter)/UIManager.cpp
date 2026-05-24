#include "UIManager.h"
#include "cvui.h"

#define CANVAS_NAME "Display Image"

void UIManager::draw(
    cv::Mat& canvas,
    const cv::Mat& frame,
    const cv::Mat& mask,
    bool auto_mode,
    const std::string& status,
    const std::string& detected,
    const BinCounter& counters)
{
    canvas = cv::Scalar(40, 40, 40);

    if (!frame.empty())
    {
        cv::Mat resized;

        cv::resize(frame, resized, cv::Size(640, 480));

        resized.copyTo(canvas(cv::Rect(20, 20, 640, 480)));
    }

    cvui::window(canvas, 700, 20, 250, 160, "System");

    cvui::text(canvas, 720, 60, "Status: " + status);

    cvui::text(canvas, 720, 90, "Detected: " + detected);

    cvui::text(
        canvas,
        720,
        120,
        "Mode: " + std::string(auto_mode ? "AUTO" : "MANUAL")
    );

    cvui::window(canvas, 700, 220, 250, 180, "Counters");

    cvui::text(canvas, 720, 260,
        "Bin 1: " + std::to_string(counters.get(1)));

    cvui::text(canvas, 720, 290,
        "Bin 2: " + std::to_string(counters.get(2)));

    cvui::text(canvas, 720, 320,
        "Bin 3: " + std::to_string(counters.get(3)));

    cvui::text(canvas, 720, 350,
        "Bin 4: " + std::to_string(counters.get(4)));

    cvui::update();

    cv::imshow(CANVAS_NAME, canvas);
}
