#include <iostream>
#include <istream>
#include <string>

#include <advent.hpp>

void part1(std::istream &input)
{
    int at = 0;
    char buf;

    for (;;) {
        input.read(&buf, 1);

        if (input.eof()) {
            break;
        }

        switch(buf) {
        case '(':
            at++;
            break;
        case ')':
            at--;
            break;
        }
    }

    std::cout << "Answer 1: " << at << std::endl;
}

void part2(std::istream &input)
{
    int at = 0;
    char buf;
    int index = 0;

    for (;;) {
        input.read(&buf, 1);

        if (input.eof()) {
            break;
        }

        index++;

        switch(buf) {
        case '(':
            at++;
            break;
        case ')':
            at--;
            break;
        }

        if (at < 0) {
            break;
        }
    }

    std::cout << "Answer 2: " << index << std::endl;
}
