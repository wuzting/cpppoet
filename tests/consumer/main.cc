// Copyright 2026 wuzting
// SPDX-License-Identifier: MIT

/// @file main.cc
/// @brief Smoke test for consuming an installed cpppoet package.

#include <cpppoet/cpppoet.h>

#include <iostream>
#include <string>

int main() {
    cpppoet::CodeBlock block = cpppoet::CodeBlock::NewBuilder()
                                   .AddStatement("return $L", {"0"})
                                   .Build();
    const std::string code = block.ToString();
    std::cout << code;
    return code == "return 0;\n" ? 0 : 1;
}
