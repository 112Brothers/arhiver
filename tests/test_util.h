#include <stdexcept>
#include "../util.h"

inline void TestToBinary() {
    if (ToBinary(0, 3) != "000" || ToBinary(6, 3) != "110") {  // NOLINT(readability-magic-numbers)
        throw std::logic_error("ToBinary() is wrong");
    }
}

inline void TestBinstrToInt() {
    if (BinstrToInt("011") != 6 || BinstrToInt("000000001") != 256) {  // NOLINT(readability-magic-numbers)
        throw std::logic_error("BinstrToInt() is wrong");
    }
}

inline void TestCanonicalFormByLens() {
    std::vector<size_t> lens = {1, 2, 3, 3};
    std::vector<std::string> res = CanonicalFormByLens(lens);
    std::vector<std::string> ans = {"0", "10", "110", "111"};
    if (res != ans) {
        throw std::logic_error("CanonicalFormByLens() is wrong");
    }
}
inline void TestUtil() {
    try {
        TestToBinary();
        TestBinstrToInt();
        TestCanonicalFormByLens();
    } catch (std::exception e) {
        throw std::logic_error(std::string(std::string("FAILED TestUtil(): ") + e.what()));
    }
}