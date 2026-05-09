#include "text-adapter.h"
//-------------------------------------------------------------------------//
#include <sstream>
#include <unordered_map>
//-------------------------------------------------------------------------//
namespace adapters {
//-------------------------------------------------------------------------//
    namespace {
//-------------------------------------------------------------------------//
        struct ParsedFile {
            //!< Keeps a list of nodes.
            common::ListNode *head = nullptr;

            //!< Keeps a list of nodes.
            std::vector<common::ListNode *> nodes;
            //!< Keeps a list of indexes.
            std::vector<std::int64_t> rand_indexes;
        };
//-------------------------------------------------------------------------//
    }; // namespace
//-------------------------------------------------------------------------//
    auto TextAdapter::serialize(const common::ListNode *node) const -> std::vector<std::uint8_t> {
        std::vector<const common::ListNode *> nodes;
        nodes.reserve(1'000'000);

        for (const auto *cur = node; cur != nullptr; cur = cur->next) {
            nodes.push_back(cur);
        }

        std::unordered_map<const common::ListNode *, std::uint64_t> index;
        index.reserve(nodes.size());

        for (std::uint64_t i = 0; i < nodes.size(); ++i) {
            index.emplace(nodes[static_cast<std::size_t>(i)], i);
        }

        std::ostringstream stream;
        for (const auto *cur : nodes) {
            std::int64_t rand_index = -1;

            if (cur->rand != nullptr) {
                auto found = index.find(cur->rand);
                if (found == index.end()) {
                    throw std::invalid_argument("rand points outside list");
                }

                rand_index = static_cast<std::int64_t>(found->second);
            }

            stream << cur->data << ';' << rand_index << '\n';
        }

        const auto text = stream.str();
        return {text.begin(), text.end()};
    }

    auto TextAdapter::deserialize(const std::uint8_t *buf, size_t buf_size) -> std::unique_ptr<common::ListNode> {
        if (buf_size == 0) {
            return nullptr;
        }

        if (buf == nullptr) {
            throw std::invalid_argument("Input buffer is null");
        }

        std::istringstream stream(std::string(reinterpret_cast<const char *>(buf), buf_size));
        auto parsed_file = std::make_unique<ParsedFile>();

        parsed_file->nodes.reserve(1'000'000);
        parsed_file->rand_indexes.reserve(1'000'000);

        std::string line;
        while (std::getline(stream, line)) {
            if (not line.empty() && line.back() == '\r') {
                line.pop_back();
            }

            auto pos = line.rfind(';');
            if (pos == std::string::npos) {
                throw (std::invalid_argument("Invalid line: no ';'"));
            }

            auto data = line.substr(0, pos);
            auto rand_str = line.substr(pos + 1);
            auto rand_index = std::stoll(rand_str);

            auto node = new common::ListNode();
            node->data = std::move(data);

            if (not parsed_file->nodes.empty()) {
                node->prev = parsed_file->nodes.back();
                parsed_file->nodes.back()->next = node;
            } else {
                parsed_file->head = node;
            }

            parsed_file->nodes.push_back(node);
            parsed_file->rand_indexes.push_back(rand_index);
        }

        for (auto i = 0; i < parsed_file->nodes.size(); ++i) {
            int64_t r = parsed_file->rand_indexes[i];

            if (r == -1) {
                parsed_file->nodes[i]->rand = nullptr;
            } else {
                if (r < 0 || static_cast<size_t>(r) >= parsed_file->nodes.size()) {
                    throw std::runtime_error("Invalid rand index");
                }

                parsed_file->nodes[i]->rand = parsed_file->nodes[static_cast<size_t>(r)];
            }
        }

        return std::unique_ptr<common::ListNode>(parsed_file->head);
    }
//-------------------------------------------------------------------------//
}; // namespace adapters
