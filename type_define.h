#ifndef __TYPE_DEFINE_H
#define __TYPE_DEFINE_H

#include <vector>
#include <string>
#include <iostream>
#include <opencv2/opencv.hpp>

const static std::vector<std::string> yolov3Label = { "人", "自行车", "汽车", "摩托车",
    "飞机","公共汽车", "火车", "卡车", "船",
    "交通信号灯", "消防栓", "停车", "停车计费",
    "长凳", "鸟", "猫", "狗", "马",
    "羊", "牛", "大象", "熊", "斑马",
    "长颈鹿", "背包", "雨伞", "手提包","领带",
    "手提箱", "飞盘", "滑雪板", "滑雪板", "运动球",
    "风筝", "棒球棒", "棒球手套", "滑板", "冲浪板",
    "网球拍", "瓶子", "酒杯", "杯子",
    "叉", "刀", "勺", "碗", "香蕉",
    "苹果", "饼", "橘子", "西兰花", "胡萝卜",
    "汉堡", "饼", "面包", "蛋糕", "椅子",
    "沙发", "盆栽", "床", "餐桌", "厕所",
    "电视", "电脑", "鼠标", "远程控制", "键盘",
    "手机", "微波炉", "烤箱", "面包机", "洗碗槽",
    "冰箱", "书", "表", "花瓶","剪刀",
    "玩偶", "吹风机", "牙刷" };

struct detectBox
{
    uint32_t box_left_top_x;
    uint32_t box_left_top_y;
    uint32_t box_right_bottom_x;
    uint32_t box_right_bottom_y;
    uint32_t score;
    uint32_t label;
    int32_t id;
    float dv;
    bool lose;
    void print()
    {
        std::cout << "[" << box_left_top_x
                  << ", "<< box_left_top_y
                  << "  "<< box_right_bottom_x
                  << ", "<< box_right_bottom_y
                  << "]: " << label
                  << ": " << score <<std::endl;
    }
    detectBox()
    {

    }
    detectBox(const detectBox &box)
    {
        box_left_top_x = box.box_left_top_x;
        box_left_top_y = box.box_left_top_y;
        box_right_bottom_x = box.box_right_bottom_x;
        box_right_bottom_y = box.box_right_bottom_y;
        score = box.score;
        label = box.label;
        id = box.id;
        dv = box.dv;
        lose = box.lose;
    }
};

#endif // !__TYPE_DEFINE_H