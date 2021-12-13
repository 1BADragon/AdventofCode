#include <vector>
#include <iostream>
#include <unordered_set>
#include <advent.hpp>
#include <stdexcept>
#include <string_utils.hpp>


struct pair_hash
{
    template <class T1, class T2>
    std::size_t operator() (const std::pair<T1, T2> &pair) const {
        return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
    }
};

struct Paper {
    Paper(std::istream &input)
    {
        auto lines = utils::lines(input);

        for (auto &l : lines) {
            if (l.empty()) {
                continue;
            }
            if (l.substr(0, 4) == "fold") {
                auto amount = parse<int>(l.substr(13));
                auto dir = l.substr(11, 1);

                if (dir == "x") {
                    folds.push_back(std::make_pair(0, amount));
                } else if (dir == "y") {
                    folds.push_back(std::make_pair(1, amount));
                } else {
                    throw std::runtime_error("Bad fold dir " + dir);
                }
            } else {
                auto cords = parse<int>(split(l, ","));

                _points.insert(std::make_pair(cords.at(0), cords.at(1)));
            }
        }
    }

    void do_fold()
    {
        auto f = folds[fold_number];
        std::unordered_set<std::pair<int, int>, pair_hash> new_points;

        for (auto &point : this->_points) {
            int x = point.first;
            int y = point.second;
            if (f.first == 0) {
                if (x >= f.second) {
                    x = f.second - std::abs(x - f.second);
                }
            } else if (f.first == 1) {
                if (y >= f.second) {
                    y = f.second - std::abs(y - f.second);
                }
            }

            new_points.insert(std::make_pair(x, y));
        }

        this->_points = new_points;

        fold_number++;
    }

    void do_folds()
    {
        while(fold_number < folds.size()) {
            do_fold();
        }
    }

    size_t n_dots() const
    {
        return _points.size();
    }

    friend std::ostream& operator<<(std::ostream &os, const Paper &p) {
        int w = 0;
        int h = 0;

        for (auto &point : p._points) {
            w = std::max(w, point.first + 1);
            h = std::max(h, point.second + 1);
        }

        for (int y = 0; y < h; ++y) {
            for (int x = 0; x < w; ++x) {
                auto point = std::make_pair(x, y);
                if (p._points.contains(point)) {
                    os << "#";
                } else {
                    os << ".";
                }
            }
            os << std::endl;
        }

        return os;
    }

    std::unordered_set<std::pair<int, int>, pair_hash> _points;
    std::vector<std::pair<int, int>> folds;
    int fold_number = 0;
};

void part1(std::istream &input)
{
    Paper p(input);

    p.do_fold();

    std::cout << "Answer 1: " << p.n_dots() << std::endl;
}

void part2(std::istream &input)
{
    Paper p(input);
    p.do_folds();

    std::cout << "Answer 2:\n" << p << std::endl;
}
