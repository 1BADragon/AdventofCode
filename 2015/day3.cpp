#include <unordered_map>
#include <tuple>
#include <stdexcept>
#include <iostream>
#include <advent.hpp>

struct pair_hash
{
    template <class T1, class T2>
    std::size_t operator() (const std::pair<T1, T2> &pair) const {
        return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
    }
};

class HouseMap {
public:
    void move(char c) {
        switch(c) {
        case '^':
            y++;
            break;
        case '<':
            x--;
            break;
        case '>':
            x++;
            break;
        case 'v':
            y--;
            break;
        default:
            throw std::runtime_error("Invalid move");
        }

        inc_loc();
    }

    void inc_loc() {
        auto cord = std::make_pair(x, y);

        if (houses.contains(cord)) {
            houses.at(cord)++;
        } else {
            houses[cord] = 1;
        }
    }

    uint64_t count_morethan(int val) {
        uint64_t total = 0;
        for (auto &h : houses) {
            if (h.second > val) {
                total++;
            }
        }

        return total;
    }

    void append(const HouseMap &h)
    {
        for (auto house : h.houses) {
            if (this->houses.contains(house.first)) {
                this->houses.at(house.first) += house.second;
            } else {
                this->houses[house.first] = house.second;
            }
        }
    }

    HouseMap operator+(const HouseMap &h)
    {
        HouseMap res;
        res.append(*this);
        res.append(h);

        return res;
    }

private:
    int x = 0;
    int y = 0;
    std::unordered_map<std::pair<int, int>, int, pair_hash> houses;
};

void part1(std::istream &input)
{
    std::string line;
    HouseMap m;

    m.inc_loc();
    std::getline(input, line);
    for (auto c : line) {
        m.move(c);
    }

    std::cout << "Answer 1: " << m.count_morethan(0) << std::endl;
}

void part2(std::istream &input)
{
    std::string line;
    HouseMap santa;
    HouseMap robot;

    santa.inc_loc();
    robot.inc_loc();
    std::getline(input, line);
    for(auto it = line.begin(); it != line.end(); ++it) {
        santa.move(*it);
        it++;
        robot.move(*it);
    }

    std::cout << "Answer 2: " << (santa + robot).count_morethan(0) << std::endl;
}
