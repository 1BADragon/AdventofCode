#include <iostream>
#include <array>
#include <fstream>
#include <string>
#include <numeric>

#include <advent.hpp>

void part1(std::istream &input)
{
    int prev;
    int curr;
    int counter = 0;

    input >> prev;

    while (!input.eof())
    {
        input >> curr;

        if (input.eof()) {
            break;
        }

        if (curr > prev) {
            counter++;
        }

        prev = curr;
    }

    std::cout << "Answer 1: " << counter << std::endl;
}

void part2(std::istream &input)
{
    std::array<int, 3> win;

    input >> win[0];
    input >> win[1];
    input >> win[2];

    int curr;
    int prev;
    int counter = 0;

    prev = std::accumulate(win.begin(), win.end(), 0);

    while (!input.eof())
    {
        win[0] = win[1];
        win[1] = win[2];

        input >> win[2];

        if (input.eof()) {
            break;
        }

        curr = std::accumulate(win.begin(), win.end(), 0);

        if (curr > prev) {
            counter++;
        }

        prev = curr;
    }

    std::cout << "Answer 2: " << counter << std::endl;
}
