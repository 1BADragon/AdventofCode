#include <iostream>

#include <advent.hpp>

class Pos {
public:
    Pos() :depth(0), horz(0), aim(0) {}

    int getDepth() const;

    int getHorz() const;

    void forward(int val) {
        horz += val;
    }

    void forward2(int val) {
        horz += val;
        depth += (val * aim);
    }

    void up(int val) {
        depth -= val;
    }

    void down(int val) {
        depth += val;
    }

    void up2(int val) {
        aim -= val;
    }

    void down2(int val) {
        aim += val;
    }

private:
    int depth;
    int horz;
    int aim;
};

void part1(std::istream &input)
{
    Pos p;

    for (;;) {
        std::string cmd;
        int val;

        input >> cmd >> val;

        if (input.eof()) {
            break;
        }

        if (cmd == "forward") {
            p.forward(val);
        } else if (cmd == "up") {
            p.up(val);
        } else if (cmd == "down") {
            p.down(val);
        } else {
            std::cout << "Unknown command: " << cmd << std::endl;
        }
    }

    std::cout << "Answer: " << p.getDepth() * p.getHorz() << std::endl;
}

void part2(std::istream &input)
{
    Pos p;

    for (;;) {
        std::string cmd;
        int val;

        input >> cmd >> val;

        if (input.eof()) {
            break;
        }

        if (cmd == "forward") {
            p.forward2(val);
        } else if (cmd == "up") {
            p.up2(val);
        } else if (cmd == "down") {
            p.down2(val);
        } else {
            std::cout << "Unknown command: " << cmd << std::endl;
        }
    }

    std::cout << "Answer: " << p.getDepth() * p.getHorz() << std::endl;
}

int Pos::getDepth() const
{
    return depth;
}

int Pos::getHorz() const
{
    return horz;
}
