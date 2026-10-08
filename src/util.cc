// Copyright 2026 wuzting
// SPDX-License-Identifier: MIT

/// @file util.cc
/// @brief Implements the internal string helpers declared in util.h.

#include "util.h"

namespace cpppoet {
using std::string;

string StringLiteral(const string& value) {
    string result = "\"";
    for (char c : value) {
        switch (c) {
            case '\\':
                result += "\\\\";
                break;
            case '"':
                result += "\\\"";
                break;
            case '\n':
                result += "\\n";
                break;
            case '\t':
                result += "\\t";
                break;
            default:
                result.push_back(c);
                break;
        }
    }
    result += "\"";
    return result;
}

}  // namespace cpppoet
