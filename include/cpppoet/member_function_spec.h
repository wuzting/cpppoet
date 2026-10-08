// Copyright 2026 wuzting
// SPDX-License-Identifier: MIT

/// @file member_function_spec.h
/// @brief Declares MemberFunctionSpec, a fluent builder for member functions
/// and constructors.

#pragma once

#include <string>
#include <utility>
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

class CPPPOET_EXPORT MemberFunctionSpec {
public:
    /// @brief Creates a member function builder.
    /// @param name The function name.
    /// @return A new MemberFunctionSpec.
    static MemberFunctionSpec Create(const std::string& name);
    /// @brief Creates a constructor builder.
    /// @return A new MemberFunctionSpec.
    static MemberFunctionSpec CreateConstructor();

    /// @brief Sets the return type.
    /// @param type The return type.
    /// @return This builder.
    MemberFunctionSpec& SetReturnType(const TypeName& type);
    /// @brief Sets the access level.
    /// @param access The access level.
    /// @return This builder.
    MemberFunctionSpec& SetAccessSpecifier(AccessSpecifier access);
    /// @brief Adds a function modifier.
    /// @param modifier The modifier to add.
    /// @return This builder.
    MemberFunctionSpec& AddModifier(Modifier modifier);
    /// @brief Adds a parameter.
    /// @param param The parameter to add.
    /// @return This builder.
    MemberFunctionSpec& AddParameter(const ParameterSpec& param);
    /// @brief Appends a code block to the body.
    /// @param code The block to append.
    /// @return This builder.
    MemberFunctionSpec& AddCode(const CodeBlock& code);
    /// @brief Appends a statement to the body.
    /// @param format A format string with placeholder arguments.
    /// @param args Values for the placeholders.
    /// @return This builder.
    MemberFunctionSpec& AddStatement(const std::string& format,
                                     std::vector<CodeBlockArg> args = {});
    /// @brief Adds a constructor initializer.
    /// @param member The member to initialize.
    /// @param expression The initializer expression.
    /// @return This builder.
    MemberFunctionSpec& AddInitializer(const std::string& member,
                                       const std::string& expression);

    /// @brief Returns whether this spec is a constructor.
    /// @return True if this spec describes a constructor.
    bool IsConstructor() const;
    /// @brief Returns whether the function is pure virtual.
    /// @return True if the function ends in = 0.
    bool IsPureVirtual() const;
    /// @brief Returns the function name.
    /// @return The function name.
    const std::string& Name() const;
    /// @brief Returns the access level.
    /// @return The access level.
    AccessSpecifier GetAccessSpecifier() const;

private:
    friend class detail::Renderer;

    std::string name_;
    bool is_constructor_ = false;
    TypeName return_type_;
    AccessSpecifier access_ = AccessSpecifier::kPublic;
    std::vector<Modifier> modifiers_;
    std::vector<ParameterSpec> parameters_;
    CodeBlock code_;
    std::vector<std::pair<std::string, std::string>> initializers_;
};

}  // namespace cpppoet
