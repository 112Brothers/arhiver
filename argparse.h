#pragma once

#include <vector>
#include <string>

enum class Command { Compress, Decompress, Help };

struct Args {
    Command command;
    std::vector<std::string> files;
    std::string archive_name;
};

Args ParseArguments(size_t argc, const char** argv);
