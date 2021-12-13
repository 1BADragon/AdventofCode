#ifndef ADVENT_HPP
#define ADVENT_HPP

#include <istream>
#include <iterator>
#include <string>
#include <vector>

extern "C" {
    void part1(std::istream &input);
    void part2(std::istream &input);
}

namespace utils {
namespace detail
{
    class Line : public std::string
    {
        friend std::istream & operator>>(std::istream & is, Line & line)
        {
            return std::getline(is, line);
        }
    };
}

template<class OutIt>
void read_lines(std::istream& is, OutIt dest)
{
    typedef std::istream_iterator<detail::Line> InIt;
    std::copy(InIt(is), InIt(), dest);
}

std::vector<std::string> lines(std::istream& is)
{
    std::vector<std::string> lines;
    read_lines(is, std::back_inserter(lines));
    return lines;
}
}

#endif
