// Copyright 2026 wuzting
// SPDX-License-Identifier: MIT

/// @file data_member_spec.cc
/// @brief Implements the DataMemberSpec builder.

#include "cpppoet/data_member_spec.h"

namespace cpppoet {
using std::string;

DataMemberSpec DataMemberSpec::Create(const TypeName& type,
                                      const string& name) {
    DataMemberSpec spec;
    spec.type_ = type;
    spec.name_ = name;
    return spec;
}

DataMemberSpec& DataMemberSpec::SetAccessSpecifier(AccessSpecifier access) {
    access_ = access;
    return *this;
}

DataMemberSpec& DataMemberSpec::AddModifier(Modifier modifier) {
    modifiers_.push_back(modifier);
    return *this;
}

DataMemberSpec& DataMemberSpec::SetInitializer(const string& initializer) {
    initializer_ = initializer;
    return *this;
}

AccessSpecifier DataMemberSpec::GetAccessSpecifier() const { return access_; }

const string& DataMemberSpec::Name() const { return name_; }

}  // namespace cpppoet
