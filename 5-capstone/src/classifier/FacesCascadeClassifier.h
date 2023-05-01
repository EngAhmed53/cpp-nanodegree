//
// Created by ahmed on 4/16/2023.
//

#ifndef HELLOWORLD_FACESCASCADECLASSIFIER_H
#define HELLOWORLD_FACESCASCADECLASSIFIER_H

#include "opencv2/opencv.hpp"
#include <string>

class FacesCascadeClassifier {
public:
    FacesCascadeClassifier();
    bool detect(cv::Mat &frame);
private:
    cv::CascadeClassifier _face_cascade;
};


#endif //HELLOWORLD_FACESCASCADECLASSIFIER_H
