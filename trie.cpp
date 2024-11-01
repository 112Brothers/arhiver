#include <stdexcept>
#include "trie.h"

Trie::Trie() : arr_(2) {
}

void Trie::Insert(std::string code, uint16_t c) {
    size_t v = ROOT;
    for (char& i : code) {
        if (i == '0') {
            if (arr_[v].child0 == 0) {
                arr_[v].child0 = arr_.size();
                arr_.push_back(NodeOfTrie());
            }
            v = arr_[v].child0;
        } else if (i == '1') {
            if (arr_[v].child1 == 0) {
                arr_[v].child1 = arr_.size();
                arr_.push_back(NodeOfTrie());
            }
            v = arr_[v].child1;
        } else {
            throw std::logic_error("HZ trie");
        }
    }
    arr_[v].sym = c;
    arr_[v].is_terminal = true;
}

size_t Trie::Go(size_t v, size_t b) {
    if (v >= arr_.size() || v == 0) {
        throw std::runtime_error("Try down from not-exist node");
    }
    if (b == 0) {
        if (arr_[v].child0 == 0) {
            throw std::runtime_error("Try down to not-exist node");
        }
        v = arr_[v].child0;
    } else {
        if (arr_[v].child1 == 0) {
            throw std::runtime_error("Try down to not-exist node");
        }
        v = arr_[v].child1;
    }
    return v;
}

NodeOfTrie& Trie::operator[](size_t i) {
    return arr_[i];
}