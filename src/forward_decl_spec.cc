// Copyright 2026 wuzting
// SPDX-License-Identifier: MIT

/// @file forward_decl_spec.cc
/// @brief Implements the ForwardDeclSpec builder.

#include "cpppoet/forward_decl_spec.h"

namespace cpppoet {
using std::string;

ForwardDeclSpec ForwardDeclSpec::Create(const string& name, Key key) {
    ForwardDeclSpec spec;
    spec.name_ = name;
    spec.key_ = key;
    return spec;
}

ForwardDeclSpec& ForwardDeclSpec::AddTemplateParameter(const string& param) {
    template_params_.push_back(param);
    return *this;
}

const string& ForwardDeclSpec::Name() const { return name_; }

ForwardDeclSpec::Key ForwardDeclSpec::GetKey() const { return key_; }

}  // namespace cpppoet
