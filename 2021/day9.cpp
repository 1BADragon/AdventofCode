#include <vector>
#include <iostream>
#include <algorithm>
#include <advent.hpp>

using Map = std::vector<std::vector<uint16_t>>;
using Visit = std::vector<std::vector<bool>>;

static Map parse_input(std::istream &input);
static bool is_low(const Map &map, size_t x, size_t y);

static uint64_t basin_size(const Map &map, Visit &v, size_t x, size_t y, int depth);

void part1(std::istream &input)
{
    auto map = parse_input(input);

    uint64_t sum = 0;
    for (size_t y = 0; y < map.size(); ++y) {
        for (size_t x = 0; x < map[y].size(); ++x) {
            if (is_low(map, x, y)) {
                sum += 1 + map[y][x];
            }
        }
    }

    std::cout << "Answer 1: " << sum << std::endl;
}

void part2(std::istream &input)
{
    auto map = parse_input(input);
    auto visited = std::vector<std::vector<bool>>();

    visited.resize(map.size());
    for (auto &v: visited) {
        v.resize(map[0].size(), false);
    }

    std::vector<uint64_t> sizes;
    for (size_t y = 0; y < map.size(); ++y) {
        for (size_t x = 0; x < map[y].size(); ++x) {
            sizes.push_back(basin_size(map, visited, x, y, 0));
        }
    }
    std::sort(sizes.begin(), sizes.end(), std::greater<uint64_t>());

//    for(auto s : sizes) {
//        std::cout << s << " ";
//    }
//    std::cout << std::endl;

    std::cout << "Answer 2: " << sizes[0] * sizes[1] * sizes[2] << std::endl;
}


static Map parse_input(std::istream &input)
{
    Map map;

    for (;;) {
        std::string line;

        std::getline(input, line);
        if (input.eof()) {
            break;
        }


        std::vector<uint16_t> cs;
        for (char c : line) {
            if (c == '\n') {
                break;
            }
            cs.push_back(c - '0');
        }

        map.push_back(cs);
    }

    return map;
}

static bool is_low(const Map &map, size_t x, size_t y)
{
    uint8_t loc = map[y][x];
    if (x > 0 && map[y][x-1] <= loc) {
        return false;
    }

    if (y > 0 && map[y-1][x] <= loc) {
        return false;
    }

    if (y < map.size() - 1 && map[y+1][x] <= loc) {
        return false;
    }

    if (x < map[y].size() - 1 && map[y][x+1] <= loc) {
        return false;
    }

    return true;
}

static uint64_t basin_size(const Map &map, Visit &v, size_t x, size_t y, int depth)
{
    if (y >= map.size() || x >= map[y].size()) {
        return 0;
    }

    if (v[y][x]) {
        return 0;
    }

    if (map[y][x] == 9) {
        return 0;
    }

//    for (int i = 0; i < depth; ++i) {
//        std::cout << " ";
//    }

    //std::cout << "At " << x << "," << y << " " << map[y][x] << " " << v[y][x] << std::endl;

    v[y][x] = true;

    return 1 + basin_size(map, v, x + 1, y, depth + 1)
             + basin_size(map, v, x - 1, y, depth + 1)
             + basin_size(map, v, x, y + 1, depth + 1)
             + basin_size(map, v, x, y - 1, depth + 1);
}
