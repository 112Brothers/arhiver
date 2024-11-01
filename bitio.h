#pragma once
#include <climits>
#include <cinttypes>
#include <iostream>
#include <queue>

const size_t BYTE_SIZE = CHAR_BIT;

class BitIstream {
    std::istream& is_;
    std::queue<bool> buf_;
    bool is_ended_;

public:
    explicit BitIstream(std::istream&);
    bool Get(uint32_t&, size_t);
};

class BitOstream {
    std::ostream& os_;
    std::deque<bool> buf_;

    void PushByte();

    const size_t buf_size_ = 64;

public:
    void Put(uint32_t, size_t);
    explicit BitOstream(std::ostream&);
    ~BitOstream();
};
