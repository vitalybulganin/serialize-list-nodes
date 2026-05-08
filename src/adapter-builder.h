/*!==========================================================================
* \file
* - Program:       pack-list
* - File:          adapter-builder.h
* - Created:       05/06/2026
* - Author:        Vitaly Bulganin
* - Description:
* - Comments:
*
-----------------------------------------------------------------------------
*
* - History:
*
===========================================================================*/
#pragma once
//-------------------------------------------------------------------------//
#ifndef __ADAPTER_BUILDER_H_A046BA63_9A29_4F4D_B38D_C362263BF972__
#define __ADAPTER_BUILDER_H_A046BA63_9A29_4F4D_B38D_C362263BF972__
//-------------------------------------------------------------------------//
#include <cstdint>
#include <string>
//-------------------------------------------------------------------------//
#include "common/adapter.h"
//-------------------------------------------------------------------------//
namespace builders {
//-------------------------------------------------------------------------//
    enum class adapter_types : std::uint8_t {
        none = 0,
        text,
        binary,
        file_binary
    };
//-------------------------------------------------------------------------//
    class AdapterBuilder final {
        friend auto make_adapter() -> AdapterBuilder;

        //!< Keeps a type of builder.
        adapter_types type = adapter_types::none;

        //!< Keeps a name of input file.
        std::string input_file;

        //!< Keeps a name of output file.
        std::string output_file;

    public:
        /**
         * Sets a type of adapter.
         * @param type [in] - Adapter type.
         * @return The reference on the instance.
         */
        auto set(adapter_types type) -> AdapterBuilder &;

        /**
         * Sets a name of input file.
         * @param fname [in] - A name of file.
         * @return The reference on the instance.
         */
        auto set_input_file(const std::string &fname) -> AdapterBuilder &;

        /**
         * Sets a name of output file.
         * @param fname [in] - A name of file.
         * @return The reference on the instance.
         */
        auto set_output_file(const std::string &fname) -> AdapterBuilder &;

        /**
         * Builds a new instance of adapter.
         * @return A new adapter.
         */
        auto build() const -> common::adapter_t;

    private:
        //!< Constructor.
        AdapterBuilder() = default;

        //!< Validates instance.
        auto validate() const -> void;
    };
//-------------------------------------------------------------------------//
    auto make_adapter() -> AdapterBuilder;
//-------------------------------------------------------------------------//
}; // namespace builders
//-------------------------------------------------------------------------//
#endif // __ADAPTER_BUILDER_H_A046BA63_9A29_4F4D_B38D_C362263BF972__
