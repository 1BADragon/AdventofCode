#include <map>
#include <vector>
#include <iostream>
#include <advent.hpp>

static std::map<char, int> points = {
    {')', 3},
    {']', 57},
    {'}', 1197},
    {'>', 25137}
};

static std::map<char, int> points2 = {
    {')', 1},
    {']', 2},
    {'}', 3},
    {'>', 4}
};

static std::map<char, char> pairs = {
    {'(', ')'},
    {'[', ']'},
    {'{', '}'},
    {'<', '>'}
};

bool line_valid(const std::string &line, char &bad, std::string &to_complete);


void part1(std::istream &input)
{
    uint64_t sum = 0;

    for (;;) {
        char bad_char = 0;
        std::string line;
        std::string needed;
        std::getline(input, line);

        if (input.eof()) {
            break;
        }

        if (!line_valid(line, bad_char, needed)) {
            sum += points.at(bad_char);
        }
    }

    std::cout << "Answer 1: " << sum << std::endl;
}

void part2(std::istream &input)
{
    std::vector<uint64_t> sums;

    for (;;) {
        char bad_char = 0;
        std::string line;
        std::string needed;
        std::getline(input, line);

        if (input.eof()) {
            break;
        }

        if (line_valid(line, bad_char, needed)) {
            uint64_t sum = 0;

            for (char c : needed) {
                sum = (sum * 5) + points2.at(c);
            }
            sums.push_back(sum);
        }
    }

    std::sort(sums.begin(), sums.end(), std::greater<uint64_t>());

    std::cout << "Answer 2: " << sums[sums.size()/2] << std::endl;
}

bool line_valid(const std::string &line, char &bad, std::string &to_complete)
{
    std::vector<char> stack;

    for (char c : line) {
        if (pairs.contains(c)) {
            stack.push_back(c);
        } else {
            if (stack.empty()) {
                bad = c;
                return false;
            } else if (c != pairs.at(stack.back())) {
                bad = c;
                return false;
            } else {
                stack.pop_back();
            }
        }
    }

    for (auto it = stack.rbegin(); it != stack.rend(); ++it) {
        to_complete.push_back(pairs.at(*it));
    }

    return true;
}
