// Copyright 2026 wuzting
// SPDX-License-Identifier: MIT

/// @file code_buffer.h
/// @brief Declares CodeBuffer, the internal indentation-aware output buffer.

#pragma once

#include <string>

namespace cpppoet {
namespace detail {

/// @brief Appends text while tracking indentation.
///
/// This is an implementation detail of the renderer: it is not part of the
/// installed public API and its symbols are not exported.
class CodeBuffer {
public:
    /// @brief Creates a buffer.
    /// @param indent The indentation string for one level.
    CodeBuffer(const std::string& indent = "    ");

    /// @brief Increases the indentation level.
    /// @param levels How many levels to add.
    /// @return This buffer.
    CodeBuffer& Indent(int levels = 1);
    /// @brief Decreases the indentation level.
    /// @param levels How many levels to remove.
    /// @return This buffer.
    CodeBuffer& Unindent(int levels = 1);

    /// @brief Appends text, indenting at the start of each line.
    /// @param text The text to append.
    /// @return This buffer.
    CodeBuffer& Emit(const std::string& text);

    /// @brief Appends a newline.
    /// @return This buffer.
    CodeBuffer& Newline();

    /// @brief Appends text verbatim, without applying indentation.
    /// @param text The text to append.
    /// @return This buffer.
    CodeBuffer& Append(const std::string& text);

    /// @brief Returns the accumulated output.
    /// @return The rendered text.
    const std::string& ToString() const;

private:
    void WriteIndent();

    std::string indent_;
    int indent_level_ = 0;
    bool at_line_start_ = true;
    std::string buffer_;
};

}  // namespace detail

}  // namespace cpppoet
