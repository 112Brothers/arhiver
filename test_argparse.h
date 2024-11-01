#include "../argparse.h"
#include <array>
#include <exception>
#include <stdexcept>
#include <string>

bool operator==(const Args& a, const Args& b) {
    return a.command == b.command && a.archive_name == b.archive_name && a.files == b.files;
}

inline void TestArgParse() {
    try {
        const char* input1[] = {"", "-h"};
        Args a1 = ParseArguments(2, input1);  // NOLINT(readability-magic-numbers)
        Args ans1;
        ans1.command = Command::Help;
        if (!(a1 == ans1)) {
            throw std::logic_error("Not parsed Help command");
        }
        const char* input2[] = {"", "-c", "CoolArch", "file121", "/home/../arh1"};
        Args a2 = ParseArguments(5, input2);  // NOLINT(readability-magic-numbers)
        Args ans2;
        ans2.command = Command::Compress;
        ans2.archive_name = "CoolArch";
        ans2.files = {"file121", "/home/../arh1"};
        if (!(a2 == ans2)) {
            throw std::logic_error("Not parsed Compress args");
        }
        const char* input3[] = {"", "-d", "CoolArhToo"};
        Args a3 = ParseArguments(3, input3);  // NOLINT(readability-magic-numbers)
        Args ans3;
        ans3.command = Command::Decompress;
        ans3.archive_name = "CoolArhToo";
        if (!(a3 == ans3)) {
            throw std::logic_error("Not parsed Decompress args");
        }
    } catch (std::exception e) {
        throw std::runtime_error(std::string("FAILED TestArgParse(): ") + e.what());
    }
}