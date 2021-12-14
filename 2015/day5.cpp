#include <iostream>
#include <set>
#include <advent.hpp>

static bool is_nice(const std::string &s);
static bool is_nice2(const std::string &s);

static bool has_voules(const std::string &s);
static bool has_double(const std::string &s);
static bool is_clean(const std::string &s);

static bool has_doublepair(const std::string &s);
static bool has_jumppair(const std::string &s);

void part1(std::istream &input)
{
    uint64_t count = 0;
    for(;;) {
        std::string line;
        std::getline(input, line);

        if (input.eof()) {
            break;
        }

        if (is_nice(line)) {
            count++;
        }
    }

    std::cout << "Answer 1: " << count << std::endl;
}

void part2(std::istream &input)
{
    uint64_t count = 0;
    for(;;) {
        std::string line;
        std::getline(input, line);

        if (input.eof()) {
            break;
        }

        if (is_nice2(line)) {
            count++;
        }
    }

    std::cout << "Answer 2: " << count << std::endl;
}

static bool is_nice(const std::string &s)
{
    return has_voules(s) && has_double(s) && is_clean(s);
}

static bool is_nice2(const std::string &s)
{
    return has_doublepair(s) && has_jumppair(s);
}

static bool has_voules(const std::string &s)
{
    std::set sset(s.begin(), s.end());

    int count = 0;
    std::string voules = "aeiou";
    std::set v_set(voules.begin(), voules.end());

    for (char c : s) {
        if (v_set.contains(c)) {
            count++;
        }
    }

    return count >= 3;
}

static bool has_double(const std::string &s)
{
    for (size_t i = 0; i < s.size() - 1; ++i) {
        if (s[i] == s[i+1]) {
            return true;
        }
    }

    return false;
}

static bool is_clean(const std::string &s)
{
    for (size_t i = 0; i < s.size() - 1; ++i) {
        switch(s[i]) {
        case 'a':
            if (s[i+1] == 'b') {
                return false;
            }
            break;
        case 'c':
            if (s[i+1] == 'd') {
                return false;
            }
            break;
        case 'p':
            if (s[i+1] == 'q') {
                return false;
            }
            break;
        case 'x':
            if (s[i+1] == 'y') {
                return false;
            }
            break;
        }
    }

    return true;
}

static bool has_doublepair(const std::string &s)
{
    for (size_t i = 0; i < s.size() - 4; ++i) {
        std::string at = s.substr(i, 2);
        for (size_t j = i+2; j < s.size() - 2; ++j) {
            if (at == s.substr(j, 2)) {
                return true;
            }
        }
    }

    return false;
}

static bool has_jumppair(const std::string &s)
{
    for (size_t i = 0; i < s.size()+2; ++i) {
        if (s[i] == s[i+2]) {
            return true;
        }
    }

    return false;
}
