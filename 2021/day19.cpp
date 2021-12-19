#include <regex>
#include <vector>
#include <iostream>
#include <optional>

#include <glm/matrix.hpp>
#include <glm/gtx/transform.hpp>

#include <advent.hpp>
#include <string_utils.hpp>

struct Point{
    int x;
    int y;
    int z;

    Point()
        : x(0), y(0), z(0) {}

    Point (int x, int y, int z)
        : x(x), y(y), z(z) {}

    Point(const glm::vec3 &gvec)
        : x(std::floor(gvec.x + 0.5)), y(std::floor(gvec.y + 0.5)),
          z(std::floor(gvec.z + 0.5)) {}

    Point operator+(const Point &rhs) const {
        return {x + rhs.x, y + rhs.y, z + rhs.z};
    }

    Point operator-() const {
        return {-x, -y, -z};
    }

    Point operator-(const Point &rhs) const {
        return {x - rhs.x, y - rhs.y, z - rhs.z};
    }

    Point operator*(const Point &rhs) const {
        return {x * rhs.x, y * rhs.y, z * rhs.z};
    }

    bool operator==(const Point &rhs) const {
        return x == rhs.x && y == rhs.y && z == rhs.z;
    }

    operator glm::vec4() const {
        return glm::vec4(x, y, z, 1);
    }

    friend std::ostream& operator<<(std::ostream &os, const Point &p);
};

struct Rotation {
    int x;
    int y;
    int z;

    bool is_zero() const {
        return x == 0 && y == 0 && z == 0;
    }

    glm::mat4 build_mat() const {
        glm::mat4 ret(1);

        ret = glm::rotate(ret, (float)x, {1, 0, 0});
        ret = glm::rotate(ret, (float)y, {0, 1, 0});
        ret = glm::rotate(ret, (float)z, {0, 0, 1});

        return ret;
    }

    friend std::ostream& operator<<(std::ostream &os, const Rotation &r);
};

struct Scanner {
    std::optional<Rotation> rotation; // Relative to scanner 0
    std::optional<Point> pos; // All positions are relative to scanner 0
    std::vector<Point> beacons;
};

struct Scanners {
    bool missing_pos() const;
    void corrilate();
    void corrilate(Scanner &s1, Scanner &s2);

    size_t find_common(const Scanner &known, const Scanner &unknown,
                       const Rotation &rot, const Point &kp, const Point &ukp, Point &rel);

    std::vector<Scanner> scanners;
};

static std::vector<Rotation> g_rotations;

static Scanners parse_input(std::istream &input);
static void init_rotations();

void part1(std::istream &input)
{
    init_rotations();

    auto scanners = parse_input(input);
    scanners.scanners[0].pos = Point{0, 0, 0}; // scanner 0 is at origin
    scanners.scanners[0].rotation = Rotation{0, 0, 0}; // scanner 0 is facing 0,0

    for (size_t i = 0; i < 4; ++i) {
        scanners.corrilate();
    }

    for (size_t i = 0; i < scanners.scanners.size(); i++) {
        std::cout << i << ": ";

        if (scanners.scanners[i].pos.has_value()) {
            std::cout << *scanners.scanners[i].pos
                      << " " << *scanners.scanners[i].rotation;
        } else {
            std::cout << "no pos";
        }

        std::cout << std::endl;
    }
}

void Scanners::corrilate(Scanner &s1, Scanner &s2)
{
    if ((s1.pos.has_value() && s2.pos.has_value()) ||
            (!s1.pos.has_value() && !s1.pos.has_value()))
    {
        return;
    }

    Scanner known;
    Scanner unknown;

    if (s1.pos.has_value()) {
        known = s1;
        unknown = s2;
    } else {
        known = s2;
        unknown = s1;
    }

    for (size_t i = 0; i < known.beacons.size(); ++i) {
        for (size_t j = 0; j < 1; ++j) {
            for (auto & rotation : g_rotations) {
                Point p;
                auto n_common = find_common(known, unknown, rotation, known.beacons[i], unknown.beacons[i], p);

                if (n_common >= 6) {
                    std::cout << n_common << " common" << std::endl;

                    if (!s1.pos.has_value()) {
                        s1.pos = *known.pos + p;
                        s1.rotation = rotation;
                    } else {
                        s2.pos = *known.pos + p;
                        s2.rotation = rotation;
                    }
                    return;
                }
            }
        }
    }
}

size_t Scanners::find_common(const Scanner &known, const Scanner &unknown,
                             const Rotation &rot, const Point& kp, const Point &ukp, Point &rel)
{
    std::cout << "-----------" << std::endl;
    std::cout << "rotation: " << rot << std::endl;
    auto rot_mat = rot.build_mat();

    size_t n_common = 0;

    auto rotated = Point(rot_mat * static_cast<glm::vec4>(ukp));

    Point diff = kp - rotated;

    for (size_t i = 0; i < known.beacons.size(); ++i) {
        for (size_t j = 0; j < unknown.beacons.size(); ++j) {

            auto p = Point(rot_mat * static_cast<glm::vec4>(unknown.beacons[j]));

            if (rotated.x == -1 && rotated.y == -1 && rotated.z == 1) {
                std::cout << known.beacons[i] << "-" << diff
                          << "="
                          << known.beacons[i] - diff << " -- " << p << std::endl;
            }

            if (known.beacons[i] - diff == p) {
                n_common++;
            }
        }
    }

    rel = diff;

    std::cout << n_common << " common" << std::endl;

    return n_common;
}

void Scanners::corrilate()
{
    for (size_t i = 0; i < scanners.size(); ++i) {
        for (size_t j = 0; j < scanners.size(); ++j) {
            if (i == j) {
                continue;
            }

            corrilate(scanners[i], scanners[j]);
        }
    }
}

bool Scanners::missing_pos() const
{
    for (auto &scanner : scanners) {
        if (!scanner.pos.has_value()) {
            return true;
        }
    }
    return false;
}

static Scanners parse_input(std::istream &input)
{
    std::vector<Scanner> ret;
    std::regex scanner_re("--- scanner (\\d+) ---");
    std::regex point_re("(-?\\d+),(-?\\d+),(-?\\d+)");
    auto lines = utils::lines(input);

    for (auto &line : lines) {
        std::smatch match;
        if (line.empty()) {
            continue;
        }

        if (std::regex_match(line, match, scanner_re)) {
            ret.emplace_back();
        } else if (std::regex_match(line, match, point_re)) {
            Point p;
            p.x = parse<int>(match[1].str());
            p.y = parse<int>(match[2].str());
            p.z = parse<int>(match[3].str());
            ret.back().beacons.push_back(p);
        }
    }

    return {ret};
}

static void init_rotations()
{
    for (int i = -270; i < 360; i += 90) {
        for (int j = -270; j < 360; j += 90) {
            for (int k = -270; k < 360; k += 90) {
                g_rotations.push_back({i, j, k});
            }
        }
    }
}

std::ostream& operator<<(std::ostream &os, const Point &p)
{
    return os << "[" << p.x << "," << p.y << "," << p.z << "]";
}

std::ostream& operator<<(std::ostream &os, const Rotation &r)
{
    return os << '<' << r.x << "," << r.y
              << "," << r.z << ">";
}

