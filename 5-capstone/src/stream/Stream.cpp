//
// Created by ahmed on 4/14/2023.
//

#include "Stream.h"

using namespace std::chrono_literals;

Stream::Stream(std::shared_ptr<FrameQueue> frame_queue, CameraConfig &config)
        : _framesQueue(std::move(frame_queue)), _config(std::move(config)) {
    std::cout << "Constructor called with source = " << _config.source << std::endl;
}

Stream::~Stream() {
    std::cout << "Destructor called\n";
    _cap.release();
}

void Stream::start() {

    std::string window_name = _config.id;
    if (_config.source == "WebCam") {
        _cap = cv::VideoCapture(0);
    } else _cap = cv::VideoCapture(_config.source);

    if (!_cap.isOpened()) {
        _framesQueue->send(std::move(WindowFrame(window_name, generateErrorFrame("Unable to open video stream!"))));
    }
    while (_cap.isOpened() && !_hasStopped) {
        std::this_thread::sleep_for(1ms);
        cv::Mat frame;
        _cap.read(frame);
        if (frame.empty()) {
            break;
        }

        // detect faces in the frame
        if(_face_cascade.detect(frame)) {
            // if there are faces in this frame save it to saved-image folder
            saveFrame(frame);
        }

        // send the frame to the main thread for drawing
        _framesQueue->send(std::move(WindowFrame(window_name, frame)));
    }

    _framesQueue->send(std::move(WindowFrame(window_name, generateErrorFrame("End of video stream!"))));
}

std::string Stream::getId() const {
    return _config.id;
}

void Stream::stop() {
    _hasStopped = true;
}

void Stream::saveFrame(const cv::Mat &frame) const {
    auto current_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string str_time = std::to_string(current_time);
    auto fileName = _config.id + "-" + str_time;
    cv::imwrite("../saved-images/" + fileName + ".jpg", frame);
}

cv::Mat Stream::generateErrorFrame(const std::string& error) {
    cv::Mat image = cv::Mat::zeros(480, 320, CV_8UC3);
    cv::putText(image, error, cv::Point(100, 200), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 2);
    return image;
}

