/*!==========================================================================
* \file
* - Program:       gtest-serialize-list-nodes
* - File:          gtest-adapter.h
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
#ifndef __GTEST_TEXT_ADAPTER_H_EE0029F0_72C4_49C4_A819_625008437801__
#define __GTEST_TEXT_ADAPTER_H_EE0029F0_72C4_49C4_A819_625008437801__
//-------------------------------------------------------------------------//
#include <gtest/gtest.h>
//-------------------------------------------------------------------------//
#include <adapter-builder.h>
//-------------------------------------------------------------------------//
TEST(TestTextAdapter, serialize) {
    auto adapter = builders::make_adapter().set(builders::adapter_types::text).set_input_file("input/inlet.in").build();
    ASSERT_TRUE(adapter);
    // adapter->deserialize()
}

TEST(TestTextAdapter, deserialize) {
}
//-------------------------------------------------------------------------//
#endif // __GTEST_TEXT_ADAPTER_H_EE0029F0_72C4_49C4_A819_625008437801__
