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
#include <fstream>
//-------------------------------------------------------------------------//
#include <gtest/gtest.h>
//-------------------------------------------------------------------------//
#include <adapter-builder.h>
//-------------------------------------------------------------------------//
#include "gtest-common.h"
//-------------------------------------------------------------------------//
TEST(BinaryFileAdapter, SerializeDeserializeFile) {
    const auto file = tmp_file("list_serialization_binary_file.bin");
    auto source = make_list({"apple", "banana", "carrot"}, {2, -1, 1});

    auto writer = builders::make_adapter()
        .set(builders::adapter_types::file_binary)
        .set_output_file(file)
        .set_input_file(file)
        .build();

    writer->serialize(source.get());
    auto restored = writer->deserialize(nullptr, 0);

    expect_list_equals(restored.get(), {"apple", "banana", "carrot"}, {2, -1, 1});

    std::remove(file.c_str());
}

TEST(BinaryFileAdapter, SerializeDeserializeEmptyFileList) {
    const auto file = tmp_file("empty_list_binary_file.bin");

    auto adapter = builders::make_adapter()
        .set(builders::adapter_types::file_binary)
        .set_output_file(file)
        .set_input_file(file)
        .build();

    adapter->serialize(nullptr);
    auto restored = adapter->deserialize(nullptr, 0);

    EXPECT_EQ(restored.get(), nullptr);

    std::remove(file.c_str());
}

TEST(BinaryFileAdapter, RejectsTruncatedFile) {
    const auto file = tmp_file("truncated_binary_file.bin");

    {
        std::ofstream out(file, std::ios::binary);
        const std::uint64_t count = 1;
        out.write(reinterpret_cast<const char*>(&count), sizeof(count));
    }

    auto adapter = builders::make_adapter()
        .set(builders::adapter_types::file_binary)
        .set_input_file(file)
        .set_output_file(file)
        .build();

    EXPECT_THROW(
        static_cast<void>(adapter->deserialize(nullptr, 0)),
        std::invalid_argument
    );

    std::remove(file.c_str());
}
//-------------------------------------------------------------------------//
#endif // __GTEST_BINARY_FILE_ADAPTER_H_EE0029F0_72C4_49C4_A819_625008437801__
