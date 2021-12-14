#include <iostream>
#include <istream>
#include <string>
#include <numeric>
#include <array>

#include <advent.hpp>
#include <string_utils.hpp>

void part1(std::istream &input)
{
    int total_sf = 0;

    for (;;) {
        std::string line;
        input >> line;

        if (input.eof()) {
            break;
        }

        auto dims = parse<int>(split(line, "x"));

        std::array<int, 3> sides;

        sides[0] = dims[0] * dims[1];
        sides[1] = dims[1] * dims[2];
        sides[2] = dims[0] * dims[2];

        total_sf += std::accumulate(sides.begin(), sides.end(), 0) * 2 +
                *std::min_element(sides.begin(), sides.end());
    }

    std::cout << "Answer: " << total_sf << std::endl;
}

void part2(std::istream &input)
{
    int total_sf = 0;

    for (;;) {
        std::string line;
        input >> line;

        if (input.eof()) {
            break;
        }

        auto dims = parse<int>(split(line, "x"));

        auto volume = std::accumulate(dims.begin(), dims.end(), 1,
                                      std::multiplies<int>());

        std::array<int, 3> areas;

        areas[0] = dims[0] * 2;
        areas[1] = dims[1] * 2;
        areas[2] = dims[2] * 2;

        auto total_area = std::accumulate(areas.begin(), areas.end(), 0);

        total_area -= *std::max_element(areas.begin(), areas.end());

        total_sf += volume + total_area;
    }

    std::cout << "Answer: " << total_sf << std::endl;
}

