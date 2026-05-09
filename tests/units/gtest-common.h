/*!==========================================================================
* \file
* - Program:       gtest-serialize-list-nodes
* - File:          gtest-common.h
* - Created:       05/08/2026
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
#ifndef __GTEST_COMMON_H_EE0029F0_72C4_49C4_A819_625008437801__
#define __GTEST_COMMON_H_EE0029F0_72C4_49C4_A819_625008437801__
//-------------------------------------------------------------------------//
#include <string>
#include <vector>
//-------------------------------------------------------------------------//
#include <gtest/gtest.h>
//-------------------------------------------------------------------------//
#include "common/list-node.h"
//-------------------------------------------------------------------------//
struct ListDeleter final {
    void operator()(common::ListNode *head) const noexcept {
        delete head;
    }
};
using list_t = std::unique_ptr<common::ListNode, ListDeleter>;
//-------------------------------------------------------------------------//
list_t make_list(const std::vector<std::string> &values, const std::vector<int> &rand_indices) {
    if (values.size() != rand_indices.size()) {
        throw std::invalid_argument("values/rand_indices size mismatch");
    }

    std::vector<common::ListNode *> nodes;
    nodes.reserve(values.size());

    for (const auto &value : values) {
        auto* node = new common::ListNode();
        node->data = value;

        if (!nodes.empty()) {
            node->prev = nodes.back();
            nodes.back()->next = node;
        }

        nodes.push_back(node);
    }

    for (auto i = 0; i < nodes.size(); ++i) {
        const int r = rand_indices[i];
        if (r == -1) {
            nodes[i]->rand = nullptr;
        } else {
            if (r < 0 || static_cast<std::size_t>(r) >= nodes.size()) {
                throw std::invalid_argument("invalid rand index in test data");
            }
            nodes[i]->rand = nodes[static_cast<std::size_t>(r)];
        }
    }

    return list_t(nodes.empty() ? nullptr : nodes.front());
}

std::vector<common::ListNode *> collect_nodes(common::ListNode *head) {
    std::vector<common::ListNode *> nodes;
    for (auto *cur = head; cur != nullptr; cur = cur->next) {
        nodes.push_back(cur);
    }
    return nodes;
}

std::string tmp_file(const std::string& name) {
    return ::testing::TempDir() + name;
}

std::vector<std::uint8_t> as_buffer(const std::string& text) {
    return {text.begin(), text.end()};
}

void expect_list_equals(common::ListNode *head,
                        const std::vector<std::string> &values,
                        const std::vector<int> &rand_indices) {
    const auto nodes = collect_nodes(head);

    ASSERT_EQ(nodes.size(), values.size());
    ASSERT_EQ(rand_indices.size(), values.size());

    for (std::size_t i = 0; i < nodes.size(); ++i) {
        EXPECT_EQ(nodes[i]->data, values[i]) << "data mismatch at index " << i;

        if (i == 0) {
            EXPECT_EQ(nodes[i]->prev, nullptr);
        } else {
            EXPECT_EQ(nodes[i]->prev, nodes[i - 1]) << "prev mismatch at index " << i;
        }

        if (i + 1 == nodes.size()) {
            EXPECT_EQ(nodes[i]->next, nullptr);
        } else {
            EXPECT_EQ(nodes[i]->next, nodes[i + 1]) << "next mismatch at index " << i;
        }

        const int r = rand_indices[i];
        if (r == -1) {
            EXPECT_EQ(nodes[i]->rand, nullptr) << "rand mismatch at index " << i;
        } else {
            ASSERT_GE(r, 0);
            ASSERT_LT(static_cast<std::size_t>(r), nodes.size());
            EXPECT_EQ(nodes[i]->rand, nodes[static_cast<std::size_t>(r)]) << "rand mismatch at index " << i;
        }
    }
}
//-------------------------------------------------------------------------//
#endif // __GTEST_COMMON_H_EE0029F0_72C4_49C4_A819_625008437801__
