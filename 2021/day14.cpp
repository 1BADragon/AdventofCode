#include <unordered_map>
#include <list>
#include <iostream>
#include <advent.hpp>
#include <string_utils.hpp>

constexpr size_t n_turns = 40;

struct pair_hash
{
    template <class T1, class T2>
    std::size_t operator() (const std::pair<T1, T2> &pair) const {
        return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
    }
};

static std::string expand(const std::string &start,
                          const std::unordered_map<std::pair<char, char>, char, pair_hash> &subs);

static void expand(char l, char r, std::unordered_map<char, uint64_t> &counts,
                   size_t depth,
                   const std::unordered_map<std::pair<char, char>, char, pair_hash> &subs);

static void print_counts(const std::unordered_map<char, uint64_t> &counts);

void part1(std::istream &input)
{
    auto lines = utils::lines(input);

    std::string polymer = lines[0];
    std::unordered_map<std::pair<char, char>, char, pair_hash> substituions;

    for(size_t i = 2; i < lines.size(); ++i) {
        substituions[std::make_pair(lines[i][0], lines[i][1])] = lines[i][6];
    }

    for (size_t step = 0; step < 10; ++step) {
        polymer = expand(polymer, substituions);

        //std::cout << polymer << std::endl;
    }

    std::unordered_map<char, uint64_t> counts;
    for (char c : polymer) {
        if (counts.contains(c)) {
            counts[c]++;
        } else {
            counts[c] = 1;
        }
    }

    //print_counts(counts);

    uint64_t maxel = std::max_element(counts.begin(), counts.end(), [] (auto x, auto y) {return x.second < y.second;})->second;
    uint64_t minel = std::max_element(counts.begin(), counts.end(), [] (auto x, auto y) {return x.second > y.second;})->second;

    std::cout << "Answer 1: " << maxel - minel << std::endl;
}

void part2(std::istream &input)
{
    auto lines = utils::lines(input);
    std::string polymer = lines[0];
    std::unordered_map<std::pair<char, char>, char, pair_hash> substituions;
    std::unordered_map<char, uint64_t> counts;

    for(size_t i = 2; i < lines.size(); ++i) {
        substituions[std::make_pair(lines[i][0], lines[i][1])] = lines[i][6];
    }

    counts[polymer[0]] = 1;
    for (size_t i = 0; i < polymer.size() - 1; ++i) {
        expand(polymer.at(i), polymer.at(i+1), counts, 0, substituions);
    }

    //print_counts(counts);

    uint64_t maxel = std::max_element(counts.begin(), counts.end(), [] (auto x, auto y) {return x.second < y.second;})->second;
    uint64_t minel = std::max_element(counts.begin(), counts.end(), [] (auto x, auto y) {return x.second > y.second;})->second;

    std::cout << "Answer 2: " << maxel - minel << std::endl;
}

static std::string expand(const std::string &start,
                          const std::unordered_map<std::pair<char, char>, char, pair_hash> &subs)
{
    std::string res;

    res.push_back(start[0]);

    for (size_t i = 0; i < start.size() - 1; i++) {
        auto pair = std::make_pair(start[i], start[i+1]);
        res.push_back(subs.at(pair));
        res.push_back(start[i+1]);
    }

    return res;
}

static void expand(char l, char r, std::unordered_map<char, uint64_t> &counts,
                   size_t depth,
                   const std::unordered_map<std::pair<char, char>, char, pair_hash> &subs)
{
    struct Stat {
        char l;
        char r;
        size_t depth;

        bool operator==(const Stat &rhs) const {
            return l == rhs.l && r == rhs.r && depth == rhs.depth;
        }
    };

    struct StatHash {
        std::size_t operator()(const Stat &s) const {
            return std::hash<char>()(s.l) ^ std::hash<char>()(s.r) ^ std::hash<size_t>()(s.depth);
        }
    };

    static std::unordered_map<Stat, std::unordered_map<char, uint64_t>, StatHash> seen;

    auto stat = Stat{l, r, depth};

    std::unordered_map<char, uint64_t> new_counts;

    if (seen.contains(stat)) {
         new_counts = seen.at(stat);
    } else {
        if (depth == n_turns) {
            if (counts.contains(r)) {
                counts[r]++;
            } else {
                counts[r] = 1;
            }

            return;
        }

        char m = subs.at(std::make_pair(l, r));

        expand(l, m, new_counts, depth+1, subs);
        expand(m, r, new_counts, depth+1, subs);

        seen[stat] = new_counts;
    }

    for (auto c : new_counts) {
        if (counts.contains(c.first)) {
            counts[c.first] += c.second;
        } else {
            counts[c.first] = c.second;
        }
    }
}

static void print_counts(const std::unordered_map<char, uint64_t> &counts)
{
    for (auto c : counts) {
        std::cout << c.first << ": " << c.second << std::endl;
    }
}
