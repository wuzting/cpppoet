// Copyright 2026 wuzting
// SPDX-License-Identifier: MIT

/// @file file_spec.h
/// @brief Declares FileSpec, the top-level translation-unit builder.

#pragma once

#include <string>
#include <vector>

#include "class_spec.h"
#include "cpppoet/export.h"
#include "enum_spec.h"
#include "forward_decl_spec.h"
#include "free_function_spec.h"
#include "type_alias_spec.h"
#include "variable_spec.h"

namespace cpppoet {

namespace detail {
class Renderer;
}

class CPPPOET_EXPORT FileSpec {
public:
    /// @brief Where a top-level alias is placed.
    enum class Placement {
        kFront,  ///< before the types
        kBack,   ///< after the types
    };
    /// @brief Creates a translation-unit builder.
    /// @param name The file name, used for the generated files and guard.
    /// @param namespaces The enclosing namespaces, outermost first.
    /// @return A new FileSpec.
    static FileSpec Create(const std::string& name,
                           const std::vector<std::string>& namespaces);

    /// @brief Adds a top-level enum.
    /// @param enum_spec The enum to add.
    /// @return This builder.
    FileSpec& AddEnum(const EnumSpec& enum_spec);
    /// @brief Adds a top-level alias.
    /// @param placement Whether to emit the alias before or after the types.
    /// @param alias The alias to add.
    /// @return This builder.
    FileSpec& AddAlias(Placement placement, const TypeAliasSpec& alias);
    /// @brief Adds a top-level class or struct.
    /// @param type The type to add.
    /// @return This builder.
    FileSpec& AddClass(const ClassSpec& type);
    /// @brief Adds a top-level variable.
    /// @param variable The variable to add.
    /// @return This builder.
    FileSpec& AddVariable(const VariableSpec& variable);
    /// @brief Adds a top-level free function.
    /// @param free_function The free function to add.
    /// @return This builder.
    FileSpec& AddFreeFunction(const FreeFunctionSpec& free_function);
    /// @brief Appends enclosing namespaces.
    /// @param namespaces The namespaces to append.
    /// @return This builder.
    FileSpec& AddNamespaces(const std::vector<std::string>& namespaces);
    /// @brief Adds a forward declaration.
    /// @param decl The declaration to add.
    /// @return This builder.
    FileSpec& AddForwardDeclaration(const ForwardDeclSpec& decl);

    /// @brief Returns the file name.
    /// @return The file name without extension.
    const std::string& Name() const;
    /// @brief Returns the enclosing namespaces.
    /// @return The namespaces, outermost first.
    const std::vector<std::string>& Namespaces() const;

private:
    friend class detail::Renderer;

    std::string name_;
    std::vector<std::string> namespaces_;
    std::vector<ForwardDeclSpec> forward_declarations_;
    std::vector<EnumSpec> enums_;
    std::vector<TypeAliasSpec> front_aliases_;
    std::vector<TypeAliasSpec> back_aliases_;
    std::vector<ClassSpec> classes_;
    std::vector<VariableSpec> variables_;
    std::vector<FreeFunctionSpec> free_functions_;
};

}  // namespace cpppoet
