#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>

#include <dlfcn.h>

using part_f = void (*)(std::istream &);

int main(int argc, char **argv)
{
    std::vector<std::string> args(argv, argv + argc);

    if (args.size() < 2) {
        std::cerr << "Usage: " << argv[0] << " <runner> [input file]";
        return -1;
    }

    std::string runner_filename(argv[1]);
    std::string input_data;

    {
        std::ifstream input_file;
        std::stringstream stream_data;

        if (args.size() > 2) {
            input_file.open(argv[2]);

            stream_data << input_file.rdbuf();
        } else {
            stream_data << std::cin.rdbuf();
        }

        input_data = stream_data.str();
    }



    void *hndl = dlopen(runner_filename.c_str(), RTLD_NOW | RTLD_GLOBAL);

    if (hndl == nullptr) {
        std::cerr << "Unable to open " << runner_filename
                  << ":\n\t" << dlerror() << std::endl;
        return -1;
    }

    part_f ptr1 = reinterpret_cast<part_f>(dlsym(hndl, "part1"));

    if (nullptr == ptr1) {
        std::cerr << "Unable to locate part1" << std::endl;
        return -1;
    }

    std::istringstream instream;

    instream.str(input_data);

    ptr1(instream);

    part_f ptr2 = reinterpret_cast<part_f>(dlsym(hndl, "part2"));

    if (ptr2 != nullptr)
    {
        instream.clear();
        instream.str(input_data);
        ptr2(instream);
    }
}
