//
// Created by ahmed on 4/16/2023.
//

#include "FacesCascadeClassifier.h"

FacesCascadeClassifier::FacesCascadeClassifier() {
    if(!_face_cascade.load("../data/haarcascade_frontalface_alt.xml")) {
        throw std::runtime_error("Unable to load face cascade classifier!");
    }
}

bool FacesCascadeClassifier::detect(cv::Mat &frame) {
    std::vector<cv::Rect> faces{};
    _face_cascade.detectMultiScale(frame, faces, 3);
    for(auto& face : faces) {
        cv::rectangle(frame, face, cv::Scalar(255, 0, 0), 2);
    }
    return !faces.empty();
}
