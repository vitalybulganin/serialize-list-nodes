#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <cstring>
#include <getopt.h>
//-------------------------------------------------------------------------//
#include "adapter-builder.h"
#include "common/list-node.h"
//-------------------------------------------------------------------------//
namespace {
//-------------------------------------------------------------------------//
    struct ParsedFile {
        //!< Keeps a list of nodes.
        std::vector<common::ListNode *> nodes;
        //!< Keeps a list of indexes.
        std::vector<std::int64_t> rand_indexes;

        /**
         * Destructor.
         * @throw None.
         */
        ~ParsedFile() noexcept {
            if (not nodes.empty()) {
                delete this->nodes.front();
            }
        }
    };
//-------------------------------------------------------------------------//
    void usage(const char* program_name) {
        std::cout << "Usage: " << program_name << " [options]" << std::endl
                  << "Options:" << std::endl
                  << "  -i, --input  FILE  Input file name" << std::endl
                  << "  -o, --output FILE  Output file name" << std::endl
                  << "  -h, --help         Show this help";
    }

    auto read_from_file(const std::string &filename) -> std::unique_ptr<ParsedFile> {
        std::ifstream stream(filename);
        if (not stream.is_open()) {
            throw std::runtime_error("Open file [" + filename + "] failed: (" + std::to_string(errno) + ") " + std::strerror(errno));
        }

        auto parsed_file = std::make_unique<ParsedFile>();

        parsed_file->nodes.reserve(1000000);
        parsed_file->rand_indexes.reserve(1000000);

        std::uint64_t lineno = 0;
        std::string line;
        while (std::getline(stream, line)) {
            ++lineno;

            if (not line.empty() && line.back() == '\r') {
                line.pop_back();
            }

            auto pos = line.rfind(';');
            if (pos == std::string::npos) {
                throw (std::invalid_argument("Invalid line: no ';'"));
            }

            auto data = line.substr(0, pos);
            auto rand_str = line.substr(pos + 1);

            if (rand_str.empty()) {
                std::fprintf(stderr, "[WARNING] Empty random index (line #%ld: %s). Set value: -1\n", lineno, line.c_str());
                rand_str = "-1";
            }
            auto rand_index = std::stoll(rand_str);

            auto node = std::make_unique<common::ListNode>();
            node->data = std::move(data);

            if (not parsed_file->nodes.empty()) {
                node->prev = parsed_file->nodes.back();
                parsed_file->nodes.back()->next = node.get();
            }

            parsed_file->nodes.push_back(node.release());
            parsed_file->rand_indexes.push_back(rand_index);
        }

        for (auto i = 0; i < parsed_file->nodes.size(); ++i) {
            int64_t r = parsed_file->rand_indexes[i];

            if (r == -1) {
                parsed_file->nodes[i]->rand = nullptr;
            } else {
                if (r < 0 || static_cast<size_t>(r) >= parsed_file->nodes.size()) {
                    throw (std::invalid_argument("Invalid rand index"));
                }

                parsed_file->nodes[i]->rand = parsed_file->nodes[static_cast<size_t>(r)];
            }
        }

        return parsed_file;
    }

    // Writes binary buffer into file.
    auto write_to_file(const std::string &filename, const std::vector<std::uint8_t> &data) -> void {
        std::ofstream stream(filename, std::ios::binary);
        if (not stream.is_open()) {
            throw (std::invalid_argument("Open file [" + filename + "] failed: (" + std::to_string(errno) + ") " + std::strerror(errno)));
        }

        stream.write(reinterpret_cast<const char *>(data.data()), static_cast<std::streamsize>(data.size()));
        if (stream.fail()) {
            throw (std::invalid_argument("Write to file [" + filename + "] failed: (" + std::to_string(errno) + ") " + std::strerror(errno)));
        }
    }
//-------------------------------------------------------------------------//
}; // namespace
//-------------------------------------------------------------------------//
int main(int argc, char *argv[]) {
    static struct option long_options[] = {
        {"input", required_argument, 0, 'i'},
        {"output", required_argument, 0, 'o'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };

    int opt;
    std::string input_file_name;
    std::string output_file_name;

    while ((opt = getopt_long(argc, argv, "i:o:h", long_options, nullptr)) != -1) {
        switch (opt) {
        case 'i':
            input_file_name = optarg;
            break;
        case 'o':
            output_file_name = optarg;
            break;
        case 'h':
        default:
            return usage(argv[0]), 0;
        }
    }

    try {
        std::fprintf(stdout, "Reading & parsing input file: %s\n", input_file_name.c_str());
        // Reading a list of nodes from file.
        auto parsed_file = read_from_file(input_file_name);
        // Making a binary buffer adapter.
        auto adapter = builders::make_adapter().set(builders::adapter_types::binary).build();

        std::fprintf(stdout, "Parsed input file: %ld nodes\n", parsed_file->nodes.size());
        if (not parsed_file->nodes.empty()) {
            std::fprintf(stdout, "Serializing a list nodes into binary buffer...\n");
            // Serializing a list of nodes.
            auto buffer = adapter->serialize(parsed_file->nodes.front());

            std::fprintf(stdout, "Saving binary buffer in file: %ld bytes\n", buffer.size());
            // Saving the binary buffer into output file.
            write_to_file(output_file_name, buffer);
            std::fprintf(stdout, "Binary buffer saved successfully: %s (%ld bytes)\n", output_file_name.c_str(), buffer.size());
        }
    } catch (std::exception &exc) {
        return std::fprintf(stderr, "[ERROR] %s\n", exc.what()), 1;
    }
    return 0;
}
