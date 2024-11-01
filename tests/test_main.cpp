#include <exception>
#include <iostream>
#include "test_argparse.h"
#include "test_util.h"

int main() {
    try {
        TestArgParse();
        TestUtil();
        std::cerr << "Success Unit-testing" << std::endl;
    } catch (std::exception e) {
        std::cerr << "Unit-test failed: " << e.what() << std::endl;
    }
}
