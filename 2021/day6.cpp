#include <iostream>
#include <vector>
#include <array>
#include <numeric>
#include <string_utils.hpp>
#include <advent.hpp>

struct State {
    size_t day;
    std::array<uint64_t, 9> count;
};

static void simulate(State &state, size_t days);
static void pass_day(State &state);
static void print_state(const State &state);

void part1(std::istream &input)
{
    std::string line;

    std::getline(input, line);

    State s = {
        .day = 0,
        .count = {0},
    };

    std::vector<int> nums = parse<int>(split(line, ","));

    for (auto n : nums) {
        s.count[n]++;
    }

    simulate(s, 80);

    std::cout << "\rAnswer 1: " << std::accumulate(s.count.begin(), s.count.end(), 0) << std::endl;
}

void part2(std::istream &input)
{
    std::string line;

    std::getline(input, line);

    State s = {
        .day = 0,
        .count = {0},
    };

    std::vector<int> nums = parse<int>(split(line, ","));

    for (auto n : nums) {
        s.count[n]++;
    }

    simulate(s, 256);

    std::cout << "\rAnswer 2: " << std::accumulate(s.count.begin(), s.count.end(), 0llu) << std::endl;
}

static void simulate(State &state, size_t days)
{
    for (int i = 0; i < days; ++i) {
        pass_day(state);

        std::cout << "\rDay " << i << std::flush;

        //std::cout << "Day " << i+1 << ": ";
        //print_state(s);
    }
}

static void pass_day(State &state)
{
    size_t repro_date = (state.day + 7) % state.count.size();

    state.count[repro_date] += state.count[state.day];

    state.day = (state.day + 1) % state.count.size();
}

static void print_state(const State &state)
{
    size_t s = state.day;

    do {
        std::cout << state.count[s];

        s = (s + 1) % state.count.size();

    } while(s != state.day);

    std::cout << std::endl;
}
