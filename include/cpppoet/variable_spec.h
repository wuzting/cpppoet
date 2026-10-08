// Copyright 2026 wuzting
// SPDX-License-Identifier: MIT

/// @file variable_spec.h
/// @brief Declares VariableSpec, a fluent builder for namespace-scope
/// variables.

#pragma once

#include <string>
#include <vector>

#include "cpppoet/export.h"
#include "type_name.h"
#include "types.h"

namespace cpppoet {

namespace detail {
class Renderer;
}

/// @brief A generated namespace-scope variable.
///
/// Usable as a top-level FileSpec member. Unlike DataMemberSpec it has no
/// access specifier; use AddModifier with kExtern, kStatic, kThreadLocal,
/// kInline, kConstexpr or kConst.
class CPPPOET_EXPORT VariableSpec {
public:
    /// @brief Creates a variable.
    /// @param type The variable type.
    /// @param name The variable name.
    /// @return A new VariableSpec.
    static VariableSpec Create(const TypeName& type, const std::string& name);

    /// @brief Adds a modifier such as extern, static or constexpr.
    /// @param modifier The modifier to add.
    /// @return This builder.
    VariableSpec& AddModifier(Modifier modifier);
    /// @brief Sets the initializer expression.
    /// @param initializer The initializer emitted after the = sign.
    /// @return This builder.
    VariableSpec& SetInitializer(const std::string& initializer);

    /// @brief Returns the variable type.
    /// @return The variable type.
    const TypeName& Type() const;
    /// @brief Returns the variable name.
    /// @return The variable name.
    const std::string& Name() const;

private:
    friend class detail::Renderer;

    TypeName type_;
    std::string name_;
    std::vector<Modifier> modifiers_;
    std::string initializer_;
};

}  // namespace cpppoet
