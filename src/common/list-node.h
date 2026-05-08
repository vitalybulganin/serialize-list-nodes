//
// Created by bulganin-va on 06.05.2026.
//

#ifndef __LIST_NODE_H_A046BA63_9A29_4F4D_B38D_C362263BF972__
#define __LIST_NODE_H_A046BA63_9A29_4F4D_B38D_C362263BF972__
//-------------------------------------------------------------------------//
#include <string>
//-------------------------------------------------------------------------//
namespace common {
//-------------------------------------------------------------------------//
    struct ListNode final {
        //!< Keeps a previous node.
        ListNode *prev = nullptr;

        //!< Keeps next node.
        ListNode *next = nullptr;

        //!< Keeps a pointer on a node.
        ListNode *rand = nullptr;

        //!< Keeps a node data.
        std::string data;

    public:
        ListNode(const ListNode &) = delete;
        ListNode(ListNode &&) = delete;

    public:
        //!< Constructor.
        ListNode() = default;

        /**
         * Destructor.
         * @throw None.
         */
        ~ListNode() noexcept;
    };
//-------------------------------------------------------------------------//
}; // namespace common
//-------------------------------------------------------------------------//
#endif // __LIST_NODE_H_A046BA63_9A29_4F4D_B38D_C362263BF972__
