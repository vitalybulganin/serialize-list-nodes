/*!==========================================================================
* \file
* - Program:       pack-list
* - File:          file-binary-adapter.h
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
#ifndef __FILE_BINARY_ADAPTER_SERIALIZER_H_A046BA63_9A29_4F4D_B38D_C362263BF972__
#define __FILE_BINARY_ADAPTER_SERIALIZER_H_A046BA63_9A29_4F4D_B38D_C362263BF972__
//-------------------------------------------------------------------------//
#include "../common/adapter.h"
//-------------------------------------------------------------------------//
namespace builders {
    class AdapterBuilder;
}; // namespace builders
//-------------------------------------------------------------------------//
namespace adapters {
//-------------------------------------------------------------------------//
    class BinaryFileAdapter : public common::Adapter {
        friend class builders::AdapterBuilder;
        //!< Keeps a name of inpout file.
        const std::string input_file;

        //!< Keeps a name of output file.
        const std::string output_file;

        // Override methods
    public:
        /**
         * Serialize an object.
         * @return Serialized object.
         */
        virtual auto serialize(const common::ListNode *node) const -> std::vector<std::uint8_t> override;

        /**
         * Deserialize an object.
         * @param data [in] - A pointer on buffer.
         * @return A head list node.
         */
        virtual auto deserialize(const std::uint8_t *data, size_t size) -> std::unique_ptr<common::ListNode> override;

    protected:
        /**
         * Constructor.
         * @param input [in] - A name of input binary file.
         * @param output [int] - A name of output binary file.
         */
        explicit BinaryFileAdapter(std::string input, std::string output);
    };
//-------------------------------------------------------------------------//
}; // namespace adapters
//-------------------------------------------------------------------------//
#endif // __FILE_BINARY_ADAPTER_SERIALIZER_H_A046BA63_9A29_4F4D_B38D_C362263BF972__
