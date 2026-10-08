// Copyright 2026 wuzting
// SPDX-License-Identifier: MIT

/// @file type_alias_spec.h
/// @brief Declares TypeAliasSpec, a fluent builder for typedef and using
/// aliases.

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

/// @brief A generated type alias.
///
/// Expressed uniformly as typedef (C style) or using (C++11); usable as a
/// top-level FileSpec member or nested inside a ClassSpec.
class CPPPOET_EXPORT TypeAliasSpec {
public:
    /// @brief The alias syntax.
    enum class Style {
        kUsing,    ///< using name = target;
        kTypedef,  ///< typedef target name;
    };

    /// @brief Creates a type alias builder.
    /// @param name The alias name.
    /// @return A new TypeAliasSpec.
    static TypeAliasSpec Create(const std::string& name);

    /// @brief Chooses using or typedef.
    /// @param style The alias syntax.
    /// @return This builder.
    TypeAliasSpec& SetStyle(Style style);
    /// @brief Sets the aliased type.
    /// @param target The aliased type.
    /// @return This builder.
    TypeAliasSpec& SetTarget(const TypeName& target);
    /// @brief Sets the access level used when nested.
    /// @param access The access level.
    /// @return This builder.
    TypeAliasSpec& SetAccessSpecifier(AccessSpecifier access);
    /// @brief Adds a template parameter.
    /// @param param The parameter name.
    /// @return This builder.
    TypeAliasSpec& AddTemplateParameter(const std::string& param);

    /// @brief Returns the alias name.
    /// @return The alias name.
    const std::string& Name() const;
    /// @brief Returns the access level used when nested.
    /// @return The access level.
    AccessSpecifier GetAccessSpecifier() const;

private:
    friend class detail::Renderer;

    std::string name_;
    TypeName target_;
    Style style_ = Style::kUsing;
    AccessSpecifier access_ = AccessSpecifier::kPrivate;
    std::vector<std::string> template_params_;
};

}  // namespace cpppoet
