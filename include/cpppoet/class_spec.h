// Copyright 2026 wuzting
// SPDX-License-Identifier: MIT

/// @file class_spec.h
/// @brief Declares ClassSpec, a fluent builder for class and struct
/// declarations.

#pragma once

#include <string>
#include <vector>

#include "cpppoet/export.h"
#include "data_member_spec.h"
#include "enum_spec.h"
#include "member_function_spec.h"
#include "type_alias_spec.h"
#include "type_name.h"
#include "types.h"

namespace cpppoet {

namespace detail {
class Renderer;
}

class CPPPOET_EXPORT ClassSpec {
public:
    /// @brief Whether to emit a class or a struct.
    enum class Kind {
        kClass,   ///< class
        kStruct,  ///< struct
    };

    /// @brief Creates a builder for a class or struct.
    /// @param kind Whether to emit a class or a struct.
    /// @param name The type name.
    /// @return A new ClassSpec.
    static ClassSpec Create(Kind kind, const std::string& name);

    /// @brief Sets the public base class.
    /// @param base The base type.
    /// @return This builder.
    ClassSpec& SetSuperclass(const TypeName& base);
    /// @brief Adds a public base interface.
    /// @param interface The interface type.
    /// @return This builder.
    ClassSpec& AddInterface(const TypeName& interface);
    /// @brief Adds a template parameter.
    /// @param param The parameter name.
    /// @return This builder.
    ClassSpec& AddTemplateParameter(const std::string& param);
    /// @brief Adds a data member.
    /// @param data_member The data member to add.
    /// @return This builder.
    ClassSpec& AddDataMember(const DataMemberSpec& data_member);
    /// @brief Adds a member function.
    /// @param member_function The member function to add.
    /// @return This builder.
    ClassSpec& AddMemberFunction(const MemberFunctionSpec& member_function);
    /// @brief Adds a nested enum.
    /// @param nested The enum to add.
    /// @return This builder.
    ClassSpec& AddEnum(const EnumSpec& nested);
    /// @brief Adds a nested type alias.
    /// @param alias The alias to add.
    /// @return This builder.
    ClassSpec& AddAlias(const TypeAliasSpec& alias);
    /// @brief Adds a nested class or struct.
    /// @param nested The nested type.
    /// @return This builder.
    ClassSpec& AddClass(const ClassSpec& nested);

    /// @brief Returns the class name.
    /// @return The class name.
    const std::string& Name() const;
    /// @brief Returns the class kind.
    /// @return Whether this type is a class or a struct.
    Kind GetKind() const;

    /// @brief Sets the access level used when nested.
    /// @param access The access level.
    /// @return This builder.
    ClassSpec& SetAccessSpecifier(AccessSpecifier access);
    /// @brief Returns the access level used when nested.
    /// @return The access level.
    AccessSpecifier GetAccessSpecifier() const;

private:
    friend class detail::Renderer;

    Kind kind_;
    std::string name_;
    TypeName superclass_;
    std::vector<TypeName> interfaces_;
    std::vector<std::string> template_params_;
    std::vector<DataMemberSpec> data_members_;
    std::vector<MemberFunctionSpec> member_functions_;
    std::vector<ClassSpec> classes_;
    std::vector<EnumSpec> enums_;
    std::vector<TypeAliasSpec> aliases_;
    AccessSpecifier access_ = AccessSpecifier::kPrivate;
};

}  // namespace cpppoet
