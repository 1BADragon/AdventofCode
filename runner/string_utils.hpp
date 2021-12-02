#ifndef STRING_UTILS_HPP
#define STRING_UTILS_HPP

#include <vector>
#include <string>
#include <sstream>
#include <type_traits>

std::vector<std::string> split(const std::string &_s, const std::string &delimiter)
{
    size_t pos = 0;
    std::string s(_s);
    std::vector<std::string> tokens;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        tokens.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    if (s.length()) {
        tokens.push_back(s);
    }

    return tokens;
}

template<typename T>
T parse(const std::string &s)
{
    T ret;
    std::stringstream(s) >> ret;
    return ret;
}

template<typename T>
std::vector<T> parse(const std::vector<std::string> &r)
{
    std::vector<T> ret;
    ret.reserve(r.size());

    for (auto & s : r) {
        ret.push_back(parse<T>(s));
    }

    return ret;
}

#endif // STRING_UTILS_HPP
