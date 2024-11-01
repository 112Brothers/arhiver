#pragma once

#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>

struct NodeOfTrie {
    size_t child0;
    size_t child1;
    uint16_t sym;
    bool is_terminal;
    size_t cnt;
    NodeOfTrie(uint16_t c, size_t cnt, bool is_terminal)
        : child0(0), child1(0), sym(c), is_terminal(is_terminal), cnt(cnt) {
    }
    NodeOfTrie() : child0(0), child1(0), is_terminal(false) {
    }
};

class Trie {
    std::vector<NodeOfTrie> arr_;

public:
    static const size_t ROOT = 1;
    Trie();
    size_t Go(size_t, size_t);
    void Insert(std::string, uint16_t);
    NodeOfTrie& operator[](size_t);
};