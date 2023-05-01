//
// Created by ahmed on 4/16/2023.
//

#ifndef HELLOWORLD_WINDOWFRAME_H
#define HELLOWORLD_WINDOWFRAME_H

#include "opencv2/opencv.hpp"
#include <string>

class WindowFrame {
public:
    WindowFrame();

    WindowFrame(std::string window_name, cv::Mat frame);

    cv::Mat getFrame() const;

    std::string getWindowName() const;

private:
    std::string _window_name;
    cv::Mat _frame;
};


#endif //HELLOWORLD_WINDOWFRAME_H
