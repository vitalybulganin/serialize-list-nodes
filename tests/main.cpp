/*!==========================================================================
* \file
* - Program:       gtest-serialize-list-nodes
* - File:          main.cpp
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
#include <gtest/gtest.h>
//-------------------------------------------------------------------------//
#define BUILD_TESTING
//-------------------------------------------------------------------------//
#include "units/gtest-text-adapter.h"
#include "units/gtest-binary-adapter.h"
#include "units/gtest-binary-file-adapter.h"
//-------------------------------------------------------------------------//
int main(int argc, char ** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
