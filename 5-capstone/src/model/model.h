//
// Created by ahmed on 4/14/2023.
//

#ifndef HELLOWORLD_MODEL_H
#define HELLOWORLD_MODEL_H

#include <string>
#include <utility>
#include "opencv2/opencv.hpp"

struct CameraConfig{
    std::string id;
    std::string source;

    CameraConfig(std::string id, std::string source): id{std::move(id)}, source{std::move(source)} {}
};



#endif
