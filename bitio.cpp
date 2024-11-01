#include "bitio.h"

BitIstream::BitIstream(std::istream& is) : is_(is), is_ended_(false) {
}

bool BitIstream::Get(uint32_t& word, size_t len) {
    while (!is_ended_ && buf_.size() < len) {
        char c = 0;
        if (is_.get(c)) {
            for (size_t i = 0; i < BYTE_SIZE; i++) {
                buf_.push((c >> i) & 1);
            }
        } else {
            is_ended_ = true;
        }
    }
    if (buf_.size() >= len) {
        word = 0;
        for (size_t i = 0; i < len; i++) {
            word |= static_cast<uint32_t>(buf_.front()) << i;
            buf_.pop();
        }
        return true;
    }
    return false;
}

void BitOstream::PushByte() {
    uint8_t x = 0;
    for (size_t i = 0; i < BYTE_SIZE; i++) {
        x |= static_cast<uint8_t>(buf_.front()) << i;
        buf_.pop_front();
    }
    os_.put(static_cast<char>(x));
}

void BitOstream::Put(uint32_t word, size_t len) {
    for (size_t i = 0; i < len; i++) {
        buf_.push_back((word >> i) & 1);
    }
    if (buf_.size() / BYTE_SIZE >= buf_size_) {
        while (buf_.size() >= BYTE_SIZE) {
            PushByte();
        }
    }
}

BitOstream::BitOstream(std::ostream& os) : os_(os) {
}

BitOstream::~BitOstream() {
    while (buf_.size() > BYTE_SIZE) {
        PushByte();
    }
    if (buf_.empty()) {
        return;
    }
    while (buf_.size() < BYTE_SIZE) {
        buf_.push_back(false);
    }
    PushByte();
}
