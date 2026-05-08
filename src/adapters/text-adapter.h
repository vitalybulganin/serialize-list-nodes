/*!==========================================================================
* \file
* - Program:       pack-list
* - File:          text-adapter.h
* - Created:       05/06/2027
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
#ifndef __TEXT_ADAPTER_SERIALIZER_H_A046BA63_9A29_4F4D_B38D_C362263BF972__
#define __TEXT_ADAPTER_SERIALIZER_H_A046BA63_9A29_4F4D_B38D_C362263BF972__
//-------------------------------------------------------------------------//
#include "../common/adapter.h"
//-------------------------------------------------------------------------//
namespace builders {
    class AdapterBuilder;
}; // namespace builders
//-------------------------------------------------------------------------//
namespace adapters {
//-------------------------------------------------------------------------//
    class TextAdapter : public common::Adapter {
        friend class builders::AdapterBuilder;

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
        //!< Constructor.
        explicit TextAdapter() = default;
    };
//-------------------------------------------------------------------------//
}; // namespace adapters
//-------------------------------------------------------------------------//
#endif // __TEXT_ADAPTER_SERIALIZER_H_A046BA63_9A29_4F4D_B38D_C362263BF972__
