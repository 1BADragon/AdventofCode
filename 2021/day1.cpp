#include <iostream>
#include <array>
#include <fstream>
#include <string>
#include <numeric>

static void part1(const std::string &filename);
static void part2(const std::string &filename);

int main(int argc, char **argv)
{
    if (argc < 2) {
        std::cerr << "Invalid input" << std::endl;
        return -1;
    }

    part1(argv[1]);
    part2(argv[1]);

    return 0;
}

static void part1(const std::string &filename)
{
    std::ifstream infile(filename);

    int prev;
    int curr;
    int counter = 0;

    infile >> prev;

    while (!infile.eof())
    {
        infile >> curr;

        if (infile.eof()) {
            break;
        }

        if (curr > prev) {
            counter++;
        }

        prev = curr;
    }

    std::cout << "Answer 1: " << counter << std::endl;

    infile.close();
}

static void part2(const std::string &filename)
{
    std::array<int, 3> win;
    std::ifstream infile(filename);

    infile >> win[0];
    infile >> win[1];
    infile >> win[2];

    int curr;
    int prev;
    int counter = 0;

    prev = std::accumulate(win.begin(), win.end(), 0);

    while (!infile.eof())
    {
        win[0] = win[1];
        win[1] = win[2];

        infile >> win[2];

        if (infile.eof()) {
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