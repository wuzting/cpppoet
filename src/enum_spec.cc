// Copyright 2026 wuzting
// SPDX-License-Identifier: MIT

/// @file enum_spec.cc
/// @brief Implements the EnumSpec builder.

#include "cpppoet/enum_spec.h"

namespace cpppoet {
using std::string;

EnumSpec EnumSpec::Create(const string& name) {
    EnumSpec spec;
    spec.name_ = name;
    return spec;
}

EnumSpec& EnumSpec::SetScope(Scope scope) {
    scope_ = scope;
    return *this;
}

EnumSpec& EnumSpec::SetUnderlyingType(const TypeName& type) {
    underlying_type_ = type;
    return *this;
}

EnumSpec& EnumSpec::AddConstant(const string& name) {
    Constant c;
    c.name = name;
    c.has_value = false;
    constants_.push_back(c);
    return *this;
}

EnumSpec& EnumSpec::AddConstant(const string& name, const string& value) {
    Constant c;
    c.name = name;
    c.value = value;
    c.has_value = true;
    constants_.push_back(c);
    return *this;
}

EnumSpec& EnumSpec::SetAccessSpecifier(AccessSpecifier access) {
    access_ = access;
    return *this;
}

const string& EnumSpec::Name() const { return name_; }

AccessSpecifier EnumSpec::GetAccessSpecifier() const { return access_; }

}  // namespace cpppoet
