#include <limits>
#include <numeric>
#include <iostream>
#include <advent.hpp>
#include <string_utils.hpp>

size_t fuel_to(const std::vector<size_t> &locs, size_t to);
size_t fuel_to2(const std::vector<size_t> &locs, size_t to);

size_t sumzero_to_n(size_t n) {
    return n * (n + 1) / 2;
}

void part1(std::istream &input)
{
    std::string line;

    std::getline(input, line);

    auto vals = parse<size_t>(split(line, ","));

    size_t cur_min = std::numeric_limits<size_t>::max();
    size_t average = std::accumulate(vals.begin(), vals.end(), 0llu) / vals.size();

    for(size_t i = 0; i < vals.size() / 2 + 1; ++i) {
        cur_min = std::min(cur_min, fuel_to(vals, average + i));
        cur_min = std::min(cur_min, fuel_to(vals, average - i));
    }

    std::cout << "Answer 1: " << cur_min << std::endl;
}

void part2(std::istream &input)
{
    std::string line;

    std::getline(input, line);

    auto vals = parse<size_t>(split(line, ","));

    size_t cur_min = std::numeric_limits<size_t>::max();
    size_t average = std::accumulate(vals.begin(), vals.end(), 0llu) / vals.size();

    for(size_t i = 0; i < vals.size() / 2 + 1; ++i) {
        cur_min = std::min(cur_min, fuel_to2(vals, average + i));
        cur_min = std::min(cur_min, fuel_to2(vals, average - i));
    }

    std::cout << "Answer 1: " << cur_min << std::endl;
}

size_t fuel_to(const std::vector<size_t> &locs, size_t to)
{
    size_t total_fuel = 0;

    for (auto v : locs) {
        size_t distance;

        if (to >= v) {
            distance = to - v;
        } else {
            distance = v - to;
        }
        total_fuel += distance;
    }

    return total_fuel;
}

size_t fuel_to2(const std::vector<size_t> &locs, size_t to)
{
    size_t total_fuel = 0;

    for (auto v : locs) {
        size_t distance;

        if (to >= v) {
            distance = to - v;
        } else {
            distance = v - to;
        }
        total_fuel += sumzero_to_n(distance);
    }

    return total_fuel;
}
