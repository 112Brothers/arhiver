#pragma once

#include <algorithm>
#include <bitset>
#include <cctype>
#include <cassert>
#include <exception>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <functional>
#include <map>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>
#include "bitio.h"
#include "trie.h"
#include "util.h"

struct HeaderDecompresFile {
    std::string file_name;
    Trie trie;
    uint16_t ReadSymbol(BitIstream&);
};

using CodeTable = std::map<uint16_t, std::string>;
using FrequnceTable = std::unordered_map<uint16_t, size_t>;

const uint16_t FILENAME_END = 256;
const uint16_t ONE_MORE_FILE = 257;
const uint16_t ARCHIVE_END = 258;

const uint16_t MAX_SYMBOL = ARCHIVE_END;
const size_t SIZE_OF_WORD = 9;

FrequnceTable CountFrequences(std::istream&, std::string);
std::pair<CodeTable, std::vector<uint16_t> > CanonicalForm(CodeTable);
std::pair<CodeTable, std::vector<uint16_t> > GenerateTable(std::unordered_map<uint16_t, size_t>);
void CompressFile(BitOstream&, std::istream&, std::string, bool);
void CompressArchive(std::string, std::vector<std::string>);

HeaderDecompresFile ReadHeader(BitIstream& bs);
bool DecompressFile(BitIstream&, std::string&, std::vector<char>&);
void DecompressArchive(std::string);
uint32_t TryGet(BitIstream& bs, size_t len);
