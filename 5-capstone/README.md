# surveillanceCameraSystem

This is the captone project for the course [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213), option A.

## Dependencies for Running Locally

- cmake >= 2.8
  - All OSes: [click here for installation instructions](https://cmake.org/install/)
- make >= 4.1 (Linux, Mac), 3.81 (Windows)
  - Linux: make is installed by default on most Linux distros
  - Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  - Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
- gcc/g++ >= 5.4
  - Linux: gcc / g++ is installed by default on most Linux distros
  - Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  - Windows: recommend using [MinGW](http://www.mingw.org/)
- OpenCV >= 4.1
  - The OpenCV 4.1.0 source code can be found [here](https://github.com/opencv/opencv/tree/4.1.0)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir -p build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./surveillanceCamera`

## Project Structure

```
└── Peoject/
    ├── assets # videos to test the application functionality
    ├── data/ # data needed to run the application
    │   ├── config.json # user's configurations for stream sources
    │   └── haarcascade_frontalface_alt.xml # face detection algorithm
    ├── saved-images/ # saved frames folder
    ├── src/ # application source code
    │   ├── classifier/ # face detection helper class
    │   │   ├── FacesCascadeClassifier.cpp
    │   │   └── FacesCascadeClassifier.h
    │   ├── model/ # models used through the application 
    │   │   ├── model.h
    │   │   ├── WindowFrame.cpp # a class wrap a frame associated with a specific window
    │   │   └── WindowFrame.h
    │   ├── nlohmann/ # json library to read and parse the config.json
    │   │   └── json.hpp
    │   ├── stream/ # the main class for camera stream handling, frames processing and saving
    │   │   ├── Stream.cpp
    │   │   └── Stream.h
    │   ├── FrameQueue.cpp # A thread-safe queue class to queue frames comming from multiple streams 
    │   ├── FrameQueue.h
    │   └── main.cpp # the start point which parse user's configurations start and display streams and handle user events
    ├── .gitignore
    ├── CMakeLists.txt
    ├── CODEOWNERS
    ├── LICENSE.md
    └── README.md
```

### Implementation Details
`config.json`
The config.json file allows users to add one or more stream configurations to the application. Each configuration includes a unique ID for the stream and its source, which can be a path, URL, or local camera.

`main.cpp`
The main.cpp file reads and parses the list of stream configurations entered by the user. It then creates and starts a Stream object for each camera, with each stream object reading camera frames in a background thread. Additionally, for each running stream, an OpenCV window is launched to display the incoming frames in the main thread.

The application will stop when the user presses the "esc" button.

`Stream.cpp`
The Stream.cpp file is responsible for the following tasks:

1. Reading video frames from the source.
2. Detecting human faces in each frame with the support of the FacesCascadeClassifier object.
3. Drawing a blue rectangle around each detected face.
4. Saving the frame in the saved image folder if it contains faces.
5. Sending the frame to the main thread to display it.

`FrameQueue.cpp`
FrameQueue.cpp is used for communication between the main thread (frames display thread) and the background threads (frames reading and processing threads).

`WindowFrame.cpp`
WindowFrame.cpp is responsible for encapsulating the captured frame and the window responsible for displaying this frame. It is used to send captured frames between the background threads and the main thread.

### Criteria Addressed:

- The project demonstrates an understanding of C++ functions and control structures. => main.cpp, Stream.cpp(Stream::start)
- The project reads data from a file and process the data, or the program writes data to a file. => main.cpp(parseConfigFile()), Stream.cpp(Stream::saveFrame)
- The project uses Object Oriented Programming techniques. => Stream, FrameQueue and WindowFrame classes
- Classes abstract implementation details from their interfaces. => header files(Stream.h, FrameQueue.h and WindowFrame.h) are separated from implementation details
- The project uses multithreading. =>  main.cpp (line:51 start each stream on a different background thread).
- A mutex or lock is used in the project. => FrameQueue.cpp (FrameQueue::receive(), FrameQueue::send())
- A condition variable is used in the project. => FrameQueue.h