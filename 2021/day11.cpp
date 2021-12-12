#include <vector>
#include <iostream>
#include <advent.hpp>

class Octopus {
public:
    Octopus(uint32_t level): _level(level), _flashed(false) {}

    void charge() {
        _level++;
    }

    bool flash() {
        if (!_flashed && _level > 9) {
            _total_flashes++;
            _flashed = true;
            return true;
        }

        return false;
    }

    void reset() {
        if (_flashed) {
            _level = 0;
        }
        _flashed = false;
    }

    static uint64_t total_flashes() {
        return _total_flashes;
    }

    uint32_t level() const {
        return _level;
    }

private:
    uint32_t _level;
    bool _flashed;
    static uint64_t _total_flashes;
};

uint64_t Octopus::_total_flashes = 0;

class OctopusMap {
public:
    OctopusMap(std::istream &input) :
        _data()
    {
        while (true)
        {
            std::string s;
            std::getline(input, s);

            if (input.eof()) {
                break;
            }

            std::vector<Octopus> row;

            for (char c : s) {
                int val = c - '0';

                row.emplace_back(val);
            }

            _data.push_back(row);
        }
    }

    void step() {
        // first the level of all octopi are increased
        for (auto &row : _data) {
            for (auto &o : row) {
                o.charge();
            }
        }

        bool flash_occured = false;
        do {
            flash_occured = false;
            for (size_t y = 0; y < _data.size(); ++y) {
                for (size_t x = 0; x < _data[y].size(); ++x) {
                    if (_data[y][x].flash()) {
                        flash_occured = true;
                        check_octo(x, y);
                    }
                }
            }
        } while(flash_occured);

        // third reset all octopi
        for (auto &row : _data) {
            for (auto &o : row) {
                o.reset();
            }
        }
    }

    void inc_octo(size_t x, size_t y) {
        if (y >= _data.size()) {
            return;
        }

        if (x >= _data[y].size()) {
            return;
        }

        _data[y][x].charge();
    }

    void check_octo(size_t x, size_t y) {
        if (y >= _data.size()) {
            return;
        }

        if (x >= _data[y].size()) {
            return;
        }

        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                if (i == 0 && j == 0) {
                    continue;
                }

                inc_octo(x + i, y + j);
            }
        }
    }

    uint64_t total_octos() const
    {
        uint64_t sum = 0;
        for (auto &row : _data) {
            sum += row.size();
        }

        return sum;
    }

    friend std::ostream& operator<<(std::ostream &o, OctopusMap &om)
    {
        for (auto &row : om._data) {
            std::string str;
            for (auto &o : row) {
                str += std::to_string(o.level());
            }
            o << str << "\n";
        }

        return o;
    }

private:
    std::vector<std::vector<Octopus>> _data;
};

void part1(std::istream &input)
{
    OctopusMap map(input);
    //std::cout << map << std::endl;

    for (size_t i = 0; i < 100; ++i) {
        map.step();
        //std::cout << "After step " << i << ":\n" << map << std::endl;
    }

    std::cout << "Answer 1: " << Octopus::total_flashes() << std::endl;
}

void part2(std::istream &input)
{
    OctopusMap map(input);

    for (size_t i = 0; i < 1000000; ++i) {
        uint64_t last_flashes = Octopus::total_flashes();
        map.step();

        if (last_flashes + map.total_octos() == Octopus::total_flashes()) {
            std::cout << "Answer 2: " << i + 1 << std::endl;
            break;
        }
    }
}
