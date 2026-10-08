// Copyright 2026 wuzting
// SPDX-License-Identifier: MIT

/// @file code_block_test.cc
/// @brief Tests for CodeBlock placeholder expansion.

#include "helpers.h"
#include "test_framework.h"

using namespace cpppoet;

CPPPOET_TEST(CodeBlock_literal_and_type_placeholders) {
    auto builder = CodeBlock::NewBuilder();
    builder.Add("$T total = $L", {th::SysType("size_t", "cstddef"), "0"});
    EXPECT_EQ(std::string("size_t total = 0"), builder.Build().ToString());
}

CPPPOET_TEST(CodeBlock_statement_and_for_control_flow) {
    auto builder = CodeBlock::NewBuilder();
    builder.AddStatement("$T total = $L",
                         {th::SysType("size_t", "cstddef"), "0"});
    builder
        .BeginControlFlow("for ($T i = $L; i < $N; ++i)",
                          {th::SysType("size_t", "cstddef"), "0", "len"})
        .AddStatement("total += $N", {"data[i]"})
        .EndControlFlow();
    EXPECT_EQ(std::string("size_t total = 0;\n"
                          "for (size_t i = 0; i < len; ++i) {\n"
                          "    total += data[i];\n"
                          "}\n"),
              builder.Build().ToString());
}

CPPPOET_TEST(CodeBlock_if_else_and_string_escape) {
    auto builder = CodeBlock::NewBuilder();
    builder.BeginControlFlow("if ($N == $L)", {"total", "0xFF"})
        .AddStatement("log($S)", {"all \"ones\""})
        .NextControlFlow("else")
        .AddStatement("log($S)", {"mixed"})
        .EndControlFlow();
    EXPECT_EQ(std::string("if (total == 0xFF) {\n"
                          "    log(\"all \\\"ones\\\"\");\n"
                          "} else {\n"
                          "    log(\"mixed\");\n"
                          "}\n"),
              builder.Build().ToString());
}

CPPPOET_TEST(CodeBlock_escapes_dollar_sign) {
    auto builder = CodeBlock::NewBuilder();
    builder.Add("cost $$5");
    EXPECT_EQ(std::string("cost $5"), builder.Build().ToString());
}

CPPPOET_TEST(CodeBlock_manual_indent_control) {
    auto builder = CodeBlock::NewBuilder();
    builder.Add("a\n").Indent().Add("b\n").Unindent().Add("c\n");
    EXPECT_EQ(std::string("a\n    b\nc\n"), builder.Build().ToString());
}
