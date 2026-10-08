// Copyright 2026 wuzting
// SPDX-License-Identifier: MIT

/// @file file_spec.cc
/// @brief Implements the FileSpec builder.

#include "cpppoet/file_spec.h"

namespace cpppoet {
using std::string;
using std::vector;

FileSpec FileSpec::Create(const string& name,
                          const vector<string>& namespaces) {
    FileSpec spec;
    spec.name_ = name;
    spec.namespaces_ = namespaces;
    return spec;
}

FileSpec& FileSpec::AddNamespaces(const vector<string>& namespaces) {
    namespaces_.insert(namespaces_.end(), namespaces.begin(), namespaces.end());
    return *this;
}

FileSpec& FileSpec::AddForwardDeclaration(const ForwardDeclSpec& decl) {
    forward_declarations_.push_back(decl);
    return *this;
}

FileSpec& FileSpec::AddEnum(const EnumSpec& enum_spec) {
    enums_.push_back(enum_spec);
    return *this;
}

FileSpec& FileSpec::AddAlias(Placement placement, const TypeAliasSpec& alias) {
    switch (placement) {
        case Placement::kFront:
            front_aliases_.push_back(alias);
            break;
        case Placement::kBack:
            back_aliases_.push_back(alias);
            break;
    }

    return *this;
}

FileSpec& FileSpec::AddClass(const ClassSpec& type) {
    classes_.push_back(type);
    return *this;
}

FileSpec& FileSpec::AddVariable(const VariableSpec& variable) {
    variables_.push_back(variable);
    return *this;
}

FileSpec& FileSpec::AddFreeFunction(const FreeFunctionSpec& free_function) {
    free_functions_.push_back(free_function);
    return *this;
}

const string& FileSpec::Name() const { return name_; }

const vector<string>& FileSpec::Namespaces() const { return namespaces_; }

}  // namespace cpppoet
