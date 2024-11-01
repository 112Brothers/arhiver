#include "archiver.h"
#include "argparse.h"

const int ERROR_CODE = 111;

int main(int argc, char** argv) {
    const char* help_message =
        "To compress: archiver -c archive_name file1 [file2 ...]\nTo decompress: archiver -d archive_name\n"
        "Help: archiver -h\n";
    try {
        Args a = ParseArguments(static_cast<size_t>(argc), const_cast<const char**>(argv));
        if (a.command == Command::Help) {
            std::cout << help_message << std::endl;
        } else if (a.command == Command::Compress) {
            CompressArchive(a.archive_name, a.files);
        } else if (a.command == Command::Decompress) {
            DecompressArchive(a.archive_name);
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return ERROR_CODE;
    }
    return 0;
}
