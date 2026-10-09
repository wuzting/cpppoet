// Copyright 2026 wuzting
// SPDX-License-Identifier: MIT

/// @file version_test.cc
/// @brief Tests for the generated version header.

#include <cpppoet/version.h>

#include <string>

#include "test_framework.h"

CPPPOET_TEST(Version_string_is_not_empty) {
    EXPECT_TRUE(std::string(cpppoet::Version()).size() > 0);
}

CPPPOET_TEST(Version_macro_matches_accessor) {
    EXPECT_EQ(std::string(CPPPOET_VERSION), std::string(cpppoet::Version()));
}
