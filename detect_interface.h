#pragma once

#include <string>
#include <vector>

#include <opencv4/opencv2/opencv.hpp>
#include <type_define.h>
#include <functional>

typedef std::function<void(cv::Mat &)> callback_func;
class JSSDetectInterface
{
public:
    JSSDetectInterface(const std::string &model_name, callback_func func);
    ~JSSDetectInterface();

    void push_image(cv::Mat &image, unsigned long id);

private:
    callback_func func_;
    int detect_callback(std::vector<detectBox> &detect_msg)
    {
        printf("there are %ld detect box in last image.\n", detect_msg.size());
        // 处理识别内容
        cv::Mat img(cv::Size(1920, 1080), CV_MAKE_TYPE(8, 4), cv::Scalar(0));
        for (auto &box : detect_msg)
        {
            cv::Scalar color = cv::Scalar(255, 0, 255, 255);
            cv::line(
                img,
                cv::Point2i(box.box_left_top_x, box.box_left_top_y),
                cv::Point2i(box.box_right_bottom_x, box.box_left_top_y),
                color);
            cv::line(
                img,
                cv::Point2i(box.box_right_bottom_x, box.box_left_top_y),
                cv::Point2i(box.box_right_bottom_x, box.box_right_bottom_y),
                color);
            cv::line(
                img,
                cv::Point2i(box.box_right_bottom_x, box.box_right_bottom_y),
                cv::Point2i(box.box_left_top_x, box.box_right_bottom_y),
                color);
            cv::line(
                img,
                cv::Point2i(box.box_left_top_x, box.box_right_bottom_y),
                cv::Point2i(box.box_left_top_x, box.box_left_top_y),
                color);
        }
        func_(img);
        return 0;
    }
};