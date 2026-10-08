// Copyright 2026 wuzting
// SPDX-License-Identifier: MIT

/// @file code_buffer.cc
/// @brief Implements the internal CodeBuffer.

#include "code_buffer.h"

namespace cpppoet {
namespace detail {

using std::string;

CodeBuffer::CodeBuffer(const string& indent) : indent_(indent) {}

CodeBuffer& CodeBuffer::Indent(int levels) {
    indent_level_ += levels;
    return *this;
}

CodeBuffer& CodeBuffer::Unindent(int levels) {
    indent_level_ -= levels;
    if (indent_level_ < 0) {
        indent_level_ = 0;
    }
    return *this;
}

CodeBuffer& CodeBuffer::Emit(const string& text) {
    for (char c : text) {
        if (at_line_start_ && c != '\n') {
            WriteIndent();
            at_line_start_ = false;
        }
        buffer_.push_back(c);
        if (c == '\n') {
            at_line_start_ = true;
        }
    }
    return *this;
}

CodeBuffer& CodeBuffer::Newline() { return Emit("\n"); }

CodeBuffer& CodeBuffer::Append(const string& text) {
    buffer_ += text;
    if (!text.empty()) {
        at_line_start_ = text.back() == '\n';
    }
    return *this;
}

const string& CodeBuffer::ToString() const { return buffer_; }

void CodeBuffer::WriteIndent() {
    for (int i = 0; i < indent_level_; ++i) {
        buffer_ += indent_;
    }
}

}  // namespace detail
}  // namespace cpppoet
