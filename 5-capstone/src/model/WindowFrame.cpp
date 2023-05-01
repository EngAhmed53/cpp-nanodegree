//
// Created by ahmed on 4/16/2023.
//

#include "WindowFrame.h"

WindowFrame::WindowFrame() = default;

WindowFrame::WindowFrame(std::string window_name, cv::Mat frame) : _window_name(
        std::move(window_name)),_frame(std::move(frame)) {}

cv::Mat WindowFrame::getFrame() const {
    return _frame;
}

std::string WindowFrame::getWindowName() const {
    return _window_name;
}


