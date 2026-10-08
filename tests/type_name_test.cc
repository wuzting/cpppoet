// Copyright 2026 wuzting
// SPDX-License-Identifier: MIT

/// @file type_name_test.cc
/// @brief Tests for TypeName rendering and include headers.

#include "helpers.h"
#include "test_framework.h"

using namespace cpppoet;

CPPPOET_TEST(TypeName_basic_forms) {
    EXPECT_EQ(std::string("uint32_t"), TypeName::Of("uint32_t").ToString());
    EXPECT_EQ(std::string("const int&"),
              TypeName::Of("int").AsConst().AsRef().ToString());
    EXPECT_EQ(std::string("uint8_t*"),
              TypeName::Of("uint8_t").AsPtr().ToString());
}

CPPPOET_TEST(TypeName_header_association) {
    TypeName sys = TypeName::Of("uint32_t").WithSystemHeader("cstdint");
    EXPECT_EQ(std::string("cstdint"), sys.SystemHeader());
    EXPECT_TRUE(sys.LocalHeader().empty());

    TypeName local = TypeName::Of("Widget").WithLocalHeader("widget.h");
    EXPECT_EQ(std::string("widget.h"), local.LocalHeader());
    EXPECT_TRUE(local.SystemHeader().empty());
}

CPPPOET_TEST(TypeName_parameterized) {
    TypeName vec = TypeName::Parameterized(
        "std::vector", {TypeName::Of("uint8_t").WithSystemHeader("cstdint")});
    EXPECT_EQ(std::string("std::vector<uint8_t>"), vec.ToString());
    EXPECT_EQ(static_cast<size_t>(1), vec.TemplateArgs().size());
}

CPPPOET_TEST(TypeName_empty_and_equality) {
    EXPECT_TRUE(TypeName().Empty());
    EXPECT_TRUE(TypeName().ToString().empty());
    EXPECT_TRUE(TypeName::Of("a") == TypeName::Of("a"));
    EXPECT_TRUE(TypeName::Of("a") != TypeName::Of("b"));
}
