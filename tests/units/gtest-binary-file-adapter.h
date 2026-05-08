/*!==========================================================================
* \file
* - Program:       gtest-serialize-list-nodes
* - File:          gtest-binary-file-adapter.h
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
#ifndef __GTEST_BINARY_FILE_ADAPTER_H_EE0029F0_72C4_49C4_A819_625008437801__
#define __GTEST_BINARY_FILE_ADAPTER_H_EE0029F0_72C4_49C4_A819_625008437801__
//-------------------------------------------------------------------------//
#include <gtest/gtest.h>
//-------------------------------------------------------------------------//
#include <adapter-builder.h>
//-------------------------------------------------------------------------//
inline auto compare(const std::unique_ptr<common::ListNode> &left, const std::unique_ptr<common::ListNode> &right) -> int {
    return 0;
}
//-------------------------------------------------------------------------//
TEST(TestBinaryFileAdapter, serialize) {
    auto expected = std::vector<std::uint8_t>();
    auto head = std::make_unique<common::ListNode>();
    auto adapter = builders::make_adapter().set(builders::adapter_types::binary).build();
    ASSERT_TRUE(adapter);

    auto buffer = adapter->serialize(head.get());
    EXPECT_FALSE(buffer.empty());
    EXPECT_EQ(buffer, expected);
}

TEST(TestFileAdapter, deserialize) {
    auto buffer = std::vector<std::uint8_t>();
    auto expected = std::unique_ptr<common::ListNode>();
    auto adapter = builders::make_adapter().set(builders::adapter_types::binary).build();
    ASSERT_TRUE(adapter);

    auto head = adapter->deserialize(buffer.data(), buffer.size());
    ASSERT_TRUE(head);
    EXPECT_EQ(compare(head, expected), 0);
}
//-------------------------------------------------------------------------//
#endif // __GTEST_BINARY_FILE_ADAPTER_H_EE0029F0_72C4_49C4_A819_625008437801__
