// Copyright 2026 wuzting
// SPDX-License-Identifier: MIT

/// @file enum_spec.h
/// @brief Declares EnumSpec, a fluent builder for enum declarations.

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

/// @brief A generated enum declaration.
///
/// Usable as a top-level FileSpec member or nested inside a ClassSpec.
class CPPPOET_EXPORT EnumSpec {
public:
    /// @brief Whether an enum is scoped (enum class) or unscoped.
    enum class Scope {
        kScoped,    ///< enum class
        kUnscoped,  ///< enum
    };

    /// @brief Creates a scoped enum builder.
    /// @param name The enum name.
    /// @return A new EnumSpec.
    static EnumSpec Create(const std::string& name);

    /// @brief Sets whether the enum is scoped.
    /// @param scope kScoped for enum class, kUnscoped for enum.
    /// @return This builder.
    EnumSpec& SetScope(Scope scope);
    /// @brief Sets the underlying type.
    /// @param type The underlying integer type.
    /// @return This builder.
    EnumSpec& SetUnderlyingType(const TypeName& type);
    /// @brief Adds a constant without an explicit value.
    /// @param name The enumerator name.
    /// @return This builder.
    EnumSpec& AddConstant(const std::string& name);
    /// @brief Adds a constant with an explicit value.
    /// @param name The enumerator name.
    /// @param value The enumerator value.
    /// @return This builder.
    EnumSpec& AddConstant(const std::string& name, const std::string& value);
    /// @brief Sets the access level used when nested.
    /// @param access The access level.
    /// @return This builder.
    EnumSpec& SetAccessSpecifier(AccessSpecifier access);

    /// @brief Returns the enum name.
    /// @return The enum name.
    const std::string& Name() const;
    /// @brief Returns the access level used when nested.
    /// @return The access level.
    AccessSpecifier GetAccessSpecifier() const;

private:
    friend class detail::Renderer;

    struct Constant {
        std::string name;
        std::string value;
        bool has_value = false;
    };

    std::string name_;
    Scope scope_ = Scope::kScoped;
    TypeName underlying_type_;
    std::vector<Constant> constants_;
    AccessSpecifier access_ = AccessSpecifier::kPrivate;
};

}  // namespace cpppoet
