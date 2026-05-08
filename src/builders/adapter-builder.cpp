#include "../adapter-builder.h"
//-------------------------------------------------------------------------//
#include "../adapters/text-adapter.h"
#include "../adapters/binary-adapter.h"
#include "../adapters/binary-file-adapter.h"
//-------------------------------------------------------------------------//
namespace builders {
//-------------------------------------------------------------------------//
    auto AdapterBuilder::set(adapter_types type) -> AdapterBuilder & {
        return this->type = type, *this;
    }

    auto AdapterBuilder::set_input_file(const std::string &fname) -> AdapterBuilder & {
        return this->input_file = fname, *this;
    }

    auto AdapterBuilder::set_output_file(const std::string &fname) -> AdapterBuilder & {
        return this->output_file = fname, *this;
    }

    auto AdapterBuilder::build() const -> common::adapter_t {
        if (this->type == adapter_types::text) {
            return common::adapter_t(new adapters::TextAdapter());
        } else if (this->type == adapter_types::binary) {
            return common::adapter_t(new adapters::BinaryAdapter());
        } else if (this->type == adapter_types::file_binary) {
            return common::adapter_t(new adapters::BinaryFileAdapter(this->input_file, this->output_file));
        }
        throw (std::invalid_argument("Unknown adapter type: " + std::to_string(static_cast<int>(this->type))));
    }
//-------------------------------------------------------------------------//
    auto AdapterBuilder::validate() const -> void {
        if (this->type == adapter_types::none) {
            throw (std::invalid_argument("Adapter type doesn't set"));
        }

        if (this->type == adapter_types::file_binary && this->input_file.empty()) {
            throw (std::invalid_argument("Input file name doesn't set"));
        }

        if (this->type == adapter_types::file_binary && this->output_file.empty()) {
            throw (std::invalid_argument("Output file name doesn't set"));
        }
    }
//-------------------------------------------------------------------------//
    auto make_adapter() -> AdapterBuilder {
        return {};
    }
//-------------------------------------------------------------------------//
}; // namespace builders
