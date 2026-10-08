// Copyright 2026 wuzting
// SPDX-License-Identifier: MIT

/// @file data_member_spec.h
/// @brief Declares DataMemberSpec, a fluent builder for data member
/// declarations.

#pragma once

#include <string>

#include "code_block.h"
#include "cpppoet/export.h"
#include "type_name.h"
#include "types.h"

namespace cpppoet {

namespace detail {
class Renderer;
}

class CPPPOET_EXPORT DataMemberSpec {
public:
    /// @brief Creates a data member.
    /// @param type The member type.
    /// @param name The member name.
    /// @return A new DataMemberSpec.
    static DataMemberSpec Create(const TypeName& type, const std::string& name);

    /// @brief Sets the access level.
    /// @param access The access level.
    /// @return This builder.
    DataMemberSpec& SetAccessSpecifier(AccessSpecifier access);
    /// @brief Adds a data member modifier.
    /// @param modifier The modifier, such as kStatic or kConstexpr.
    /// @return This builder.
    DataMemberSpec& AddModifier(Modifier modifier);
    /// @brief Sets the initializer expression.
    /// @param initializer The initializer emitted after the = sign.
    /// @return This builder.
    DataMemberSpec& SetInitializer(const std::string& initializer);

    /// @brief Returns the access level.
    /// @return The access level.
    AccessSpecifier GetAccessSpecifier() const;
    /// @brief Returns the member name.
    /// @return The member name.
    const std::string& Name() const;

private:
    friend class detail::Renderer;

    TypeName type_;
    std::string name_;
    AccessSpecifier access_ = AccessSpecifier::kPrivate;
    std::vector<Modifier> modifiers_;
    std::string initializer_;
};

}  // namespace cpppoet
