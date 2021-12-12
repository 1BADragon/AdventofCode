#include <map>
#include <memory>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <advent.hpp>
#include <string_utils.hpp>

class Cave {
public:
    Cave(const std::string &c) : _c(c), _visited_count(0) {

    }

    bool can_visit() const
    {
        if (is_start()) {
            return false;
        }

        if (is_end()) {
            return true;
        }

        if (is_small()) {
            return _visited_count == 0;
        } else {
            return true;
        }
    }

    bool is_small() const
    {
        char c = _c[0];

        if (c >= 'a' && c <= 'z') {
            return true;
        }

        return false;
    }

    bool can_visit2() const
    {
        if (is_small() && _visited_count > 2) {
            throw std::runtime_error("_visited_count greater than 2");
        }

        if (is_start()) {
            return false;
        }

        if (is_end()) {
            return true;
        }

        if (is_small()) {
            if (_visited_count == 0) {
                return true;
            } else if (_visited_count == 1 && _2visited == nullptr) {
                return true;
            } else {
                return false;
            }
        } else {
            return true;
        }
    }

    bool is_start() const
    {
        return _c == "start";
    }

    bool is_end() const
    {
        return _c == "end";
    }

    std::string name() const
    {
        return _c;
    }

    void visit()
    {
        if (_visited_count == 1 && is_small()) {
            _2visited = this;
        }
        _visited_count++;
    }

    void reset() {
        if (is_small() && _visited_count == 2 && _2visited != this) {
            std::stringstream ss;
            ss << "Bad state: " << name() << "--" << _visited_count
               << " :";
            if (_2visited != nullptr) {
                ss << _2visited->name();
            } else {
                ss << "NULL";
            }
            throw std::runtime_error(ss.str());
        }
        if (_visited_count == 2 && _2visited == this) {
            _visited_count = 1;
            _2visited = nullptr;
        } else {
            _visited_count = 0;
        }
    }

private:
    std::string _c;
    int _visited_count;
    static const Cave* _2visited;
};

const Cave* Cave::_2visited;

class CaveMap {
public:
    using CavePtr = std::shared_ptr<Cave>;

    CaveMap(std::istream &input, bool visit2 = false) : _visit2(visit2)
    {
        while (true) {
            std::string line;
            std::getline(input, line);
            if (input.eof()) {
                break;
            }

            auto parts = split(line, "-");

            CavePtr a;
            CavePtr b;

            if (_caves.contains(parts[0])) {
                a = _caves.at(parts[0]);
            } else {
                a = std::make_shared<Cave>(parts[0]);
                _caves[parts[0]] = a;
            }

            if (_caves.contains(parts[1])) {
                b = _caves.at(parts[1]);
            } else {
                b = std::make_shared<Cave>(parts[1]);
                _caves[parts[1]] = b;
            }

            _connections[a->name()].push_back(b);
            _connections[b->name()].push_back(a);
        }
    }

    std::vector<std::vector<CavePtr>> get_all_paths() const {
        std::vector<std::vector<CavePtr>> res;
        auto start = _caves.at("start");
        get_paths_starting_from(start, res, {});
        return res;
    }

    void get_paths_starting_from(CavePtr start,
                                 std::vector<std::vector<CavePtr>> &res,
                                 std::vector<CavePtr> curr) const {
        start->visit();
        curr.push_back(start);
        if (start->is_end()) {
            res.push_back(curr);
            return;
        }

        for (auto &conn : _connections.at(start->name())) {
            if (!_visit2) {
                if (conn->can_visit()) {
                    get_paths_starting_from(conn, res, curr);
                    conn->reset();
                }
            } else {
                if (conn->can_visit2()) {
                    get_paths_starting_from(conn, res, curr);
                    conn->reset();
                }
            }
        }
    }

private:
    bool _visit2;
    std::map<std::string, CavePtr> _caves;
    std::map<std::string, std::vector<CavePtr>> _connections;
};

void part1(std::istream &input)
{
    CaveMap map(input);

    auto paths = map.get_all_paths();

    std::cout << "Answer 1: " << paths.size() << std::endl;
}

void part2(std::istream &input)
{
    CaveMap map(input, true);

    auto paths = map.get_all_paths();

//    for (auto &path : paths) {
//        for (auto &c : path) {
//            std::cout << c->name() << " ";
//        }
//        std::cout << std::endl;
//    }

    std::cout << "Answer 2: " << paths.size() << std::endl;
}
