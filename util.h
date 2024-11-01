#pragma once

#include <cinttypes>
#include <string>
#include <vector>

inline uint16_t Extend(char x) {
    return *reinterpret_cast<uint8_t*>(&x);
}

inline char Cut(uint16_t x) {
    return static_cast<char>(x);
}

inline std::string ToBinary(uint16_t x, size_t len) {
    std::string result;
    for (size_t i = 0; i < len; i++) {
        result = std::to_string(x % 2) + result;
        x >>= 1;
    }
    return result;
}

inline uint32_t BinstrToInt(std::string s) {
    uint32_t res = 0;
    for (size_t i = 0; i < s.size(); i++) {
        res |= static_cast<uint32_t>(s[i] - '0') << i;
    }
    return res;
}

inline std::vector<std::string> CanonicalFormByLens(std::vector<size_t> lens) {
    uint16_t x = 0;
    size_t len = 0;
    std::vector<std::string> result(lens.size());
    for (size_t i = 0; i < lens.size(); i++) {
        if (i == 0) {
            x = 0;
            len = lens[i];
        } else {
            x++;
            x <<= lens[i] - len;
            len = lens[i];
        }
        result[i] = ToBinary(x, len);
    }
    return result;
}
