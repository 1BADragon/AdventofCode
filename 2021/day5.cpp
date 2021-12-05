
#include <advent.hpp>
#include <cstdio>
#include <tuple>
#include <vector>
#include <iostream>
#include <sstream>

#include <string_utils.hpp>

using Board = std::vector<std::vector<int>>;

struct Point {
    int x;
    int y;
};

struct Segment {
    Point p1;
    Point p2;
};

std::vector<Segment> parse_input(std::istream &input);
bool is_vert_hor(const Segment &s);
void adjust_size(const Segment &s, Board &board);
void draw_line(const Segment &s, Board &board);
void print_board(const Board &b);
std::string to_string(const Segment &s);

void part1(std::istream &input)
{
    Board board;
    auto segments = parse_input(input);

    for (auto s : segments) {
        if (!is_vert_hor(s)) {
//            std::cout << "Skipping " << to_string(s) << std::endl;
            continue;
        }

        adjust_size(s, board);
        draw_line(s, board);
    }

    size_t n = 0;
    for (auto &line : board) {
        for (auto &cell : line) {
            if (cell > 1) {
                n++;
            }
        }
    }

//    print_board(board);

    std::cout << "Answer 1: " << n << std::endl;
}

void part2(std::istream &input)
{
    Board board;
    auto segments = parse_input(input);

    for (auto s : segments) {
        adjust_size(s, board);
        draw_line(s, board);
    }

    size_t n = 0;
    for (auto &line : board) {
        for (auto &cell : line) {
            if (cell > 1) {
                n++;
            }
        }
    }

//    print_board(board);

    std::cout << "Answer 1: " << n << std::endl;
}

std::vector<Segment> parse_input(std::istream &input)
{
    std::vector<Segment> ret;
    for (;;) {
        std::string line;

        std::getline(input, line);

        if (input.eof()) {

            break;
        }

        auto points = split(line, "->");

        auto p1_vals = parse<int>(split(points[0], ","));
        auto p2_vals = parse<int>(split(points[1], ","));

        Segment s;

        s.p1.x = p1_vals[0];
        s.p1.y = p1_vals[1];

        s.p2.x = p2_vals[0];
        s.p2.y = p2_vals[1];

        ret.push_back(s);
    }

    return ret;
}

bool is_vert_hor(const Segment &s)
{
    return s.p1.x == s.p2.x || s.p1.y == s.p2.y;
}

void adjust_size(const Segment &s, std::vector<std::vector<int>> &board)
{
    bool change = false;

    size_t x = std::max(s.p1.x, s.p2.x) + 1;
    size_t y = std::max(s.p1.y, s.p2.y) + 1;

    if (board.size() != 0) {
        x = std::max(board.at(0).size(), x);

        if (x != board.at(0).size()) {
            change = true;
        }
    }

    if (y > board.size()) {
        change = true;
        board.resize(y);
    }

    if (change) {
//        std::cout << "X Resize to " << x << std::endl;
        for (auto &row : board) {
            row.resize(x, 0);
        }
    }
}

void draw_line(const Segment &s, std::vector<std::vector<int>> &board)
{
    if (s.p1.x == s.p2.x) {
        if (s.p1.y <= s.p2.y) {
            for (int i = s.p1.y; i <= s.p2.y; ++i) {
                board.at(i).at(s.p1.x)++;
            }
        } else {
            for (int i = s.p1.y; i >= s.p2.y; --i) {
                board.at(i).at(s.p1.x)++;
            }
        }
    } else if (s.p1.y == s.p2.y) {
        if (s.p1.x <= s.p2.x) {
            for (int i = s.p1.x; i <= s.p2.x; ++i) {
                board.at(s.p1.y).at(i)++;
            }
        } else {
            for (int i = s.p1.x; i >= s.p2.x; --i) {
                board.at(s.p1.y).at(i)++;
            }
        }
    } else {
        Point start;
        Point end;

        if (s.p1.x < s.p2.x) {
            start = s.p1;
            end = s.p2;
        } else {
            start = s.p2;
            end = s.p1;
        }

        int y = start.y;
        for (int x = start.x; x <= end.x; ++x) {
            board.at(y).at(x)++;

            if (start.y < end.y) {
                y++;
            } else {
                y--;
            }
        }
    }
}

void print_board(const Board &b)
{
    for (auto &row : b) {
        for (auto &c : row) {
            std::cout << c << " ";
        }
        std::cout << std::endl;
    }
}

std::string to_string(const Segment &s)
{
    std::stringstream ss;
    ss << s.p1.x << "," << s.p1.y << " -> " << s.p2.x << "," << s.p2.y;

    return ss.str();
}
