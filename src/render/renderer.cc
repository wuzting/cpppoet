// Copyright 2026 wuzting
// SPDX-License-Identifier: MIT

/// @file renderer.cc
/// @brief Implements the internal Renderer.

#include "renderer.h"

#include <cstddef>
#include <string>
#include <vector>

#include "../util.h"

namespace cpppoet {
namespace detail {

using std::string;
using std::vector;

namespace {

bool HasModifier(const vector<Modifier>& modifiers, Modifier modifier) {
    for (Modifier m : modifiers) {
        if (m == modifier) {
            return true;
        }
    }
    return false;
}

}  // namespace

void Renderer::Render(const TypeName& type) {
    if (type.Empty()) {
        return;
    }
    if (!type.SystemHeader().empty() || !type.LocalHeader().empty()) {
        includes_.Add(type);
    }
    out_.Emit(type.ToString());
}

void Renderer::Render(const DataMemberSpec& spec) {
    for (Modifier m : spec.modifiers_) {
        switch (m) {
            case Modifier::kStatic:
                out_.Emit("static ");
                break;
            case Modifier::kConstexpr:
                out_.Emit("constexpr ");
                break;
            default:
                break;
        }
    }
    Render(spec.type_);
    out_.Emit(" " + spec.name_);
    if (!spec.initializer_.empty()) {
        out_.Emit(" = " + spec.initializer_);
    }
    out_.Emit(";");
}

void Renderer::Render(const VariableSpec& spec) {
    for (Modifier m : spec.modifiers_) {
        switch (m) {
            case Modifier::kExtern:
                out_.Emit("extern ");
                break;
            case Modifier::kStatic:
                out_.Emit("static ");
                break;
            case Modifier::kThreadLocal:
                out_.Emit("thread_local ");
                break;
            case Modifier::kInline:
                out_.Emit("inline ");
                break;
            case Modifier::kConstexpr:
                out_.Emit("constexpr ");
                break;
            case Modifier::kConst:
                out_.Emit("const ");
                break;
            default:
                break;
        }
    }
    Render(spec.type_);
    out_.Emit(" " + spec.name_);
    if (!spec.initializer_.empty()) {
        out_.Emit(" = " + spec.initializer_);
    }
    out_.Emit(";");
}

void Renderer::Render(const ParameterSpec& spec, bool with_default) {
    if (spec.is_const_) {
        out_.Emit("const ");
    }
    Render(spec.type_);
    switch (spec.pass_type_) {
        case ParameterSpec::PassType::kByValue:
            break;
        case ParameterSpec::PassType::kByRef:
            out_.Emit("&");
            break;
        case ParameterSpec::PassType::kByPtr:
            out_.Emit("*");
            break;
        case ParameterSpec::PassType::kByRvalueRef:
            out_.Emit("&&");
            break;
    }
    out_.Emit(" " + spec.name_);
    if (with_default && !spec.default_value_.empty()) {
        out_.Emit(" = " + spec.default_value_);
    }
}

void Renderer::Render(const ForwardDeclSpec& spec) {
    if (!spec.template_params_.empty()) {
        out_.Emit("template <");
        for (size_t i = 0; i < spec.template_params_.size(); ++i) {
            if (i > 0) {
                out_.Emit(", ");
            }
            out_.Emit("typename " + spec.template_params_[i]);
        }
        out_.Emit(">\n");
    }

    out_.Emit(
        (spec.key_ == ForwardDeclSpec::Key::kStruct ? "struct " : "class ") +
        spec.name_ + ";\n");
}

void Renderer::Render(const CodeBlock& block) {
    size_t arg_index = 0;
    for (const auto& part : block.parts_) {
        if (part == "$L") {
            if (arg_index < block.args_.size()) {
                const auto& arg = block.args_[arg_index++];
                if (arg.GetKind() == CodeBlockArg::Kind::kType) {
                    Render(arg.Type());
                } else {
                    out_.Emit(arg.Text());
                }
            }
        } else if (part == "$N") {
            if (arg_index < block.args_.size()) {
                out_.Emit(block.args_[arg_index++].Text());
            }
        } else if (part == "$S") {
            if (arg_index < block.args_.size()) {
                out_.Emit(StringLiteral(block.args_[arg_index++].Text()));
            }
        } else if (part == "$T") {
            if (arg_index < block.args_.size()) {
                Render(block.args_[arg_index++].Type());
            }
        } else if (part == "$$") {
            out_.Emit("$");
        } else if (part == "$>") {
            out_.Indent();
        } else if (part == "$<") {
            out_.Unindent();
        } else {
            out_.Emit(part);
        }
    }
}

void Renderer::RenderHeader(const EnumSpec& spec) {
    out_.Emit(spec.scope_ == EnumSpec::Scope::kScoped ? "enum class "
                                                      : "enum ");
    out_.Emit(spec.name_);
    if (!spec.underlying_type_.Empty()) {
        out_.Emit(" : ");
        Render(spec.underlying_type_);
    }
    out_.Emit(" {\n");
    out_.Indent();
    for (size_t i = 0; i < spec.constants_.size(); ++i) {
        out_.Emit(spec.constants_[i].name);
        if (spec.constants_[i].has_value) {
            out_.Emit(" = " + spec.constants_[i].value);
        }
        if (i + 1 < spec.constants_.size()) {
            out_.Emit(",");
        }
        out_.Emit("\n");
    }
    out_.Unindent();
    out_.Emit("};\n");
}

void Renderer::RenderHeader(const ClassSpec& spec) {
    string keyword =
        (spec.kind_ == ClassSpec::Kind::kStruct) ? "struct" : "class";
    if (!spec.template_params_.empty()) {
        out_.Emit("template <");
        for (size_t i = 0; i < spec.template_params_.size(); ++i) {
            if (i > 0) {
                out_.Emit(", ");
            }
            out_.Emit("typename " + spec.template_params_[i]);
        }
        out_.Emit(">\n");
    }
    out_.Emit(keyword + " " + spec.name_);
    if (!spec.superclass_.Empty() || !spec.interfaces_.empty()) {
        out_.Emit(" : ");
        bool first = true;
        if (!spec.superclass_.Empty()) {
            out_.Emit("public ");
            Render(spec.superclass_);
            first = false;
        }
        for (const auto& iface : spec.interfaces_) {
            if (!first) {
                out_.Emit(", ");
            }
            out_.Emit("public ");
            Render(iface);
            first = false;
        }
    }
    out_.Emit(" {\n");
    out_.Indent();
    RenderMembers(spec);
    out_.Unindent();
    out_.Emit("};\n");
}

void Renderer::RenderHeader(const TypeAliasSpec& spec) {
    // typedef cannot express a template alias; rewrite as using
    bool use_using = (spec.style_ == TypeAliasSpec::Style::kUsing) ||
                     !spec.template_params_.empty();

    if (!spec.template_params_.empty()) {
        out_.Emit("template <");
        for (size_t i = 0; i < spec.template_params_.size(); ++i) {
            if (i > 0) {
                out_.Emit(", ");
            }
            out_.Emit("typename " + spec.template_params_[i]);
        }
        out_.Emit(">\n");
    }

    if (use_using) {
        if (spec.style_ == TypeAliasSpec::Style::kTypedef &&
            !spec.template_params_.empty()) {
            out_.Emit(
                "// typedef cannot express a template alias; rewritten as "
                "using\n");
        }
        out_.Emit("using " + spec.name_ + " = ");
        Render(spec.target_);
        out_.Emit(";\n");
    } else {
        out_.Emit("typedef ");
        Render(spec.target_);
        out_.Emit(" " + spec.name_ + ";\n");
    }
}

void Renderer::RenderSource(const ClassSpec& spec, bool as_inline,
                            const string& parent_name) {
    auto recursive_name =
        parent_name.empty() ? spec.name_ : parent_name + "::" + spec.name_;
    for (const auto& member_function : spec.member_functions_) {
        RenderDefinition(member_function, recursive_name, spec.name_,
                         as_inline);
        out_.Emit("\n");
    }

    for (const auto& nested : spec.classes_) {
        RenderSource(nested, as_inline, recursive_name);
    }
}

void Renderer::RenderDeclaration(const MemberFunctionSpec& spec,
                                 const string& class_name) {
    if (spec.IsPureVirtual()) {
        RenderMemberFunctionSignature(spec, true, class_name);
        out_.Emit(";\n");
        return;
    }
    if (HasModifier(spec.modifiers_, Modifier::kInline)) {
        RenderMemberFunctionSignature(spec, true, class_name);
        RenderMemberFunctionBody(spec);
        return;
    }
    if (HasModifier(spec.modifiers_, Modifier::kDefaulted) ||
        HasModifier(spec.modifiers_, Modifier::kDeleted)) {
        RenderMemberFunctionSignature(spec, true, class_name);
        if (HasModifier(spec.modifiers_, Modifier::kDefaulted)) {
            out_.Emit(" = default");
        } else {
            out_.Emit(" = delete");
        }
        out_.Emit(";\n");
        return;
    }
    RenderMemberFunctionSignature(spec, true, class_name);
    out_.Emit(";\n");
}

void Renderer::RenderDefinition(const MemberFunctionSpec& spec,
                                const string& recursive_name,
                                const string& class_name, bool as_inline) {
    if (spec.IsPureVirtual() ||
        HasModifier(spec.modifiers_, Modifier::kInline) ||
        HasModifier(spec.modifiers_, Modifier::kDefaulted) ||
        HasModifier(spec.modifiers_, Modifier::kDeleted)) {
        return;
    }

    if (as_inline) {
        out_.Emit("inline ");
    }

    if (!spec.is_constructor_) {
        Render(spec.return_type_);
        out_.Emit(" ");
    }

    out_.Emit(recursive_name +
              "::" + (spec.is_constructor_ ? class_name : spec.name_));
    for (size_t i = 0; i < spec.parameters_.size(); ++i) {
        if (i == 0) {
            out_.Emit("(");
        } else {
            out_.Emit(", ");
        }
        Render(spec.parameters_[i], false);
    }
    if (!spec.parameters_.empty()) {
        out_.Emit(")");
    } else {
        out_.Emit("()");
    }
    if (HasModifier(spec.modifiers_, Modifier::kConst)) {
        out_.Emit(" const");
    }
    if (HasModifier(spec.modifiers_, Modifier::kNoexcept)) {
        out_.Emit(" noexcept");
    }
    if (!spec.initializers_.empty()) {
        out_.Emit(" : ");
        for (size_t i = 0; i < spec.initializers_.size(); ++i) {
            if (i > 0) {
                out_.Emit(", ");
            }
            out_.Emit(spec.initializers_[i].first + "(" +
                      spec.initializers_[i].second + ")");
        }
    }
    out_.Emit(" {\n");
    out_.Indent();
    Render(spec.code_);
    out_.Unindent();
    out_.Emit("}\n");
}

void Renderer::RenderDeclaration(const FreeFunctionSpec& spec) {
    if (HasModifier(spec.modifiers_, Modifier::kInline)) {
        RenderFreeFunctionSignature(spec, true);
        RenderFreeFunctionBody(spec);
        return;
    }

    RenderFreeFunctionSignature(spec, true);
    out_.Emit(";\n");
}

void Renderer::RenderDefinition(const FreeFunctionSpec& spec, bool as_inline) {
    if (HasModifier(spec.modifiers_, Modifier::kInline)) {
        return;
    }
    if (as_inline) {
        out_.Emit("inline ");
    }
    RenderFreeFunctionSignature(spec, false);
    RenderFreeFunctionBody(spec);
}

void Renderer::RenderHeader(const FileSpec& spec) {
    for (const auto& d : spec.forward_declarations_) {
        Render(d);
        out_.Emit("\n");
    }

    for (const auto& e : spec.enums_) {
        RenderHeader(e);
        out_.Emit("\n");
    }

    for (const auto& a : spec.front_aliases_) {
        RenderHeader(a);
        out_.Emit("\n");
    }

    for (const auto& t : spec.classes_) {
        RenderHeader(t);
        out_.Emit("\n");
    }

    for (const auto& a : spec.back_aliases_) {
        RenderHeader(a);
        out_.Emit("\n");
    }

    for (const auto& v : spec.variables_) {
        Render(v);
        out_.Emit("\n");
    }

    for (const auto& f : spec.free_functions_) {
        RenderDeclaration(f);
        out_.Emit("\n");
    }
}

void Renderer::RenderSource(const FileSpec& spec, bool as_inline) {
    for (const auto& t : spec.classes_) {
        RenderSource(t, as_inline, "");
    }
    for (const auto& f : spec.free_functions_) {
        RenderDefinition(f, as_inline);
    }
}

void Renderer::RenderNamespaceBegin(const FileSpec& spec) {
    for (const auto& ns : spec.namespaces_) {
        out_.Emit("namespace " + ns + " {\n");
    }

    if (!spec.namespaces_.empty()) {
        out_.Emit("\n");
    }
}

void Renderer::RenderNamespaceEnd(const FileSpec& spec) {
    for (auto it = spec.namespaces_.rbegin(); it != spec.namespaces_.rend();
         ++it) {
        out_.Emit("}  // namespace " + *it + "\n");
    }
}

void Renderer::RenderPragmaOnceBegin(const FileSpec& spec) {
    if (options_.use_pragma_once) {
        out_.Emit("#pragma once\n\n");
        return;
    }

    out_.Emit("#ifndef " + spec.name_ + "_H\n");
    out_.Emit("#define " + spec.name_ + "_H\n\n");
}

void Renderer::RenderPragmaOnceEnd(const FileSpec& spec) {
    if (options_.use_pragma_once) {
        return;
    }

    out_.Emit("#endif // " + spec.name_ + "_H\n");
}

void Renderer::RenderIncludes(const IncludeCollector& includes) {
    for (const auto& inc : includes.SystemIncludes()) {
        out_.Emit("#include <" + inc + ">\n");
    }

    for (const auto& inc : includes.LocalIncludes()) {
        out_.Emit("#include \"" + inc + "\"\n");
    }

    if (!includes.SystemIncludes().empty() ||
        !includes.LocalIncludes().empty()) {
        out_.Emit("\n");
    }
}

void Renderer::RenderHeaderFile(const FileSpec& spec) {
    Renderer body(options_);
    body.RenderNamespaceBegin(spec);
    body.RenderHeader(spec);
    body.RenderNamespaceEnd(spec);

    RenderPragmaOnceBegin(spec);
    RenderIncludes(body.includes_);
    out_.Append(body.ToString());
    RenderPragmaOnceEnd(spec);
}

void Renderer::RenderSourceFile(const FileSpec& spec, bool as_inline) {
    Renderer body(options_);
    body.RenderNamespaceBegin(spec);
    body.RenderSource(spec, as_inline);
    body.RenderNamespaceEnd(spec);

    RenderSourceInclude(spec);
    RenderIncludes(body.includes_);
    out_.Append(body.ToString());
}

void Renderer::RenderSelfContainedHeader(const FileSpec& spec) {
    Renderer body(options_);
    body.RenderNamespaceBegin(spec);
    body.RenderHeader(spec);
    body.RenderSource(spec, true);
    body.RenderNamespaceEnd(spec);

    RenderPragmaOnceBegin(spec);
    RenderIncludes(body.includes_);
    out_.Append(body.ToString());
    RenderPragmaOnceEnd(spec);
}

void Renderer::RenderSelfContainedSource(const FileSpec& spec) {
    Renderer body(options_);
    body.RenderNamespaceBegin(spec);
    body.RenderHeader(spec);
    body.RenderSource(spec, false);
    body.RenderNamespaceEnd(spec);

    RenderIncludes(body.includes_);
    out_.Append(body.ToString());
}

void Renderer::RenderSourceInclude(const FileSpec& spec) {
    out_.Emit("#include \"" + spec.Name() + options_.header_ext + "\"\n\n");
}

void Renderer::RenderMembers(const ClassSpec& spec) {
    bool emitted_any = false;
    RenderMembersForAccess(spec, AccessSpecifier::kPublic, emitted_any);
    RenderMembersForAccess(spec, AccessSpecifier::kProtected, emitted_any);
    RenderMembersForAccess(spec, AccessSpecifier::kPrivate, emitted_any);
}

void Renderer::RenderAccessSpecifier(const ClassSpec& spec, bool& emitted,
                                     bool& emitted_any,
                                     AccessSpecifier access) {
    if (emitted) {
        return;
    }

    emitted = true;

    const bool implicit_section = spec.kind_ == ClassSpec::Kind::kStruct &&
                                  access == AccessSpecifier::kPublic &&
                                  !emitted_any;
    if (implicit_section) {
        return;
    }

    out_.Unindent();
    out_.Emit(AccessToString(access) + ":\n");
    out_.Indent();
    emitted_any = true;
}

void Renderer::RenderMembersForAccess(const ClassSpec& spec,
                                      AccessSpecifier access,
                                      bool& emitted_any) {
    bool emitted = false;
    for (const auto& nested : spec.enums_) {
        if (nested.GetAccessSpecifier() != access) {
            continue;
        }

        RenderAccessSpecifier(spec, emitted, emitted_any, access);
        RenderHeader(nested);
        out_.Emit("\n");
    }

    for (const auto& nested : spec.classes_) {
        if (nested.GetAccessSpecifier() != access) {
            continue;
        }

        RenderAccessSpecifier(spec, emitted, emitted_any, access);
        RenderHeader(nested);
        out_.Emit("\n");
    }

    for (const auto& alias : spec.aliases_) {
        if (alias.GetAccessSpecifier() != access) {
            continue;
        }

        RenderAccessSpecifier(spec, emitted, emitted_any, access);
        RenderHeader(alias);
        out_.Emit("\n");
    }

    for (const auto& member_function : spec.member_functions_) {
        if (member_function.GetAccessSpecifier() != access) {
            continue;
        }

        RenderAccessSpecifier(spec, emitted, emitted_any, access);
        RenderDeclaration(member_function, spec.name_);
    }

    for (const auto& data_member : spec.data_members_) {
        if (data_member.GetAccessSpecifier() != access) {
            continue;
        }

        RenderAccessSpecifier(spec, emitted, emitted_any, access);
        Render(data_member);
        out_.Emit("\n");
    }
}

string Renderer::AccessToString(AccessSpecifier access) const {
    switch (access) {
        case AccessSpecifier::kPublic:
            return "public";
        case AccessSpecifier::kProtected:
            return "protected";
        case AccessSpecifier::kPrivate:
            return "private";
    }
    return "private";
}

void Renderer::RenderMemberFunctionModifiers(const MemberFunctionSpec& spec) {
    string result;
    for (Modifier m : spec.modifiers_) {
        switch (m) {
            case Modifier::kStatic:
                result += "static ";
                break;
            case Modifier::kVirtual:
            case Modifier::kPureVirtual:
                result += "virtual ";
                break;
            case Modifier::kInline:
                result += "inline ";
                break;
            case Modifier::kExplicit:
                result += "explicit ";
                break;
            case Modifier::kFriend:
                result += "friend ";
                break;
            default:
                break;
        }
    }
    if (!result.empty()) {
        out_.Emit(result);
    }
}

void Renderer::RenderMemberFunctionSignature(const MemberFunctionSpec& spec,
                                             bool with_default,
                                             const string& class_name) {
    RenderMemberFunctionModifiers(spec);
    if (!spec.is_constructor_) {
        Render(spec.return_type_);
        out_.Emit(" ");
    }
    out_.Emit(spec.is_constructor_ ? class_name : spec.name_);
    for (size_t i = 0; i < spec.parameters_.size(); ++i) {
        if (i == 0) {
            out_.Emit("(");
        } else {
            out_.Emit(", ");
        }
        Render(spec.parameters_[i], with_default);
    }
    if (!spec.parameters_.empty()) {
        out_.Emit(")");
    } else {
        out_.Emit("()");
    }
    if (HasModifier(spec.modifiers_, Modifier::kConst)) {
        out_.Emit(" const");
    }
    if (HasModifier(spec.modifiers_, Modifier::kNoexcept)) {
        out_.Emit(" noexcept");
    }
    if (HasModifier(spec.modifiers_, Modifier::kOverride)) {
        out_.Emit(" override");
    }
    if (HasModifier(spec.modifiers_, Modifier::kFinal)) {
        out_.Emit(" final");
    }
    if (HasModifier(spec.modifiers_, Modifier::kPureVirtual)) {
        out_.Emit(" = 0");
    }
}

void Renderer::RenderMemberFunctionBody(const MemberFunctionSpec& spec) {
    out_.Emit(" {\n");
    out_.Indent();
    Render(spec.code_);
    out_.Unindent();
    out_.Emit("}\n");
}

void Renderer::RenderFreeFunctionModifiers(const FreeFunctionSpec& spec) {
    string result;
    for (Modifier m : spec.modifiers_) {
        switch (m) {
            case Modifier::kStatic:
                result += "static ";
                break;
            case Modifier::kInline:
                result += "inline ";
                break;
            default:
                break;
        }
    }
    if (!result.empty()) {
        out_.Emit(result);
    }
}

void Renderer::RenderFreeFunctionSignature(const FreeFunctionSpec& spec,
                                           bool with_default) {
    RenderFreeFunctionModifiers(spec);
    Render(spec.return_type_);
    out_.Emit(" ");
    out_.Emit(spec.name_);
    for (size_t i = 0; i < spec.parameters_.size(); ++i) {
        if (i == 0) {
            out_.Emit("(");
        } else {
            out_.Emit(", ");
        }
        Render(spec.parameters_[i], with_default);
    }
    if (!spec.parameters_.empty()) {
        out_.Emit(")");
    } else {
        out_.Emit("()");
    }
    if (HasModifier(spec.modifiers_, Modifier::kNoexcept)) {
        out_.Emit(" noexcept");
    }
}

void Renderer::RenderFreeFunctionBody(const FreeFunctionSpec& spec) {
    out_.Emit(" {\n");
    out_.Indent();
    Render(spec.code_);
    out_.Unindent();
    out_.Emit("}\n");
}

}  // namespace detail
}  // namespace cpppoet
