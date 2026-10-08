// Copyright 2026 wuzting
// SPDX-License-Identifier: MIT

/// @file free_function_spec.h
/// @brief Declares FreeFunctionSpec, a fluent builder for free functions.

#pragma once

#include <string>
#include <vector>

#include "code_block.h"
#include "cpppoet/export.h"
#include "parameter_spec.h"
#include "type_name.h"
#include "types.h"

namespace cpppoet {

namespace detail {
class Renderer;
}

class CPPPOET_EXPORT FreeFunctionSpec {
public:
    /// @brief Creates a free function builder.
    /// @param name The function name.
    /// @return A new FreeFunctionSpec.
    static FreeFunctionSpec Create(const std::string& name);

    /// @brief Sets the return type.
    /// @param type The return type.
    /// @return This builder.
    FreeFunctionSpec& SetReturnType(const TypeName& type);
    /// @brief Adds a function modifier.
    /// @param modifier The modifier to add.
    /// @return This builder.
    FreeFunctionSpec& AddModifier(Modifier modifier);
    /// @brief Adds a parameter.
    /// @param param The parameter to add.
    /// @return This builder.
    FreeFunctionSpec& AddParameter(const ParameterSpec& param);
    /// @brief Appends a code block to the body.
    /// @param code The block to append.
    /// @return This builder.
    FreeFunctionSpec& AddCode(const CodeBlock& code);
    /// @brief Appends a statement to the body.
    /// @param format A format string with placeholder arguments.
    /// @param args Values for the placeholders.
    /// @return This builder.
    FreeFunctionSpec& AddStatement(const std::string& format,
                                   std::vector<CodeBlockArg> args = {});

    /// @brief Returns the function name.
    /// @return The function name.
    const std::string& Name() const;

private:
    friend class detail::Renderer;

    std::string name_;
    TypeName return_type_;
    std::vector<Modifier> modifiers_;
    std::vector<ParameterSpec> parameters_;
    CodeBlock code_;
};

}  // namespace cpppoet
