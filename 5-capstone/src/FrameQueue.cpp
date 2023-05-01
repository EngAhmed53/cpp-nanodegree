//
// Created by ahmed on 4/15/2023.
//

#include "FrameQueue.h"

WindowFrame FrameQueue::receive() {
    std::unique_lock<std::mutex> uLock(_mutex);
    _cond.wait(uLock, [this] { return !_queue.empty(); });
    WindowFrame wFrame = std::move(_queue.front());
    _queue.pop_front();
    return wFrame;
}

bool FrameQueue::receive(WindowFrame &wFrame) {
    std::unique_lock<std::mutex> uLock(_mutex);
    _cond.wait_for(uLock,std::chrono::milliseconds (250), [this] { return !_queue.empty(); });
    if (_queue.empty()) return false;
    else {
        wFrame = std::move(_queue.front());
        _queue.pop_front();
        return true;
    }
}

void FrameQueue::send(WindowFrame &&wFrame) {
    std::lock_guard<std::mutex> lock_guard(_mutex);
    _queue.push_back(wFrame);
    _cond.notify_one();
}

