// Copyright 2026 wuzting
// SPDX-License-Identifier: MIT

/// @file member_function_spec.cc
/// @brief Implements the MemberFunctionSpec builder.

#include "cpppoet/member_function_spec.h"

#include "cpppoet/types.h"

namespace cpppoet {
using std::string;
using std::vector;

MemberFunctionSpec MemberFunctionSpec::Create(const string& name) {
    MemberFunctionSpec spec;
    spec.name_ = name;
    return spec;
}

MemberFunctionSpec MemberFunctionSpec::CreateConstructor() {
    MemberFunctionSpec spec;
    spec.is_constructor_ = true;
    return spec;
}

MemberFunctionSpec& MemberFunctionSpec::SetReturnType(const TypeName& type) {
    return_type_ = type;
    return *this;
}

MemberFunctionSpec& MemberFunctionSpec::SetAccessSpecifier(
    AccessSpecifier access) {
    access_ = access;
    return *this;
}

MemberFunctionSpec& MemberFunctionSpec::AddModifier(Modifier modifier) {
    modifiers_.push_back(modifier);
    return *this;
}

MemberFunctionSpec& MemberFunctionSpec::AddParameter(
    const ParameterSpec& param) {
    parameters_.push_back(param);
    return *this;
}

MemberFunctionSpec& MemberFunctionSpec::AddCode(const CodeBlock& code) {
    CodeBlock::Builder builder = CodeBlock::NewBuilder();
    builder.Add(code_);
    builder.Add(code);
    code_ = builder.Build();
    return *this;
}

MemberFunctionSpec& MemberFunctionSpec::AddStatement(
    const string& format, vector<CodeBlockArg> args) {
    CodeBlock::Builder builder = CodeBlock::NewBuilder();
    builder.Add(code_);
    builder.AddStatement(format, std::move(args));
    code_ = builder.Build();
    return *this;
}

MemberFunctionSpec& MemberFunctionSpec::AddInitializer(
    const string& member, const string& expression) {
    initializers_.emplace_back(member, expression);
    return *this;
}

bool MemberFunctionSpec::IsConstructor() const { return is_constructor_; }

bool MemberFunctionSpec::IsPureVirtual() const {
    for (Modifier m : modifiers_) {
        if (m == Modifier::kPureVirtual) {
            return true;
        }
    }
    return false;
}

const string& MemberFunctionSpec::Name() const { return name_; }

AccessSpecifier MemberFunctionSpec::GetAccessSpecifier() const {
    return access_;
}

}  // namespace cpppoet
