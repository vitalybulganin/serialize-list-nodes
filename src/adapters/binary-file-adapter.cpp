#include "binary-file-adapter.h"
//-------------------------------------------------------------------------//
#include <cstring>
#include <fstream>
#include <utility>
#include <unordered_map>
//-------------------------------------------------------------------------//
namespace adapters {
//-------------------------------------------------------------------------//
    namespace {
//-------------------------------------------------------------------------//
        template <typename T>
        auto read_pod(std::istream &in) -> T {
            T value{};

            in.read(reinterpret_cast<char*>(&value), sizeof(T));
            if (!in) {
                throw (std::invalid_argument("Failed to read binary value"));
            }

            return value;
        }

        auto read_string(std::istream& in, std::uint64_t size) -> std::string {
            if (size > 1000) {
                throw (std::invalid_argument("String is too large"));
            }

            std::string s(static_cast<std::size_t>(size), '\0');

            if (size > 0) {
                in.read(s.data(), static_cast<std::streamsize>(size));
                if (!in) {
                    throw (std::invalid_argument("Failed to read string data"));
                }
            }

            return s;
        }
//-------------------------------------------------------------------------//
    }; // namespace
//-------------------------------------------------------------------------//
    auto BinaryFileAdapter::serialize(const common::ListNode *head) const -> std::vector<std::uint8_t> {
        std::uint64_t i = 0;
        std::vector<const common::ListNode*> nodes;
        std::unordered_map<const common::ListNode *, std::uint64_t> index;

        nodes.reserve(1'000'000);
        index.reserve(1'000'000);

        for (const common::ListNode *cur = head; cur != nullptr; cur = cur->next) {
            nodes.push_back(cur);
            index.emplace(cur, i++);
        }

        std::ofstream out(this->output_file, std::ios::binary);
        if (not out.is_open()) {
            throw (std::invalid_argument("Open file [" + this->output_file + "] failed: (" + std::to_string(errno) + ") " + std::strerror(errno)));
        }

        const auto count = nodes.size();
        // Writing a count of nodes.
        out.write(reinterpret_cast<const char*>(&count), sizeof(count));

        for (const auto *node : nodes) {
            std::int64_t rand_index = -1;

            if (node->rand != nullptr) {
                auto found = index.find(node->rand);
                if (found == std::end(index)) {
                    throw (std::invalid_argument("rand points outside list"));
                }

                rand_index = static_cast<std::int64_t>(found->second);
            }

            const auto data_size = node->data.size();
            // Writing a node index.
            out.write(reinterpret_cast<const char*>(&rand_index), sizeof(rand_index));
            // Writing a length of data.
            out.write(reinterpret_cast<const char*>(&data_size), sizeof(data_size));
            // Writing data.
            out.write(node->data.data(), static_cast<std::streamsize>(data_size));
        }

        if (out.fail()) {
            throw std::invalid_argument("Write failed");
        }
        return {};
    }

    auto BinaryFileAdapter::deserialize(const std::uint8_t *data, size_t size) -> std::unique_ptr<common::ListNode> {
        std::ifstream in(this->input_file, std::ios::binary);
        if (not in.is_open()) {
            throw (std::invalid_argument("Open file [" + this->input_file + "] failed: (" + std::to_string(errno) + ") " + std::strerror(errno)));
        }

        // Reading a count of nodes.
        auto count = read_pod<std::uint64_t>(in);
        if (count > 1'000'000) {
            throw (std::invalid_argument("Too many nodes"));
        }

        std::vector<common::ListNode *> nodes;
        std::vector<std::int64_t> rand_indices;

        nodes.reserve(static_cast<std::size_t>(count));
        rand_indices.reserve(static_cast<std::size_t>(count));

        for (auto i = 0; i < count; ++i) {
            auto rand_index = read_pod<std::int64_t>(in);
            auto data_size = read_pod<std::uint64_t>(in);

            auto *node = new common::ListNode();
            node->data = read_string(in, data_size);

            if (not nodes.empty()) {
                node->prev = nodes.back();
                nodes.back()->next = node;
            }

            nodes.push_back(node);
            rand_indices.push_back(rand_index);
        }

        for (auto i = 0; i < nodes.size(); ++i) {
            auto r = rand_indices[i];
            if (r == -1) {
                nodes[i]->rand = nullptr;
            } else {
                if (r < 0 || static_cast<std::uint64_t>(r) >= count) {
                    throw std::runtime_error("Invalid rand index");
                }

                nodes[i]->rand = nodes[static_cast<std::size_t>(r)];
            }
        }

        return nodes.empty() ? nullptr : std::unique_ptr<common::ListNode>(nodes.front());
    }
//-------------------------------------------------------------------------//
    BinaryFileAdapter::BinaryFileAdapter(std::string input, std::string output)
        : input_file(std::move(input)), output_file(std::move(output)) {
    }
//-------------------------------------------------------------------------//
}; // namespace adapters
