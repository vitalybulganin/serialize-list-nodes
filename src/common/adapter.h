/*!==========================================================================
* \file
* - Program:       pack-list
* - File:          adapter.h
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
#ifndef __ADAPTER_H_A046BA63_9A29_4F4D_B38D_C362263BF972__
#define __ADAPTER_H_A046BA63_9A29_4F4D_B38D_C362263BF972__
//-------------------------------------------------------------------------//
#include <vector>
#include <memory>
//-------------------------------------------------------------------------//
#include "list-node.h"
//-------------------------------------------------------------------------//
namespace common {
//-------------------------------------------------------------------------//
    struct Adapter {
        /**
         * Serialize an object.
         * @return Serialized object.
         */
        virtual auto serialize(const ListNode *node) const -> std::vector<std::uint8_t> = 0;

        /**
         * Deserialize an object.
         * @param data [in] - A pointer on buffer.
         * @param size [in] - A size of buffer.
         * @return A head list node.
         */
        virtual auto deserialize(const std::uint8_t *data, size_t size) -> std::unique_ptr<ListNode> = 0;

        /**
         * Destructor.
         * @throw None.
         */
        virtual ~Adapter() noexcept = default;
    };
//-------------------------------------------------------------------------//
    using adapter_t = std::shared_ptr<Adapter>;
//-------------------------------------------------------------------------//
}; // namespace common
//-------------------------------------------------------------------------//
#endif // __ADAPTER_H_A046BA63_9A29_4F4D_B38D_C362263BF972__
