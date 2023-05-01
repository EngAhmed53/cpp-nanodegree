#include <iostream>
#include <fstream>
#include <vector>
#include "model/model.h"
#include "nlohmann/json.hpp"
#include <future>
#include "stream/Stream.h"
#include <memory>
#include "FrameQueue.h"

using json = nlohmann::json;
using namespace std::chrono_literals;

std::vector<CameraConfig> parseConfigFile() {
    std::vector<CameraConfig> configs;

    std::ifstream configFile("../data/config.json");

    if (configFile.is_open()) {
        json jsonFile;
        configFile >> jsonFile;

        for (auto obj: jsonFile.at("configs")) {
            configs.emplace_back(obj["id"].get<std::string>(), obj["source"].get<std::string>());
        }
    } else {
        throw std::runtime_error("Could not open the config file!");
    }

    return configs;
}

bool isBlank(const std::string &str) {
    return std::all_of(str.begin(), str.end(), [](unsigned char c) {
        return std::isspace(c);
    });
}

int main() {

    std::vector<std::future<void>> bgJobs{};
    std::vector<std::shared_ptr<Stream>> runningStreams{};
    std::shared_ptr<FrameQueue> frames_queue = std::make_shared<FrameQueue>();

    auto configs = parseConfigFile();
    for (auto &config: configs) {
        if (!isBlank(config.source)) {
            cv::namedWindow(config.id, cv::WINDOW_NORMAL);
            cv::resizeWindow(config.id, 480, 320);
            auto stream = std::make_shared<Stream>(frames_queue, config);
            bgJobs.emplace_back(std::async(std::launch::async, &Stream::start, stream));
            runningStreams.emplace_back(std::move(stream));
        } else {
            std::cerr << "Unable to start stream " << config.id << ", malformed source!\n";
        }
    }

    while (true) {
        std::this_thread::sleep_for(1ms);

        WindowFrame wFrame;
        if (frames_queue->receive(wFrame)) {
            cv::imshow(wFrame.getWindowName(), wFrame.getFrame());
        }

        int key = cv::waitKey(1);
        if (key == 27) {
            for (const auto &stream: runningStreams) {
                stream->stop();
                int isVisible = cv::getWindowProperty(stream->getId(), cv::WND_PROP_VISIBLE);
                if (isVisible != 0) {
                    cv::destroyWindow(stream->getId());
                }
            }
            break;
        }
    }

    for (auto &bgJob: bgJobs) {
        bgJob.wait();
    }

    return 0;
}