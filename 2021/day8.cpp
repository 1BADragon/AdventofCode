
#include <advent.hpp>
#include <vector>
#include <array>
#include <string>
#include <string_utils.hpp>
#include <cassert>
#include <iostream>
#include <iterator>
#include <set>

using SegSet = std::set<char>;

struct Display {
    std::vector<std::string> input;
    std::vector<std::string> output;
};

static Display parse_line(const std::string &line);
static uint32_t get_output(const Display &d);
static SegSet intercect(const SegSet &a, const SegSet &b);
static void print_found(int i);

void part1(std::istream &input)
{
    std::vector<Display> displays;
    for (;;) {
        std::string line;
        std::getline(input, line);

        if (input.eof()) {
            break;
        }

        auto d = parse_line(line);

        displays.push_back(d);
    }

    int count = 0;
    for (auto &d : displays) {
        for (auto &o : d.output) {
            size_t s = o.size();
            if (s == 2 || s == 3 || s == 4 || s == 7) {
                count++;
            }
        }
    }

    std::cout << "Answer 1: " << count << std::endl;
}

void part2(std::istream &input)
{
    std::vector<Display> displays;
    for (;;) {
        std::string line;
        std::getline(input, line);

        if (input.eof()) {
            break;
        }

        auto d = parse_line(line);

        displays.push_back(d);
    }

    uint64_t sum = 0;
    for (auto &d : displays) {
        sum += get_output(d);
    }

    std::cout << "Answer 2: " << sum << std::endl;
}

static Display parse_line(const std::string &line)
{
    Display d;

    auto rl = split(line, " | ");

    d.input = split(rl[0], " ");
    d.output = split(rl[1], " ");

    assert(d.input.size() == 10);
    assert(d.output.size() == 4);

    return d;
}

static uint32_t get_output(const Display &d)
{
    /*
     * Segments in my head by index:
     *
     *       00000
     *      1     2
     *      1     2
     *       33333
     *      4     5
     *      4     5
     *       66666
     *
     * have:
     *  0 1 3 4 6 7 8 9
     */
    std::array<std::string, 7> segments;
    std::array<SegSet, 10> numbers;

    // first find the input values with 2, 3, 4 and 7 segments
    for (auto &i : d.input) {
        if (i.size() == 2) {
            print_found(1);
            numbers[1] = {i.begin(), i.end()};
        }

        if (i.size() == 3) {
            print_found(7);
            numbers[7] = {i.begin(), i.end()};
        }

        if (i.size() == 4) {
            print_found(4);
            numbers[4] = {i.begin(), i.end()};
        }

        if (i.size() == 7) {
            print_found(8);
            numbers[8] = {i.begin(), i.end()};
        }
    }

    for (auto &i : d.input) {
        if (i.size() == 5) {
            // its either 2, 3, 5
            SegSet i_set(i.begin(), i.end());

            auto tmp = intercect(numbers[1], i_set);
            if (tmp == numbers[1]) {
                // its the 3
                print_found(3);
                numbers[3] = {i.begin(), i.end()};
            }
        }
    }

    for (auto &i : d.input) {
        if (i.size() == 6) {
            // its either 0, 6, or 9
            SegSet i_set(i.begin(), i.end());

            auto tmp = intercect(numbers[4], i_set);

            if (tmp == numbers[4]) {
                // its the 9
                print_found(9);
                numbers[9] = {i.begin(), i.end()};
                continue;
            }

            tmp = intercect(numbers[7], i_set);

            if (tmp != numbers[7]) {
                // its the 6
                print_found(6);
                numbers[6] = {i.begin(), i.end()};
                continue;
            }
        }
    }

    for (auto &i : d.input) {
        if (i.size() == 6) {
            SegSet tmp(i.begin(), i.end());
            if (tmp != numbers[6] && tmp != numbers[9]) {
                // its the 0
                print_found(0);
                numbers[0] = tmp;
            }
        }
    }

    // still need the 2 and 5
    for (auto &i : d.input) {
        if (i.size() == 5) {
            SegSet i_set(i.begin(), i.end());

            auto tmp = intercect(numbers[6], i_set);

            if (tmp.size() == 5) {
                // its the 5
                print_found(5);
                numbers[5] = {i.begin(), i.end()};
            } else if (numbers[3] != i_set) {
                print_found(2);
                numbers[2] = {i.begin(), i.end()};
            }
        }
    }

//    for (size_t i = 0; i < numbers.size(); ++i) {
//        std::cout << "Number " << i << ": ";
//        for (auto c : numbers[i]) {
//            std::cout << c;
//        }
//        std::cout << std::endl;
//    }

    std::string numstr;
    for (auto &o : d.output) {
        SegSet tmp{o.begin(), o.end()};

        for (size_t i = 0; i < numbers.size(); ++i) {
            if (numbers[i] == tmp) {
                numstr.push_back('0' + i);
                break;
            }
        }
    }

    return parse<uint32_t>(numstr);
}

static SegSet intercect(const SegSet &a, const SegSet &b)
{
    SegSet ret;
    for (auto &i : a) {
        if (b.contains(i)) {
            ret.insert(i);
        }
    }

    return ret;
}

static void print_found(int i)
{
    //std::cout << "Found the " << i << std::endl;
}
