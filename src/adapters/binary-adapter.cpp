#include "binary-adapter.h"
//-------------------------------------------------------------------------//
#include <cstring>
#include <optional>
#include <unordered_map>
//-------------------------------------------------------------------------//
namespace adapters {
//-------------------------------------------------------------------------//
    namespace {
//-------------------------------------------------------------------------//
        class BufferWriter final {
            std::vector<std::uint8_t> &buf;

        public:
            explicit BufferWriter(std::vector<std::uint8_t> &buffer) : buf(buffer) {
            }

            template<typename T>
            void write_pod(const T& value) {
                const auto *p = reinterpret_cast<const std::uint8_t*>(&value);
                this->buf.insert(std::end(this->buf), p, p + sizeof(T));
            }

            void write_bytes(const void* data, std::size_t size) {
                const auto* p = static_cast<const std::uint8_t*>(data);
                this->buf.insert(std::end(this->buf), p, p + size);
            }
        };

        class BufferReader final {
            const std::uint8_t *data = nullptr;
            std::size_t size = 0;
            std::size_t offset = 0;

        public:
            explicit BufferReader(const std::uint8_t *buf, std::size_t buf_size) : data(buf), size(buf_size) {
            }

            template<typename T>
            T read_pod() {
                if (this->offset + sizeof(T) > this->size) {
                    throw (std::invalid_argument("Unexpected end of buffer"));
                }

                T value{};
                std::memcpy(&value, this->data + this->offset, sizeof(T));
                this->offset += sizeof(T);
                return value;
            }

            std::string read_string(std::size_t len) {
                if (this->offset + len > this->size) {
                    throw std::invalid_argument("Unexpected end of buffer");
                }

                std::string s(reinterpret_cast<const char *>(this->data + this->offset), len);
                this->offset += len;
                return s;
            }

            void read_bytes(void* dst, std::size_t len) {
                if (this->offset + len > this->size) {
                    throw std::invalid_argument("Unexpected end of buffer");
                }

                std::memcpy(dst, this->data + this->offset, len);
                this->offset += len;
            }
        };
//-------------------------------------------------------------------------//
    }; // namespace
//-------------------------------------------------------------------------//
    auto BinaryAdapter::serialize(const common::ListNode *head) const -> std::vector<std::uint8_t> {
        std::vector<std::uint8_t> buffer;
        std::uint64_t i = 0;
        std::vector<const common::ListNode *> nodes;
        std::unordered_map<const common::ListNode *, std::uint64_t> index;

        nodes.reserve(1'000'000);
        index.reserve(1'000'000);

        for (const auto *cur = head; cur != nullptr; cur = cur->next) {
            nodes.push_back(cur);
            index.emplace(cur, i++);
        }

        buffer.reserve(sizeof(std::uint16_t) + nodes.size() * sizeof(std::int64_t));

        BufferWriter writer(buffer);
        // Writing a count of nodes.
        writer.write_pod(nodes.size());

        for (const auto *node : nodes) {
            std::int64_t rand_index = -1;

            if (node->rand != nullptr) {
                auto found = index.find(node->rand);
                if (found == index.end()) {
                    throw (std::invalid_argument("rand points outside list"));
                }

                rand_index = static_cast<std::int64_t>(found->second);
            }

            const auto data_size = static_cast<std::uint16_t>(node->data.length());

            writer.write_pod(rand_index);
            writer.write_pod(data_size);
            writer.write_bytes(node->data.data(), data_size);
        }

        return std::move(buffer);
    }

    auto BinaryAdapter::deserialize(const std::uint8_t *data, size_t size) -> std::unique_ptr<common::ListNode> {
        BufferReader reader(data, size);
        auto nodes_count = reader.read_pod<std::uint64_t>();

        std::vector<common::ListNode *> nodes;
        std::vector<std::int64_t> rand_indices;

        nodes.reserve(nodes_count);
        rand_indices.reserve(nodes_count);

        for (std::uint64_t i = 0; i < nodes_count; ++i) {
            auto rand_index = reader.read_pod<std::int64_t>();
            auto data_size = reader.read_pod<std::uint16_t>();

            auto *node = new common::ListNode();
            node->data = reader.read_string(data_size);

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
                if (r < 0 || static_cast<std::uint64_t>(r) >= nodes_count) {
                    throw (std::invalid_argument("Invalid rand index"));
                }

                nodes[i]->rand = nodes[static_cast<std::size_t>(r)];
            }
        }

        return nodes.empty() ? nullptr : std::unique_ptr<common::ListNode>(nodes.front());
    }
//-------------------------------------------------------------------------//
}; // namespace adapters
