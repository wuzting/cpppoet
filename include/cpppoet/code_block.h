// Copyright 2026 wuzting
// SPDX-License-Identifier: MIT

/// @file code_block.h
/// @brief Declares CodeBlock and CodeBlockArg, JavaPoet-style placeholder code
/// blocks.

#pragma once

#include <string>
#include <vector>

#include "cpppoet/export.h"
#include "type_name.h"

namespace cpppoet {

namespace detail {
class Renderer;
}

class CPPPOET_EXPORT CodeBlockArg {
public:
    /// @brief Wraps a literal code fragment.
    /// @param text The text to wrap.
    CodeBlockArg(std::string text);
    /// @brief Wraps a literal code fragment.
    /// @param text The text to wrap.
    CodeBlockArg(const char* text);
    /// @brief Wraps a type so that $T can record its include header.
    /// @param type The type to wrap.
    CodeBlockArg(const TypeName& type);

    /// @brief Whether the argument holds text or a type.
    enum class Kind {
        kString,  ///< literal text
        kType,    ///< a type, so $T can record its include header
    };

    /// @brief Returns whether the argument holds text or a type.
    /// @return kString or kType.
    Kind GetKind() const;
    /// @brief Returns the literal text.
    /// @return The literal text.
    const std::string& Text() const;
    /// @brief Returns the wrapped type.
    /// @return The wrapped type.
    const TypeName& Type() const;

private:
    Kind kind_;
    std::string text_;
    TypeName type_;
};

/// @brief A code block with JavaPoet-style placeholders.
///
/// Placeholders: @c $L literal, @c $N name, @c $S escaped string, @c $T type,
/// @c $$ dollar sign, @c $> indent, @c $< unindent. Use BeginControlFlow,
/// NextControlFlow and EndControlFlow for control flow.
class CPPPOET_EXPORT CodeBlock {
public:
    CodeBlock() = default;

    class Builder {
    public:
        /// @brief Appends a format string with placeholders.
        /// @param format The format string using $L, $N, $S, $T, $$, $> or $<.
        /// @param args Values for the placeholders.
        /// @return This builder.
        Builder& Add(const std::string& format,
                     std::vector<CodeBlockArg> args = {});
        /// @brief Appends every part of another block.
        /// @param block The block to append.
        /// @return This builder.
        Builder& Add(const CodeBlock& block);
        /// @brief Opens a control-flow block.
        /// @param control_flow The header, such as if (...).
        /// @param args Values for placeholders in the header.
        /// @return This builder.
        Builder& BeginControlFlow(const std::string& control_flow,
                                  std::vector<CodeBlockArg> args = {});
        /// @brief Closes the current branch and opens the next.
        /// @param control_flow The next clause, such as else if (...).
        /// @param args Values for placeholders in the clause.
        /// @return This builder.
        Builder& NextControlFlow(const std::string& control_flow,
                                 std::vector<CodeBlockArg> args = {});
        /// @brief Closes the current control-flow block.
        /// @return This builder.
        Builder& EndControlFlow();
        /// @brief Closes the block with a trailing clause.
        /// @param control_flow The trailing clause, such as while (...).
        /// @param args Values for placeholders in the clause.
        /// @return This builder.
        Builder& EndControlFlow(const std::string& control_flow,
                                std::vector<CodeBlockArg> args = {});
        /// @brief Appends a statement and a terminating semicolon.
        /// @param format The statement format string.
        /// @param args Values for the placeholders.
        /// @return This builder.
        Builder& AddStatement(const std::string& format,
                              std::vector<CodeBlockArg> args = {});
        /// @brief Increases the indentation level.
        /// @return This builder.
        Builder& Indent();
        /// @brief Decreases the indentation level.
        /// @return This builder.
        Builder& Unindent();
        /// @brief Builds the code block.
        /// @return The finished block.
        CodeBlock Build();

    private:
        friend class CodeBlock;
        std::vector<std::string> parts_;
        std::vector<CodeBlockArg> args_;
    };

    /// @brief Returns a new builder.
    /// @return A new Builder.
    static Builder NewBuilder();

    /// @brief Returns whether the block is empty.
    /// @return True if the block has no parts.
    bool IsEmpty() const;

    /// @brief Renders the block as source text.
    /// @return The rendered block.
    std::string ToString() const;

private:
    friend class Builder;
    friend class detail::Renderer;
    std::vector<std::string> parts_;
    std::vector<CodeBlockArg> args_;
};

}  // namespace cpppoet
