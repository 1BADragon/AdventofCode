#include <variant>
#include <vector>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <limits>
#include <assert.h>
#include <advent.hpp>

class ROBitStream {
public:
    ROBitStream(const std::string &hex)
        : index(0), bit(0), len(0), data()
    {
        uint16_t cur_val = 0;
        size_t i = 0;
        while (i+1 < hex.size()) {
            auto num = hex.substr(i, 2);

            std::stringstream ss;
            ss << std::hex << num;
            ss >> cur_val;

            data.push_back(cur_val);
            len += 8;

            i += 2;
        }

        if (i < hex.size()) {
            auto num = hex.substr(i) + "0";
            std::stringstream ss;
            ss << std::hex << num;
            ss >> cur_val;
            data.push_back(cur_val);
            len += 4;
        }
    }

    ROBitStream(ROBitStream &orig, size_t size)
        :index(0), bit(0), len(0), data()
    {
        size_t total_size = 0;
        while (total_size + 8 < size) {
            uint8_t val = orig.read<uint8_t>(8);

            data.push_back(val);
            total_size += 8;
            len += 8;
        }

        uint8_t remaining = 0;
        for (size_t i = 7; i >= 0; --i) {
            if (total_size >= size) {
                break;
            }

            remaining |= (orig.read_bit() << i);
            total_size++;
            len++;
        }

        data.push_back(remaining);
    }

    template<typename T>
    T read(int b)
    {
        assert(b <= sizeof(T) * 8);

        T val = 0;

        for (int i = b-1; i >= 0; --i) {
            val |= read_bit() << i;
        }

        return val;
    }

    uint8_t read_bit()
    {
        if (index * 8 + bit >= len) {
            throw std::length_error("Out of data to read");
        }

        if (bit >= 8) {
            bit = 0;
            index ++;
        }

        uint8_t val = (data[index] >> (7 - bit)) & 1;

        bit++;

        return val;
    }

    uint64_t total_len() const {
        return len;
    }

    uint64_t remaining() const {
        return total_len() - at();
    }

    uint64_t at() const {
        return index * 8 + bit;
    }

private:
    size_t index;
    size_t bit;
    uint64_t len;
    std::vector<uint8_t> data;
};

struct Packet;
struct Literal {
    uint64_t val;
};

using Payload = std::variant<std::vector<Packet>, Literal>;

struct Packet {
    uint16_t version;
    uint16_t type;

    Payload payload;
};

static std::vector<Packet> parse_packets(ROBitStream &bs, size_t count = std::numeric_limits<size_t>::max());
static std::optional<Packet> parse_packet(ROBitStream &bs);
uint64_t version_sum(const Packet &p);
uint64_t packet_value(const Packet &p);

std::string format_packet(const Packet &p, size_t depth = 0);

std::ostream& operator<<(std::ostream &os, const Packet &p);

void part1(std::istream &input)
{
    std::string line;
    std::getline(input, line);
    ROBitStream bs(line);

    auto packet = parse_packet(bs);
    std::cout << *packet << std::endl;

    std::cout << "Answer 1: " << version_sum(*packet) << std::endl;
    std::cout << "Answer 2: " << packet_value(*packet) << std::endl;
}

static std::vector<Packet> parse_packets(ROBitStream &bs, size_t count)
{
    std::vector<Packet> packets;

    while (packets.size() < count) {
        try {
            auto packet_opt = parse_packet(bs);
            if (!packet_opt) {
                break;
            }

            packets.push_back(*packet_opt);
        } catch (std::length_error &l) {
            break;
        }
    }

    return packets;
}

static std::optional<Packet> parse_packet(ROBitStream &bs)
{
    Packet p;
    p.version = bs.read<uint8_t>(3);
    p.type = bs.read<uint8_t>(3);

    if (p.type == 4) {
        Literal l{0};

        for (;;) {
            l.val <<= 4;
            uint8_t tmp = bs.read<uint8_t>(5);
            l.val |= tmp & 0xf;
            if ((tmp & (1 << 4)) == 0) {
                break;
            }
        }
        p.payload = l;

    } else {
        uint8_t i = bs.read<uint8_t>(1);

        if (i == 1) {
            uint16_t t_packets = bs.read<uint16_t>(11);
            auto packets = parse_packets(bs, t_packets);

            p.payload = std::move(packets);
        } else {
            uint16_t size = bs.read<uint16_t>(15);
            ROBitStream sub_packet_stream(bs, size);

            auto packets = parse_packets(sub_packet_stream);

            p.payload = std::move(packets);
        }
    }

    return p;
}

uint64_t version_sum(const Packet &p)
{
    uint64_t sum = 0;

    if (p.type != 4) {
        for (auto &p : std::get<0>(p.payload)) {
            sum += version_sum(p);
        }
    }

    return sum + p.version;
}

uint64_t packet_value(const Packet &p)
{
    uint64_t res = 0;
    if (p.type == 0 || p.type == 1) {
        auto &payload = std::get<0>(p.payload);
        if (payload.size() == 1) {
            return packet_value(payload[0]);
        }
    }

    switch (p.type) {
    case 0:
        for (auto &p : std::get<0>(p.payload)) {
            res += packet_value(p);
        }
        break;
    case 1:
        res = 1;
        for (auto &p : std::get<0>(p.payload)) {
            res *= packet_value(p);
        }
        break;
    case 2:
    case 3:
    {
        std::vector<uint64_t> vals;
        for (auto &p : std::get<0>(p.payload)) {
            vals.push_back(packet_value(p));
        }
        if (p.type == 2) {
            res = *std::min_element(vals.begin(), vals.end());
        } else {
            res = *std::max_element(vals.begin(), vals.end());
        }
    }
        break;
    case 4:
        res = std::get<1>(p.payload).val;
        break;
    case 5:
    case 6:
    case 7:
    {
        auto &c = std::get<0>(p.payload);
        uint64_t left = packet_value(c[0]);
        uint64_t right = packet_value(c[1]);

        if (p.type == 5) {
            res = left > right;
        } else if (p.type == 6) {
            res = left < right;
        } else {
            res = left == right;
        }
    }
    }

    return res;
}

std::string format_packet(const Packet &p, size_t depth)
{
    std::stringstream ss;

    for (size_t i = 0; i < depth; ++i) {
        ss << " ";
    }

    ss << "v:" << p.version << " t:" << p.type << " ";

    if (p.type == 4) {
        ss << "literal: " << std::get<Literal>(p.payload).val << "\n";
    } else {
        ss << "operator :\n";
        for (auto &subpacket : std::get<0>(p.payload)) {
            ss << format_packet(subpacket, depth+1);
        }
    }

    return ss.str();
}

std::ostream& operator<<(std::ostream &os, const Packet &p)
{
    return os << format_packet(p);
}
