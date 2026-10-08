// Copyright 2026 wuzting
// SPDX-License-Identifier: MIT

/// @file free_function_spec.cc
/// @brief Implements the FreeFunctionSpec builder.

#include "cpppoet/free_function_spec.h"

#include <utility>

#include "cpppoet/types.h"

namespace cpppoet {
using std::string;
using std::vector;

FreeFunctionSpec FreeFunctionSpec::Create(const string& name) {
    FreeFunctionSpec spec;
    spec.name_ = name;
    return spec;
}

FreeFunctionSpec& FreeFunctionSpec::SetReturnType(const TypeName& type) {
    return_type_ = type;
    return *this;
}

FreeFunctionSpec& FreeFunctionSpec::AddModifier(Modifier modifier) {
    modifiers_.push_back(modifier);
    return *this;
}

FreeFunctionSpec& FreeFunctionSpec::AddParameter(const ParameterSpec& param) {
    parameters_.push_back(param);
    return *this;
}

FreeFunctionSpec& FreeFunctionSpec::AddCode(const CodeBlock& code) {
    CodeBlock::Builder builder = CodeBlock::NewBuilder();
    builder.Add(code_);
    builder.Add(code);
    code_ = builder.Build();
    return *this;
}

FreeFunctionSpec& FreeFunctionSpec::AddStatement(const string& format,
                                                 vector<CodeBlockArg> args) {
    CodeBlock::Builder builder = CodeBlock::NewBuilder();
    builder.Add(code_);
    builder.AddStatement(format, std::move(args));
    code_ = builder.Build();
    return *this;
}

const string& FreeFunctionSpec::Name() const { return name_; }

}  // namespace cpppoet
