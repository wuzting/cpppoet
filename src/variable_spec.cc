// Copyright 2026 wuzting
// SPDX-License-Identifier: MIT

/// @file variable_spec.cc
/// @brief Implements the VariableSpec builder.

#include "cpppoet/variable_spec.h"

namespace cpppoet {
using std::string;

VariableSpec VariableSpec::Create(const TypeName& type, const string& name) {
    VariableSpec spec;
    spec.type_ = type;
    spec.name_ = name;
    return spec;
}

VariableSpec& VariableSpec::AddModifier(Modifier modifier) {
    modifiers_.push_back(modifier);
    return *this;
}

VariableSpec& VariableSpec::SetInitializer(const string& initializer) {
    initializer_ = initializer;
    return *this;
}

const TypeName& VariableSpec::Type() const { return type_; }

const string& VariableSpec::Name() const { return name_; }

}  // namespace cpppoet
