// Copyright 2026 wuzting
// SPDX-License-Identifier: MIT

/// @file helpers.h
/// @brief Shared test helpers for building and rendering specs.

#pragma once

#include <cpppoet/cpppoet.h>

#include <string>

#include "render/renderer.h"

namespace th {

inline cpppoet::TypeName SysType(const char* name, const char* header) {
    return cpppoet::TypeName::Of(name).WithSystemHeader(header);
}

inline std::string Render(const cpppoet::DataMemberSpec& data_member) {
    cpppoet::detail::Renderer renderer{cpppoet::EmitOptions{}};
    renderer.Render(data_member);
    return renderer.ToString();
}

inline std::string Render(const cpppoet::VariableSpec& variable) {
    cpppoet::detail::Renderer renderer{cpppoet::EmitOptions{}};
    renderer.Render(variable);
    return renderer.ToString();
}

inline std::string Render(const cpppoet::ParameterSpec& param,
                          bool with_default) {
    cpppoet::detail::Renderer renderer{cpppoet::EmitOptions{}};
    renderer.Render(param, with_default);
    return renderer.ToString();
}

inline std::string Render(const cpppoet::ForwardDeclSpec& decl) {
    cpppoet::detail::Renderer renderer{cpppoet::EmitOptions{}};
    renderer.Render(decl);
    return renderer.ToString();
}

inline std::string RenderHeader(const cpppoet::EnumSpec& spec) {
    cpppoet::detail::Renderer renderer{cpppoet::EmitOptions{}};
    renderer.RenderHeader(spec);
    return renderer.ToString();
}

inline std::string RenderHeader(const cpppoet::ClassSpec& spec) {
    cpppoet::detail::Renderer renderer{cpppoet::EmitOptions{}};
    renderer.RenderHeader(spec);
    return renderer.ToString();
}

inline std::string RenderHeader(const cpppoet::TypeAliasSpec& spec) {
    cpppoet::detail::Renderer renderer{cpppoet::EmitOptions{}};
    renderer.RenderHeader(spec);
    return renderer.ToString();
}

inline std::string RenderDecl(
    const cpppoet::MemberFunctionSpec& member_function,
    const std::string& class_name) {
    cpppoet::detail::Renderer renderer{cpppoet::EmitOptions{}};
    renderer.RenderDeclaration(member_function, class_name);
    return renderer.ToString();
}

inline std::string RenderDef(const cpppoet::MemberFunctionSpec& member_function,
                             const std::string& recursive_name,
                             const std::string& class_name) {
    cpppoet::detail::Renderer renderer{cpppoet::EmitOptions{}};
    renderer.RenderDefinition(member_function, recursive_name, class_name,
                              false);
    return renderer.ToString();
}

inline std::string RenderDecl(const cpppoet::FreeFunctionSpec& free_function) {
    cpppoet::detail::Renderer renderer{cpppoet::EmitOptions{}};
    renderer.RenderDeclaration(free_function);
    return renderer.ToString();
}

inline std::string RenderDef(const cpppoet::FreeFunctionSpec& free_function,
                             bool as_inline = false) {
    cpppoet::detail::Renderer renderer{cpppoet::EmitOptions{}};
    renderer.RenderDefinition(free_function, as_inline);
    return renderer.ToString();
}

}  // namespace th
