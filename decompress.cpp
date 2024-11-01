#include "archiver.h"
#include "trie.h"

uint32_t TryGet(BitIstream& bs, size_t len) {
    uint32_t word = 0;
    if (!bs.Get(word, len)) {
        throw std::runtime_error("Not correct archive");
    }
    return word;
}

uint16_t HeaderDecompresFile::ReadSymbol(BitIstream& bs) {
    size_t v = Trie::ROOT;
    while (!trie[v].is_terminal) {
        uint32_t x = TryGet(bs, 1);
        v = trie.Go(v, x);
    }
    return trie[v].sym;
}

HeaderDecompresFile ReadHeader(BitIstream& bs) {
    HeaderDecompresFile res;
    size_t symbols_count = TryGet(bs, SIZE_OF_WORD);
    std::vector<uint16_t> symbols;
    for (size_t i = 0; i < symbols_count; i++) {
        symbols.push_back(static_cast<uint16_t>(TryGet(bs, SIZE_OF_WORD)));
    }
    size_t left_symbols = symbols_count;
    std::vector<size_t> lens;
    for (size_t len = 1; left_symbols > 0; len++) {
        size_t cnt_lens = TryGet(bs, SIZE_OF_WORD);
        left_symbols -= cnt_lens;
        for (size_t i = 0; i < cnt_lens; i++) {
            lens.push_back(len);
        }
    }
    std::vector<std::string> table = CanonicalFormByLens(lens);
    // assert(table.size() == SYMBOLS_COUNT && symbols.size() == SYMBOLS_COUNT);
    for (size_t i = 0; i < symbols_count; i++) {
        res.trie.Insert(table[i], symbols[i]);
    }
    uint16_t c = 0;
    while (c = res.ReadSymbol(bs), c != FILENAME_END) {
        // assert(c < 256);
        res.file_name += static_cast<char>(c);
    }
    return res;
}

bool DecompressFile(BitIstream& bs, HeaderDecompresFile& header, std::ostream& os) {
    uint16_t c = 0;
    while (c = header.ReadSymbol(bs), !(c == ONE_MORE_FILE || c == ARCHIVE_END)) {
        os.put(Cut(c));
    }
    if (c == ONE_MORE_FILE) {
        return true;
    } else {
        return false;
    }
}

void DecompressArchive(std::string archive_name) {
    std::ifstream archive_file;
    archive_file.exceptions(std::ifstream::badbit);
    archive_file.open(archive_name, std::ios::binary);
    if (!archive_file) {
        throw std::runtime_error("Not exist archive with name " + archive_name);
    }
    BitIstream bin(archive_file);
    std::cout << "Decompress files in archive '" << archive_name << "':" << std::endl;
    while (true) {
        HeaderDecompresFile header = ReadHeader(bin);
        std::ofstream file(header.file_name);
        bool more_file = DecompressFile(bin, header, file);
        file.close();
        std::cout << header.file_name << " was decompressed" << std::endl;
        if (!more_file) {
            break;
        }
    }
    std::cout << "Successful decompress" << std::endl;
}