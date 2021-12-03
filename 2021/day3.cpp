#include <array>
#include <sstream>
#include <iostream>
#include <vector>
#include <bitset>
#include <advent.hpp>

static size_t input_size = 0;

void filter_out(std::vector<std::string> &vec, size_t pos, char val);
std::vector<size_t> count_pos(std::vector<std::string> _vals);

void part1(std::istream &input)
{
    std::vector<std::string> lines;
    std::vector<size_t> _vals;

    std::vector<std::string> v_o2;
    std::vector<std::string> v_co2;

    size_t total = 0;
    std::vector<size_t> counts = {0};

    for(;;) {
        std::string line;
        std::getline(input, line);

        if (input.eof()) {
            break;
        }

        lines.push_back(line);

        total++;

        for (size_t i = 0; i < line.size(); ++i) {
            if (line[i] == '\n') {
                break;
            }

            if (counts.size() <= i) {
                counts.push_back(0);
            }

            if (line[i] == '1') {
                counts[i]++;
            }
        }

        _vals.push_back(std::stoi(line, nullptr, 2));
    }

    input_size = counts.size();

    v_o2 = lines;
    v_co2 = lines;

    std::string s_gamma;
    std::string s_epsilon;

    for (size_t &v : counts) {
        if (v < (total / 2)) {
            std::vector<size_t> tmp;

            s_gamma.push_back('1');
            s_epsilon.push_back('0');
        } else {
            s_gamma.push_back('0');
            s_epsilon.push_back('1');
        }
    }

    size_t pos = 0;
    while (v_o2.size() > 1) {
        auto counts = count_pos(v_o2);

        if (counts[pos] >= (v_o2.size() - counts[pos])) {
            filter_out(v_o2, pos, '0');
        } else {
            filter_out(v_o2, pos, '1');
        }

        pos++;
    }

    pos = 0;
    while (v_co2.size() > 1) {
        auto counts = count_pos(v_co2);

        if (counts[pos] < (v_co2.size() - counts[pos])) {
            filter_out(v_co2, pos, '0');
        } else {
            filter_out(v_co2, pos, '1');
        }

        pos++;
    }

    size_t gamma = std::stoi(s_gamma, nullptr, 2);
    size_t epsilon = std::stoi(s_epsilon, nullptr, 2);

    size_t o2 = std::stoi(*v_o2.begin(), nullptr, 2);
    size_t co2 = std::stoi(*v_co2.begin(), nullptr, 2);

    std::cout << "gamma: " << gamma <<  "\nepsilon: " << epsilon << std::endl;
    std::cout << "o2: " << o2 << "\nco2: " << co2 << std::endl;

    std::cout << "Answer1: " << gamma * epsilon << std::endl;
    std::cout << "Answer2: " << o2 * co2 << std::endl;
}

void filter_out(std::vector<std::string> &vec, size_t pos, char val)
{
    auto it = vec.begin();
    while (it != vec.end()) {
        if ((*it)[pos] == val) {
            it = vec.erase(it);
        } else {
            it++;
        }
    }
}

std::vector<size_t> count_pos(std::vector<std::string> _vals)
{
    std::vector<size_t> ret;

    for (auto &v : _vals) {
        for (size_t i = 0; i < v.size(); ++i) {
            if (v[i] == '\n') {
                break;
            }

            if (ret.size() <= i) {
                ret.push_back(0);
            }

            if (v[i] == '1') {
                ret[i]++;
            }
        }
    }

    return ret;
}

