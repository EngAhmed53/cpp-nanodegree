cmake_minimum_required(VERSION 3.7)

add_definitions(-std=c++17)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

project(surveillanceCamera)

find_package(OpenCV 4.6 REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LIBRARY_DIRS})
add_definitions(${OpenCV_DEFINITIONS})

add_executable(surveillanceCamera src/main.cpp src/nlohmann/json.hpp src/stream/Stream.h src/model/model.h src/stream/Stream.cpp src/FrameQueue.h src/FrameQueue.cpp src/model/WindowFrame.cpp src/model/WindowFrame.h src/classifier/FacesCascadeClassifier.cpp src/classifier/FacesCascadeClassifier.h)
target_link_libraries(surveillanceCamera ${OpenCV_LIBRARIES})