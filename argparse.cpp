#include "argparse.h"
#include <exception>
#include <stdexcept>

Args ParseArguments(size_t argc, const char** argv) {
    Args result;
    if (argc < 2) {
        throw std::runtime_error("Not enough args");
    }
    std::string cmd = argv[1];
    if (cmd == "-h") {
        result.command = Command::Help;
    } else if (cmd == "-c") {
        result.command = Command::Compress;
        if (argc < 3) {
            throw std::runtime_error("Not founded name of archive");
        }
        result.archive_name = argv[2];
        for (size_t i = 3; i < argc; i++) {
            result.files.push_back(std::string(argv[i]));
        }
    } else if (cmd == "-d") {
        result.command = Command::Decompress;
        if (argc < 3) {
            throw std::runtime_error("Not founded name of archive");
        }
        result.archive_name = argv[2];
    } else {
        throw std::runtime_error("Not correct command");
    }
    return result;
}
