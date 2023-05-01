//
// Created by ahmed on 4/14/2023.
//

#ifndef HELLOWORLD_STREAM_H
#define HELLOWORLD_STREAM_H

#include <opencv2/opencv.hpp>
#include <string>
#include <memory>
#include "../model/WindowFrame.h"
#include "../FrameQueue.h"
#include "../model/model.h"
#include "../classifier/FacesCascadeClassifier.h"
#include <future>

class Stream {
public:
    Stream(std::shared_ptr<FrameQueue> frame_queue, CameraConfig &config);

    ~Stream();

    void start();

    void stop();

    std::string getId() const;

private:
    CameraConfig _config;
    cv::VideoCapture _cap;
    std::shared_ptr<FrameQueue> _framesQueue;
    FacesCascadeClassifier _face_cascade;
    bool _hasStopped{false};
    std::vector<std::future<void>> _imageWritingFuture{};

    void saveFrame(const cv::Mat &frame) const;

    static cv::Mat generateErrorFrame(const std::string& error);
};

#endif //HELLOWORLD_STREAM_H
