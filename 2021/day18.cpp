#include <sstream>
#include <variant>
#include <memory>
#include <stdexcept>
#include <iostream>
#include <cassert>
#include <advent.hpp>
#include <string_utils.hpp>

struct SnailFishNumber;
using SnailFishNumberPtr = std::shared_ptr<SnailFishNumber>;

struct SnailFishNumber {
public:
    using Value = std::variant<std::shared_ptr<SnailFishNumber>, int>;

    Value left;
    Value right;
    std::shared_ptr<SnailFishNumber> parent;

    void replace(Value at, Value with) {
        if (left == at) {
            left = with;
        } else if (right == at) {
            right = with;
        } else {
            throw std::runtime_error("Not a child of parent");
        }
    }

    bool is_left_child() const {
        auto parent_left = parent->left;
        if (parent_left.index() == 1) {
            return false;
        }

        return std::get<0>(parent_left).get() == this;
    }

    bool is_right_child() const {
        auto parent_right = parent->right;
        if (parent_right.index() == 1) {
            return false;
        }

        return std::get<0>(parent_right).get() == this;
    }

    void propagate_back(int val)
    {
        if (!parent) {
            return;
        }

        if (is_right_child()) {
            if (parent->left.index() == 1) {
                std::get<1>(parent->left) += val;
                return;
            } else {
                std::get<0>(parent->left)->propagate_right(val);
            }
        } else if (is_left_child()) {
            parent->propagate_back(val);
        }
    }

    void propagate_fwd(int val)
    {
        if (!parent) {
            return;
        }

        if (is_left_child()) {
            if (parent->right.index() == 1) {
                std::get<1>(parent->right) += val;
                return;
            } else {
                std::get<0>(parent->right)->propagate_left(val);
            }

        } else if (is_right_child()) {
            parent->propagate_fwd(val);
        }
    }

    void propagate_right(int val)
    {
        if (right.index() == 1) {
            std::get<1>(right) += val;
        } else {
            std::get<0>(right)->propagate_right(val);
        }
    }

    void propagate_left(int val)
    {
        if (left.index() == 1) {
            std::get<1>(left) += val;
        } else {
            std::get<0>(left)->propagate_left(val);
        }
    }
};


static SnailFishNumberPtr add(SnailFishNumberPtr lhs, SnailFishNumberPtr rhs);
static void reduce(SnailFishNumberPtr num);
static uint64_t magnitude(const SnailFishNumberPtr num);
static SnailFishNumberPtr parse_number(std::istream &&ss);
static SnailFishNumberPtr parse_number(std::istream &ss);
static std::string to_string(const SnailFishNumberPtr num);

void part1(std::istream &input)
{
    auto lines = utils::lines(input);

    auto cur = add(parse_number(std::stringstream(lines[0])),
                   parse_number(std::stringstream(lines[1])));
    reduce(cur);

    for (size_t i = 2; i < lines.size(); ++i) {
        auto num = parse_number(std::stringstream(lines[i]));

        cur = add(cur, num);
        reduce(cur);

    }

    std::cout << "Final: " << to_string(cur) << std::endl;
    std::cout << "Answer 1: " << magnitude(cur) << std::endl;
}

void part2(std::istream &input)
{
    auto lines = utils::lines(input);

    uint64_t max = 0;
    SnailFishNumberPtr max_num;

    for (size_t i = 0; i < lines.size(); ++i) {
        for (size_t j = 0; j < lines.size(); ++j) {
            if (i == j) {
                continue;
            }
            auto n1 = parse_number(std::stringstream(lines[i]));
            auto n2 = parse_number(std::stringstream(lines[j]));

            auto sum = add(n1, n2);
            reduce(sum);

            uint64_t mag = magnitude(sum);
            if (mag > max) {
                max = mag;
                max_num = sum;
            }
        }
    }

    std::cout << "Max number: " << to_string(max_num) << std::endl;
    std::cout << "Answer 2: " << max << std::endl;
}

static SnailFishNumberPtr add(SnailFishNumberPtr lhs, SnailFishNumberPtr rhs)
{
    auto res = std::make_shared<SnailFishNumber>();

    res->left = lhs;
    res->right = rhs;

    return res;
}


static void make_parent(SnailFishNumberPtr num) {
    if (num->left.index() == 0) {
        std::get<0>(num->left)->parent = num;
    }

    if (num->right.index() == 0) {
        std::get<0>(num->right)->parent = num;
    }
}

