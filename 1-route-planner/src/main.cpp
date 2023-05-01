#include <optional>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <io2d.h>
#include "route_model.h"
#include "render.h"
#include "route_planner.h"
#include <regex>

using namespace std::experimental;

const std::string input_pattern = "\\d+(\\.\\d+)?,[ ]\\d+(\\.\\d+)?,[ ]\\d+(\\.\\d+)?,[ ]\\d+(\\.\\d+)?";
const std::regex pattern(input_pattern);

static std::optional<std::vector<std::byte>> ReadFile(const std::string &path)
{
    std::ifstream is{path, std::ios::binary | std::ios::ate};
    if (!is)
        return std::nullopt;

    auto size = is.tellg();
    std::vector<std::byte> contents(size);

    is.seekg(0);
    is.read((char *)contents.data(), size);

    if (contents.empty())
        return std::nullopt;
    return std::move(contents);
}

bool inRange(float value)
{
    return value <= 100 && value >= 0;
}

bool isCorrectInput(std::string input, float &start_x, float &start_y, float &end_x, float &end_y)
{
    if (std::regex_match(input, pattern))
    {
        sscanf(input.c_str(), "%f, %f, %f, %f", &start_x, &start_y, &end_x, &end_y);
        bool validRange = inRange(start_x) && inRange(start_y) && inRange(end_x) && inRange(end_y);
        if (!validRange)
        {
            std::cout << "Error: Invalid range\n";
        }
        return validRange;
    }
    else
    {
        std::cout << "Error: Invalid pattern\n";
        return false;
    }
}

int main(int argc, const char **argv)
{
    std::string osm_data_file = "";
    if (argc > 1)
    {
        for (int i = 1; i < argc; ++i)
            if (std::string_view{argv[i]} == "-f" && ++i < argc)
                osm_data_file = argv[i];
    }
    else
    {
        std::cout << "To specify a map file use the following format: " << std::endl;
        std::cout << "Usage: [executable] [-f filename.osm]" << std::endl;
        osm_data_file = "../map.osm";
    }

    std::vector<std::byte> osm_data;

    if (osm_data.empty() && !osm_data_file.empty())
    {
        std::cout << "Reading OpenStreetMap data from the following file: " << osm_data_file << std::endl;
        auto data = ReadFile(osm_data_file);
        if (!data)
            std::cout << "Failed to read." << std::endl;
        else
            osm_data = std::move(*data);
    }
    //Complete this TODO to satisfy Project Rubric Criterias of User Input

    // TODO 1: Declare floats `start_x`, `start_y`, `end_x`, and `end_y` and get
    // user input for these values using std::cin. Pass the user input to the
    // RoutePlanner object below in place of 10, 10, 90, 90.
    std::string input;
    float start_x = -1;
    float start_y = -1;
    float end_x = -1;
    float end_y = -1;

    std::cout << "Please insert start and end points\nStart & End points input should follow this pattern \"start_x, start_y, end_x, end_y\"\n";
    std::cout << "Range should be from 0 to 100:";
    std::getline(std::cin, input);
    while (!isCorrectInput(input, start_x, start_y, end_x, end_y))
    {
        input = "";
        std::cout << "Your input is invalid\n";
        std::cout << "Please try again:";
        std::getline(std::cin, input);
    }

    // Build Model.
    RouteModel model{osm_data};

    // Create RoutePlanner object and perform A* search.
    RoutePlanner route_planner{model, start_x, start_y, end_x, end_y};
    route_planner.AStarSearch();

    std::cout << "Distance: " << route_planner.GetDistance() << " meters. \n";

    // Render results of search.
    Render render{model};

    auto display = io2d::output_surface{400, 400, io2d::format::argb32, io2d::scaling::none, io2d::refresh_style::fixed, 30};
    display.size_change_callback([](io2d::output_surface &surface) {
        surface.dimensions(surface.display_dimensions());
    });
    display.draw_callback([&](io2d::output_surface &surface) {
        render.Display(surface);
    });
    display.begin_show();
}
