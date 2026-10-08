// Copyright 2026 wuzting
// SPDX-License-Identifier: MIT

/// @file parameter_spec.h
/// @brief Declares ParameterSpec, a fluent builder for function parameters.

#pragma once

#include <string>

#include "cpppoet/export.h"
#include "type_name.h"

namespace cpppoet {

namespace detail {
class Renderer;
}

class CPPPOET_EXPORT ParameterSpec {
public:
    /// @brief How a parameter is passed.
    enum class PassType {
        kByValue,      ///< T
        kByRef,        ///< T&
        kByPtr,        ///< T*
        kByRvalueRef,  ///< T&&
    };

    /// @brief Creates a parameter.
    /// @param type The parameter type.
    /// @param name The parameter name.
    /// @return A new ParameterSpec.
    static ParameterSpec Create(const TypeName& type, const std::string& name);

    /// @brief Sets whether the parameter is const.
    /// @param is_const True to emit const.
    /// @return This builder.
    ParameterSpec& SetConst(bool is_const);
    /// @brief Sets how the parameter is passed.
    /// @param pass The pass type.
    /// @return This builder.
    ParameterSpec& SetPassType(PassType pass);
    /// @brief Sets the default value.
    /// @param value The default, emitted in the declaration only.
    /// @return This builder.
    ParameterSpec& SetDefaultValue(const std::string& value);

    /// @brief Returns the parameter type.
    /// @return The parameter type.
    const TypeName& Type() const;
    /// @brief Returns the parameter name.
    /// @return The parameter name.
    const std::string& Name() const;

private:
    friend class detail::Renderer;

    TypeName type_;
    std::string name_;
    std::string default_value_;
    bool is_const_ = false;
    PassType pass_type_ = PassType::kByValue;
};

}  // namespace cpppoet
