#include <regex>
#include <stdexcept>
#include <iostream>
#include <cstdio>
#include <sstream>
#include <advent.hpp>
#include <string_utils.hpp>

// target area: x=20..30, y=-10..-5

struct Point {
    int x;
    int y;

    std::string to_string() const {
        std::stringstream ss;
        ss << "(" << x << "," << y << ")";
        return ss.str();
    }
};

struct Dim {
    Point p1;
    Point p2;
};

Dim parse_input(std::istream &input);
void step(Point &p, int &vx, int &vy);
int test(const Dim &d, const Point &p);


void part1(std::istream &input)
{
    auto dim = parse_input(input);

    printf("dim: (%d, %d), (%d, %d)\n", dim.p1.x, dim.p1.y, dim.p2.x, dim.p2.y);

    int max_y = 0;
    int count = 0;

    for (int start_xv = 1; start_xv < 300; ++start_xv) {
        for (int start_yv = -250; start_yv < 250; ++start_yv) {
            Point p{0, 0};
            int xv = start_xv;
            int yv = start_yv;
            int rc;
            int this_max = 0;

            do {
                this_max = std::max(this_max, p.y);

                step(p, xv, yv);
                rc = test(dim, p);
            } while (rc > 0);

            if (rc == 0) {
                count ++;
                max_y = std::max(this_max, max_y);
            }
        }
    }

    std::cout << "Answer 1: " << max_y << std::endl;
    std::cout << "Answer 2: " << count << std::endl;
}

Dim parse_input(std::istream &input)
{
    std::regex re("target area: x=(-?[0-9]+)..(-?[0-9]+), y=(-?[0-9]+)..(-?[0-9]+)");
    std::string line;
    std::smatch match;

    std::getline(input, line);

    if (!std::regex_match(line, match, re)) {
        throw std::runtime_error("Re does not match");
    }

    int x1 = parse<int>(match[1].str());
    int x2 = parse<int>(match[2].str());
    int y1 = parse<int>(match[3].str());
    int y2 = parse<int>(match[4].str());


    auto p1 = Point{std::min(x1, x2), std::min(y1, y2)};
    auto p2 = Point{std::max(x1, x2), std::max(y1, y2)};

    return Dim{p1, p2};
}

void step(Point &p, int &vx, int &vy)
{
    p.x += vx;
    p.y += vy;

    if (vx > 0) {
        vx--;
    } else if (vx < 0) {
        vx++;
    }

    vy--;
}

int test(const Dim &d, const Point &p)
{
    if (p.x > d.p2.x || p.y < d.p1.y) {
        return -1;
    }

    if (p.x >= d.p1.x && p.x <= d.p2.x &&
            p.y >= d.p1.y && p.y <= d.p2.y)
    {
        return 0;
    }

    return 1;
}
