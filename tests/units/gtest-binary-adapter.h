/*!==========================================================================
* \file
* - Program:       gtest-serialize-list-nodes
* - File:          gtest-binary-adapter.h
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
#ifndef __GTEST_BINARY_ADAPTER_H_EE0029F0_72C4_49C4_A819_625008437801__
#define __GTEST_BINARY_ADAPTER_H_EE0029F0_72C4_49C4_A819_625008437801__
//-------------------------------------------------------------------------//
#include <gtest/gtest.h>
//-------------------------------------------------------------------------//
#include <adapter-builder.h>
//-------------------------------------------------------------------------//
#include "gtest-common.h"
//-------------------------------------------------------------------------//
TEST(TestBinaryAdapter, SerializeDeserializeEmptyList) {
    auto adapter = builders::make_adapter()
        .set(builders::adapter_types::binary)
        .build();

    const auto buffer = adapter->serialize(nullptr);
    auto restored = adapter->deserialize(buffer.data(), buffer.size());

    EXPECT_EQ(restored.get(), nullptr);
}

TEST(TestBinaryAdapter, SerializeDeserializeSingleNodeNullRand) {
    auto source = make_list({"apple"}, {-1});
    auto adapter = builders::make_adapter()
        .set(builders::adapter_types::binary)
        .build();

    const auto buffer = adapter->serialize(source.get());
    auto restored = adapter->deserialize(buffer.data(), buffer.size());

    expect_list_equals(restored.get(), {"apple"}, {-1});
}

TEST(TestBinaryAdapter, SerializeDeserializeSingleNodeSelfRand) {
    auto source = make_list({"self"}, {0});
    auto adapter = builders::make_adapter()
        .set(builders::adapter_types::binary)
        .build();

    const auto buffer = adapter->serialize(source.get());
    auto restored = adapter->deserialize(buffer.data(), buffer.size());

    expect_list_equals(restored.get(), {"self"}, {0});
}

TEST(TestBinaryAdapter, SerializeDeserializeSeveralNodes) {
    auto source = make_list({"apple", "banana", "carrot"}, {2, -1, 1});
    auto adapter = builders::make_adapter()
        .set(builders::adapter_types::binary)
        .build();

    const auto buffer = adapter->serialize(source.get());
    auto restored = adapter->deserialize(buffer.data(), buffer.size());

    expect_list_equals(restored.get(), {"apple", "banana", "carrot"}, {2, -1, 1});
}

TEST(TestBinaryAdapter, SerializeDeserializeUtf8AndSpecialChars) {
    auto source = make_list(
        {"hello;world", "строка с пробелами", "emoji 🚀🔥", "line\\nnot_real_newline"},
        {3, 0, -1, 2}
    );

    auto adapter = builders::make_adapter()
        .set(builders::adapter_types::binary)
        .build();

    const auto buffer = adapter->serialize(source.get());
    auto restored = adapter->deserialize(buffer.data(), buffer.size());

    expect_list_equals(
        restored.get(),
        {"hello;world", "строка с пробелами", "emoji 🚀🔥", "line\\nnot_real_newline"},
        {3, 0, -1, 2}
    );
}

TEST(TestBinaryAdapter, SerializeDeserializeMaxStringSize) {
    const std::string big(1000, 'x');
    auto source = make_list({big}, {-1});
    auto adapter = builders::make_adapter()
        .set(builders::adapter_types::binary)
        .build();

    const auto buffer = adapter->serialize(source.get());
    auto restored = adapter->deserialize(buffer.data(), buffer.size());

    expect_list_equals(restored.get(), {big}, {-1});
}

TEST(TestBinaryAdapter, RejectsTruncatedBuffer) {
    auto adapter = builders::make_adapter()
        .set(builders::adapter_types::binary)
        .build();

    const std::vector<std::uint8_t> buffer = {1, 0, 0};

    EXPECT_THROW(
        static_cast<void>(adapter->deserialize(buffer.data(), buffer.size())),
        std::invalid_argument
    );
}

TEST(TestBinaryAdapter, RejectsRandOutsideList) {
    auto first = std::make_unique<common::ListNode>();
    auto foreign = std::make_unique<common::ListNode>();

    first->data = "first";
    foreign->data = "foreign";
    first->rand = foreign.get();

    auto adapter = builders::make_adapter()
        .set(builders::adapter_types::binary)
        .build();

    EXPECT_THROW(
        static_cast<void>(adapter->serialize(first.get())),
        std::invalid_argument
    );
}
//-------------------------------------------------------------------------//
#endif // __GTEST_ADAPTER_H_EE0029F0_72C4_49C4_A819_625008437801__
