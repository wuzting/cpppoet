// Copyright 2026 wuzting
// SPDX-License-Identifier: MIT

/// @file parameter_spec.cc
/// @brief Implements the ParameterSpec builder.

#include "cpppoet/parameter_spec.h"

namespace cpppoet {
using std::string;

ParameterSpec ParameterSpec::Create(const TypeName& type, const string& name) {
    ParameterSpec spec;
    spec.type_ = type;
    spec.name_ = name;
    return spec;
}

ParameterSpec& ParameterSpec::SetConst(bool is_const) {
    is_const_ = is_const;
    return *this;
}

ParameterSpec& ParameterSpec::SetPassType(PassType pass) {
    pass_type_ = pass;
    return *this;
}

ParameterSpec& ParameterSpec::SetDefaultValue(const string& value) {
    default_value_ = value;
    return *this;
}

const TypeName& ParameterSpec::Type() const { return type_; }

const string& ParameterSpec::Name() const { return name_; }

}  // namespace cpppoet
