#ifndef HELLOWORLD_FRAMEQUEUE_H
#define HELLOWORLD_FRAMEQUEUE_H

#include <deque>
#include <mutex>
#include "opencv2/opencv.hpp"
#include "model/WindowFrame.h"
#include <condition_variable>

class FrameQueue {
public:
    void send(WindowFrame &&windowFrame);

    WindowFrame receive();

    bool receive(WindowFrame &wFrame);

private:
    std::deque<WindowFrame> _queue;
    std::mutex _mutex;
    std::condition_variable _cond;


};

#endif // HELLOWORLD_FRAMEQUEUE_H
