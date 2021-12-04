
#include <vector>
#include <array>
#include <cassert>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <advent.hpp>
#include <string_utils.hpp>

struct BingoCell {
    int val;
    bool chosen;
};

using Board = std::array<std::array<BingoCell, 5>, 5>;

static void parse_bingo(std::istream &input, std::vector<int> &numbers, std::vector<Board> &boards);
static bool mark_board(Board &b, int val);
static int board_score(const Board &b);
static void print_board(const Board &b);

void part1(std::istream &input)
{
    std::vector<int> numbers;
    std::vector<Board> boards;

    parse_bingo(input, numbers, boards);

//    std::cout << "Found " << numbers.size() <<  " numbers and " << boards.size()
//              << " boards" << std::endl;

//    for (auto &b : boards) {
//        print_board(b);
//        std::cout << std::endl;
//    }

    for (int num : numbers) {
        for (auto &b : boards) {
            if (mark_board(b, num)) {
                std::cout << "Answer 1: " << board_score(b) * num << std::endl;
                return;
            }
        }
    }
}

void part2(std::istream &input)
{
    std::vector<int> numbers;
    std::vector<Board> boards;

    parse_bingo(input, numbers, boards);

//    std::cout << "Found " << numbers.size() <<  " numbers and " << boards.size()
//              << " boards" << std::endl;

//    for (auto &b : boards) {
//        print_board(b);
//        std::cout << std::endl;
//    }

    for (int num : numbers) {
        auto it = boards.begin();
        while (it != boards.end()) {
            bool win = mark_board(*it, num);

            if (boards.size() > 1) {
                if (win) {
                    it = boards.erase(it);
                } else {
                    it++;
                }
            } else if (win) {
                std::cout << "Answer 2: " << board_score(boards[0]) * num << std::endl;
                return;
            } else {
                it++;
            }
        }
    }
}

static void parse_bingo(std::istream &input, std::vector<int> &numbers, std::vector<Board> &boards)
{
    std::string line;

    std::getline(input, line);

    auto line_nums = parse<int>(split(line, ","));
    numbers.insert(numbers.end(), line_nums.begin(), line_nums.end());

    std::getline(input, line); // blank line

    for (;;) {
        Board curr;

        for (size_t i = 0; i < 5; ++i) {
            std::getline(input, line);
            if (input.eof()) {
                return;
            }

            std::istringstream iss(line);

            for (size_t j = 0; j < 5; ++j) {
                iss >> curr[i][j].val;
                curr[i][j].chosen = false;
            }
        }

        std::getline(input, line); // blank line

        boards.push_back(curr);
    }
}

static bool mark_board(Board &b, int val)
{
    bool win = false;
    for (size_t i = 0; i < 5; ++i) {
        for (size_t j = 0; j < 5; ++j) {
            if (b[i][j].val == val) {
                b[i][j].chosen = true;

                win = true;
                for (size_t k = 0; k < 5; ++k) {
                    if (b[k][j].chosen == false) {
                        win = false;
                        break;
                    }
                }

                if (win) {
                    return true;
                }

                win = true;
                for (size_t k = 0; k < 5; ++k) {
                    if (b[i][k].chosen == false) {
                        win = false;
                        break;
                    }
                }

                if (win) {
                    return true;
                }
            }
        }
    }

    return false;
}

static int board_score(const Board &b)
{
    int sum = 0;

    for(size_t i = 0; i < 5; ++i) {
        for (size_t j = 0; j < 5; ++j) {
            if (!b[i][j].chosen) {
                sum += b[i][j].val;
            }
        }
    }

    return sum;
}

static void print_board(const Board &b)
{
    for (size_t i = 0; i < 5; ++i) {
        for (size_t j = 0; j < 5; ++j) {
            if (b[i][j].chosen) {
                fprintf(stdout, "\e[1m");
            }
            fprintf(stdout, "%-2d ", b[i][j].val);
            fprintf(stdout, "\e[0m");
        }
        fprintf(stdout, "\n");
    }

    fflush(stdout);
}
