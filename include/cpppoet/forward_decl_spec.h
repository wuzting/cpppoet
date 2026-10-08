// Copyright 2026 wuzting
// SPDX-License-Identifier: MIT

/// @file forward_decl_spec.h
/// @brief Declares ForwardDeclSpec, a forward declaration builder.

#pragma once

#include <string>
#include <vector>

#include "cpppoet/export.h"

namespace cpppoet {

namespace detail {
class Renderer;
}

class CPPPOET_EXPORT ForwardDeclSpec {
public:
    /// @brief The tag keyword used in the declaration.
    enum class Key {
        kStruct,  ///< struct
        kClass,   ///< class
    };

    /// @brief Creates a forward declaration.
    /// @param name The declared name.
    /// @param key Whether to use struct or class.
    /// @return A new ForwardDeclSpec.
    static ForwardDeclSpec Create(const std::string& name,
                                  Key key = Key::kStruct);

    /// @brief Adds a template parameter.
    /// @param param The parameter name.
    /// @return This builder.
    ForwardDeclSpec& AddTemplateParameter(const std::string& param);

    /// @brief Returns the declared name.
    /// @return The declared name.
    const std::string& Name() const;
    /// @brief Returns the tag keyword.
    /// @return Whether the declaration uses struct or class.
    Key GetKey() const;

private:
    friend class detail::Renderer;

    std::string name_;
    Key key_ = Key::kStruct;
    std::vector<std::string> template_params_;
};

}  // namespace cpppoet
