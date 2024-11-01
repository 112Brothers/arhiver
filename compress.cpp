#include "archiver.h"
#include "heap.h"

std::pair<CodeTable, std::vector<uint16_t>> CanonicalForm(CodeTable table) {
    std::vector<uint16_t> huffman;
    for (const auto& i : table) {
        huffman.push_back(i.first);
    }
    std::sort(huffman.begin(), huffman.end(), [&table](uint16_t a, uint16_t b) {
        if (table[a].size() != table[b].size()) {
            return table[a].size() < table[b].size();
        }
        return a < b;
    });
    CodeTable result;
    std::vector<size_t> lens(huffman.size());
    for (size_t i = 0; i < huffman.size(); i++) {
        lens[i] = table[huffman[i]].size();
    }
    std::vector<std::string> codes = CanonicalFormByLens(lens);
    for (size_t i = 0; i < huffman.size(); i++) {
        result[huffman[i]] = codes[i];
    }
    return {result, huffman};
}

std::pair<CodeTable, std::vector<uint16_t>> GenerateTable(std::unordered_map<uint16_t, size_t> frequency) {
    std::vector<NodeOfTrie> trie(1);
    using Pair = std::pair<size_t, size_t>;
    Heap<Pair> q;
    for (auto [sym, cnt] : frequency) {
        trie.emplace_back(sym, cnt, true);
        q.Push(Pair(cnt, trie.size() - 1));
    }
    while (q.Size() >= 2) {
        Pair a = q.Top();
        q.Pop();
        Pair b = q.Top();
        q.Pop();
        trie.emplace_back(0, a.first + b.first, false);
        trie.back().child0 = a.second;
        trie.back().child1 = b.second;
        q.Push(Pair(trie.back().cnt, trie.size() - 1));
    }
    CodeTable code;
    const std::function<void(size_t, std::string)> dfs = [&dfs, &trie, &code](size_t v, std::string s) {
        if (trie[v].is_terminal) {
            code[trie[v].sym] = s;
            return;
        }
        if (trie[v].child0 != 0) {
            dfs(trie[v].child0, s + "0");
        }
        if (trie[v].child1 != 0) {
            dfs(trie[v].child1, s + "1");
        }
    };
    dfs(q.Top().second, "");
    return CanonicalForm(code);
}

FrequnceTable CountFrequences(std::istream& file, std::string file_name) {
    FrequnceTable res;
    char c = 0;
    while (file.get(c)) {
        res[Extend(c)]++;
    }
    file.clear();
    file.seekg(0);
    for (char i : file_name) {
        res[Extend(i)]++;
    }
    res[FILENAME_END] = res[ONE_MORE_FILE] = res[ARCHIVE_END] = 1;
    return res;
}

void CompressFile(BitOstream& bs, std::istream& file, std::string file_name, bool last_file) {
    FrequnceTable freq = CountFrequences(file, file_name);
    auto [table, order] = GenerateTable(freq);
    uint32_t arr_table[MAX_SYMBOL + 1];
    size_t size_code[MAX_SYMBOL + 1];
    for (const auto& [sym, s] : table) {
        arr_table[sym] = BinstrToInt(s);
        size_code[sym] = table[sym].size();
    }

    bs.Put(static_cast<uint32_t>(table.size()), SIZE_OF_WORD);
    size_t max_symbol_code_size = 0;
    std::unordered_map<size_t, size_t> cnt_sizes;
    for (uint16_t i : order) {
        bs.Put(i, SIZE_OF_WORD);
        size_t cur_size = table[i].size();
        max_symbol_code_size = std::max(max_symbol_code_size, cur_size);
        cnt_sizes[cur_size]++;
    }
    for (size_t i = 1; i <= max_symbol_code_size; i++) {
        bs.Put(cnt_sizes[i], SIZE_OF_WORD);
    }
    auto put_symbol = [&](uint16_t c) {
        if (c > MAX_SYMBOL) {
            throw std::logic_error("Not correct symbol");
        }
        bs.Put(arr_table[c], size_code[c]);
    };

    for (char i : file_name) {
        put_symbol(Extend(i));
    }
    put_symbol(FILENAME_END);
    char c = 0;
    while (file.get(c)) {
        put_symbol(Extend(c));
    }
    if (!last_file) {
        put_symbol(ONE_MORE_FILE);
    } else {
        put_symbol(ARCHIVE_END);
    }
}

void CompressArchive(std::string archive_name, std::vector<std::string> files) {
    std::vector<std::ifstream> opened_files(files.size());
    for (size_t i = 0; i < files.size(); i++) {
        opened_files[i].exceptions(std::ifstream::badbit);
        opened_files[i].open(files[i], std::ios::binary);
        if (!opened_files[i]) {
            throw std::runtime_error("Not exist file " + files[i]);
        }
    }
    std::ofstream archive_file;
    archive_file.exceptions(std::ofstream::badbit);
    archive_file.open(archive_name, std::ios::binary);
    BitOstream bout(archive_file);
    for (size_t i = 0; i < files.size(); i++) {
        // char buf[1024 * 128];
        // opened_files[i].rdbuf()->pubsetbuf(buf, sizeof(buf));
        std::filesystem::path p(files[i]);
        CompressFile(bout, opened_files[i], p.filename(), i + 1 == files.size());
        opened_files[i].close();
        std::cout << p.filename() << " was compressed" << std::endl;
    }
}
