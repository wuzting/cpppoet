// Copyright 2026 wuzting
// SPDX-License-Identifier: MIT

/// @file type_alias_spec.cc
/// @brief Implements the TypeAliasSpec builder.

#include "cpppoet/type_alias_spec.h"

namespace cpppoet {
using std::string;

TypeAliasSpec TypeAliasSpec::Create(const string& name) {
    TypeAliasSpec spec;
    spec.name_ = name;
    return spec;
}

TypeAliasSpec& TypeAliasSpec::SetStyle(Style style) {
    style_ = style;
    return *this;
}

TypeAliasSpec& TypeAliasSpec::SetTarget(const TypeName& target) {
    target_ = target;
    return *this;
}

TypeAliasSpec& TypeAliasSpec::SetAccessSpecifier(AccessSpecifier access) {
    access_ = access;
    return *this;
}

TypeAliasSpec& TypeAliasSpec::AddTemplateParameter(const string& param) {
    template_params_.push_back(param);
    return *this;
}

const string& TypeAliasSpec::Name() const { return name_; }

AccessSpecifier TypeAliasSpec::GetAccessSpecifier() const { return access_; }

}  // namespace cpppoet
