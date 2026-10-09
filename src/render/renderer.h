// Copyright 2026 wuzting
// SPDX-License-Identifier: MIT

/// @file renderer.h
/// @brief Declares the internal Renderer that turns specs into source text.

#pragma once

#include <string>
#include <vector>

#include "../code_buffer.h"
#include "cpppoet/class_spec.h"
#include "cpppoet/code_block.h"
#include "cpppoet/data_member_spec.h"
#include "cpppoet/enum_spec.h"
#include "cpppoet/file_spec.h"
#include "cpppoet/forward_decl_spec.h"
#include "cpppoet/free_function_spec.h"
#include "cpppoet/member_function_spec.h"
#include "cpppoet/parameter_spec.h"
#include "cpppoet/type_alias_spec.h"
#include "cpppoet/type_name.h"
#include "cpppoet/types.h"
#include "cpppoet/variable_spec.h"
#include "include_collector.h"

namespace cpppoet {
namespace detail {

/// @brief Renders the model specs into text.
///
/// The renderer owns every piece of formatting logic. It is an internal
/// implementation detail: it is not installed and its symbols are not
/// exported.
class Renderer {
public:
    /// @brief Creates a renderer.
    /// @param options The emission options.
    explicit Renderer(const EmitOptions& options)
        : options_(options), out_(options.indent) {}

    /// @brief Renders a type name and records its include headers.
    void Render(const TypeName& type);

    /// @brief Renders a data member declaration.
    void Render(const DataMemberSpec& spec);
    /// @brief Renders a namespace-scope variable declaration.
    void Render(const VariableSpec& spec);
    /// @brief Renders a function parameter.
    /// @param with_default Whether to emit the default value.
    void Render(const ParameterSpec& spec, bool with_default);
    /// @brief Renders a forward declaration.
    void Render(const ForwardDeclSpec& spec);
    /// @brief Renders a placeholder code block.
    void Render(const CodeBlock& block);

    /// @brief Renders an enum definition.
    void RenderHeader(const EnumSpec& spec);
    /// @brief Renders a class or struct definition.
    void RenderHeader(const ClassSpec& spec);
    /// @brief Renders a type alias declaration.
    void RenderHeader(const TypeAliasSpec& spec);

    /// @brief Renders the out-of-line definitions of a class.
    /// @param as_inline Whether to mark definitions inline.
    /// @param parent_name The enclosing type names, for nested classes.
    void RenderSource(const ClassSpec& spec, bool as_inline,
                      const std::string& parent_name);

    /// @brief Renders an in-class member function declaration.
    void RenderDeclaration(const MemberFunctionSpec& spec,
                           const std::string& class_name);
    /// @brief Renders an out-of-line member function definition.
    void RenderDefinition(const MemberFunctionSpec& spec,
                          const std::string& recursive_name,
                          const std::string& class_name, bool as_inline);

    /// @brief Renders a free function declaration.
    void RenderDeclaration(const FreeFunctionSpec& spec);
    /// @brief Renders a free function definition.
    void RenderDefinition(const FreeFunctionSpec& spec, bool as_inline);

    /// @brief Renders the header body of a file spec.
    void RenderHeader(const FileSpec& spec);
    /// @brief Renders the source body of a file spec.
    void RenderSource(const FileSpec& spec, bool as_inline);
    /// @brief Renders the opening namespaces.
    void RenderNamespaceBegin(const FileSpec& spec);
    /// @brief Renders the closing namespaces.
    void RenderNamespaceEnd(const FileSpec& spec);
    /// @brief Renders the include guard opening.
    void RenderPragmaOnceBegin(const FileSpec& spec);
    /// @brief Renders the include guard closing.
    void RenderPragmaOnceEnd(const FileSpec& spec);
    /// @brief Renders the source file's include of its own header.
    void RenderSourceInclude(const FileSpec& spec);

    /// @brief Renders a complete header file.
    void RenderHeaderFile(const FileSpec& spec);
    /// @brief Renders a complete source file.
    /// @param as_inline Whether to mark out-of-line definitions inline.
    void RenderSourceFile(const FileSpec& spec, bool as_inline);
    /// @brief Renders a header file that also embeds its definitions inline.
    void RenderSelfContainedHeader(const FileSpec& spec);
    /// @brief Renders a source file that also embeds its declarations.
    void RenderSelfContainedSource(const FileSpec& spec);

    /// @brief Returns the accumulated output.
    /// @return The rendered text.
    std::string ToString() const { return out_.ToString(); }

private:
    void RenderIncludes(const IncludeCollector& includes);
    void RenderMembers(const ClassSpec& spec);
    void RenderMembersForAccess(const ClassSpec& spec, AccessSpecifier access,
                                bool& emitted_any);
    void RenderAccessSpecifier(const ClassSpec& spec, bool& emitted,
                               bool& emitted_any, AccessSpecifier access);
    std::string AccessToString(AccessSpecifier access) const;

    void RenderMemberFunctionModifiers(const MemberFunctionSpec& spec);
    void RenderMemberFunctionSignature(const MemberFunctionSpec& spec,
                                       bool with_default,
                                       const std::string& class_name);
    void RenderMemberFunctionBody(const MemberFunctionSpec& spec);

    void RenderFreeFunctionModifiers(const FreeFunctionSpec& spec);
    void RenderFreeFunctionSignature(const FreeFunctionSpec& spec,
                                     bool with_default);
    void RenderFreeFunctionBody(const FreeFunctionSpec& spec);

    EmitOptions options_;
    CodeBuffer out_;
    IncludeCollector includes_;
};

}  // namespace detail
}  // namespace cpppoet
