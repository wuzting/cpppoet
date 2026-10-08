// Copyright 2026 wuzting
// SPDX-License-Identifier: MIT

/// @file class_spec.cc
/// @brief Implements the ClassSpec builder.

#include "cpppoet/class_spec.h"

namespace cpppoet {
using std::string;

ClassSpec ClassSpec::Create(Kind kind, const string& name) {
    ClassSpec spec;
    spec.kind_ = kind;
    spec.name_ = name;
    return spec;
}

ClassSpec& ClassSpec::SetSuperclass(const TypeName& base) {
    superclass_ = base;
    return *this;
}

ClassSpec& ClassSpec::AddInterface(const TypeName& interface) {
    interfaces_.push_back(interface);
    return *this;
}

ClassSpec& ClassSpec::AddTemplateParameter(const string& param) {
    template_params_.push_back(param);
    return *this;
}

ClassSpec& ClassSpec::AddDataMember(const DataMemberSpec& data_member) {
    data_members_.push_back(data_member);
    return *this;
}

ClassSpec& ClassSpec::AddMemberFunction(
    const MemberFunctionSpec& member_function) {
    member_functions_.push_back(member_function);
    return *this;
}

ClassSpec& ClassSpec::AddEnum(const EnumSpec& nested) {
    enums_.push_back(nested);
    return *this;
}

ClassSpec& ClassSpec::AddAlias(const TypeAliasSpec& alias) {
    aliases_.push_back(alias);
    return *this;
}

ClassSpec& ClassSpec::AddClass(const ClassSpec& nested) {
    classes_.push_back(nested);
    return *this;
}

const string& ClassSpec::Name() const { return name_; }

ClassSpec::Kind ClassSpec::GetKind() const { return kind_; }

AccessSpecifier ClassSpec::GetAccessSpecifier() const { return access_; }

ClassSpec& ClassSpec::SetAccessSpecifier(AccessSpecifier access) {
    access_ = access;
    return *this;
}

}  // namespace cpppoet
