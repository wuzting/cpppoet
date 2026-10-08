// Copyright 2026 wuzting
// SPDX-License-Identifier: MIT

/// @file type_name.h
/// @brief Declares TypeName, a type expression with optional include headers.

#pragma once

#include <string>
#include <vector>

#include "cpppoet/export.h"

namespace cpppoet {

class CPPPOET_EXPORT TypeName {
public:
    TypeName() = default;

    /// @brief Creates a plain type name.
    /// @param name The type name.
    /// @return The new type.
    static TypeName Of(const std::string& name);

    /// @brief Creates a template type.
    /// @param name The template name.
    /// @param template_args The template arguments.
    /// @return The new type.
    static TypeName Parameterized(const std::string& name,
                                  std::vector<TypeName> template_args);

    /// @brief Associates a system include.
    /// @param header The header name.
    /// @return This type.
    TypeName& WithSystemHeader(const std::string& header);
    /// @brief Associates a local include.
    /// @param header The header name.
    /// @return This type.
    TypeName& WithLocalHeader(const std::string& header);

    /// @brief Returns a const-qualified copy.
    /// @return A const copy of this type.
    TypeName AsConst() const;
    /// @brief Returns a reference copy.
    /// @return A reference copy of this type.
    TypeName AsRef() const;
    /// @brief Returns a pointer copy.
    /// @return A pointer copy of this type.
    TypeName AsPtr() const;

    /// @brief Renders this type as C++ source text.
    /// @return The rendered type.
    std::string ToString() const;

    /// @brief Returns whether the type is empty.
    /// @return True if no name is set.
    bool Empty() const;
    /// @brief Returns the unqualified type name.
    /// @return The type name.
    const std::string& Name() const;
    /// @brief Returns the template arguments.
    /// @return The template arguments.
    const std::vector<TypeName>& TemplateArgs() const;
    /// @brief Returns the associated system header.
    /// @return The header name, or an empty string.
    const std::string& SystemHeader() const;
    /// @brief Returns the associated local header.
    /// @return The header name, or an empty string.
    const std::string& LocalHeader() const;

    friend bool operator==(const TypeName& a, const TypeName& b) {
        return a.ToString() == b.ToString();
    }

    friend bool operator!=(const TypeName& a, const TypeName& b) {
        return !(a == b);
    }

private:
    std::string name_;
    std::vector<TypeName> template_args_;
    std::string system_header_;
    std::string local_header_;
    bool is_const_ = false;
    bool is_ref_ = false;
    bool is_ptr_ = false;
};

}  // namespace cpppoet