static SnailFishNumber::Value split(SnailFishNumberPtr num, int depth, bool &changed)
{
    // look at left child
    if (num->left.index() == 0) {
        bool this_changed = false;
        num->left = split(std::get<0>(num->left), depth+1, this_changed);

        if (this_changed) {
            changed = true;
            return num;
        }
    } else {
        int val = std::get<1>(num->left);
        if (val >= 10) {
            auto new_val = std::make_shared<SnailFishNumber>();
            new_val->left = val / 2;
            new_val->right = val / 2 + (val % 2);
            new_val->parent = num;

            num->left = new_val;
            changed = true;
            return num;
        }
    }

    // look at right child
    if (num->right.index() == 0) {
        bool this_changed = false;
        num->right = split(std::get<0>(num->right), depth+1, this_changed);

        if (this_changed) {
            changed = true;
            return num;
        }
    } else {
        int val = std::get<1>(num->right);
        if (val >= 10) {
            auto new_val = std::make_shared<SnailFishNumber>();
            new_val->left = val / 2;
            new_val->right = val / 2 + (val % 2);
            new_val->parent = num;

            num->right = new_val;
            changed = true;
            return num;
        }
    }

    changed = false;
    return num;
}

static SnailFishNumber::Value explodes(SnailFishNumberPtr num, int depth, bool &changed)
{
    if (depth > 4) {
        assert(num->left.index());
        assert(num->right.index());

        num->propagate_back(std::get<1>(num->left));
        num->propagate_fwd(std::get<1>(num->right));

        changed = true;

        return 0;
    }

    if (num->left.index() == 0) {
        bool this_changed = false;
        num->left = explodes(std::get<0>(num->left), depth+1, this_changed);
        make_parent(num);
        if (this_changed) {
            changed = true;
            return num;
        }
    }

    if (num->right.index() == 0) {
        bool this_changed = false;
        num->right = explodes(std::get<0>(num->right), depth+1, this_changed);
        make_parent(num);
        if (this_changed) {
            changed = true;
            return num;
        }
    }

    changed = false;
    return num;
}

static void reduce(SnailFishNumberPtr num)
{
    for(;;) {
        if (num->left.index() == 0) {
            bool this_change = false;
            num->left = explodes(std::get<0>(num->left), 2, this_change);
            make_parent(num);
            if (num->left.index() == 0) {
                std::get<0>(num->left)->parent = num;
            }

            if (this_change) {
                continue;
            }
        }

        if (num->right.index() == 0) {
            bool this_change = false;
            num->right = explodes(std::get<0>(num->right), 2, this_change);
            make_parent(num);
            if (num->right.index() == 0) {
                std::get<0>(num->right)->parent = num;
            }

            if (this_change) {
                continue;
            }
        }

        if (num->left.index() == 0) {
            bool this_change = false;
            num->left = split(std::get<0>(num->left), 2, this_change);
            make_parent(num);
            if (num->left.index() == 0) {
                std::get<0>(num->left)->parent = num;
            }

            if (this_change) {
                continue;
            }
        }

        if (num->right.index() == 0) {
            bool this_change = false;
            num->right = split(std::get<0>(num->right), 2, this_change);
            make_parent(num);
            if (num->right.index() == 0) {
                std::get<0>(num->right)->parent = num;
            }

            if (this_change) {
                continue;
            }
        }

        break;
    };
}

static uint64_t magnitude(const SnailFishNumberPtr num)
{
    uint64_t left = 0;
    uint64_t right = 0;

    // left first
    if (num->left.index() == 0) {
        left = magnitude(std::get<0>(num->left));
    } else {
        left = std::get<1>(num->left);
    }

    if (num->right.index() == 0) {
        right = magnitude(std::get<0>(num->right));
    } else {
        right = std::get<1>(num->right);
    }

    return 3 * left + 2 * right;
}

static SnailFishNumberPtr parse_number(std::istream &&ss)
{
    return parse_number(ss);
}

static SnailFishNumberPtr parse_number(std::istream &ss)
{
    auto res = std::make_shared<SnailFishNumber>();
    char c;

    c = ss.get();

    if (c != '[') {
        throw std::runtime_error("Invalid char");
    }

    c = ss.peek();

    if (c == '[') {
        auto child = parse_number(ss);
        child->parent = res;
        res->left = child;
    } else {
        ss.get();
        res->left = c - '0';
    }

    c = ss.get();
    if (c != ',') {
        throw std::runtime_error("Invalid char");
    }

    c = ss.peek();
    if (c == '[') {
        auto child = parse_number(ss);
        child->parent = res;
        res->right = child;
    } else {
        ss.get();
        res->right = c - '0';
    }

    c = ss.get();
    if (c != ']') {
        throw std::runtime_error("Invalid char");
    }

    return res;
}

static std::string to_string(const SnailFishNumberPtr num)
{
    std::stringstream ss;

    ss << '[';
    if (num->left.index() == 0) {
        ss << to_string(std::get<0>(num->left));
    } else {
        ss << std::get<1>(num->left);
    }

    ss << ',';

    if (num->right.index() == 0) {
        ss << to_string(std::get<0>(num->right));
    } else {
        ss << std::get<1>(num->right);
    }

    ss << ']';
    return ss.str();
}
