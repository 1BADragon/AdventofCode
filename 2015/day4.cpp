#include <iostream>
#include <advent.hpp>

#include <md5.h>

void part1(std::istream &input)
{
    std::string key;
    std::getline(input, key);

    uint64_t num = 1;

    for (;;) {
        MD5Context ctx;
        uint8_t output[16];
        auto input = key + std::to_string(num);

        MD5Init(&ctx);
        MD5Update(&ctx, reinterpret_cast<uint8_t *>(input.data()), input.size());
        MD5Final(output, &ctx);

        if (output[0] == 0 && output[1] == 0 && output[2] >> 4 == 0) {
            break;
        }
        num++;
    }

    std::cout << "Answer 1: " << num << std::endl;
}

void part2(std::istream &input)
{
    std::string key;
    std::getline(input, key);

    uint64_t num = 1;

    for (;;) {
        MD5Context ctx;
        uint8_t output[16];
        auto input = key + std::to_string(num);

        MD5Init(&ctx);
        MD5Update(&ctx, reinterpret_cast<uint8_t *>(input.data()), input.size());
        MD5Final(output, &ctx);

        if (output[0] == 0 && output[1] == 0 && output[2] == 0) {
            break;
        }
        num++;
    }

    std::cout << "Answer 2: " << num << std::endl;
}
