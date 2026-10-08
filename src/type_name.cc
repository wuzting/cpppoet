// Copyright 2026 wuzting
// SPDX-License-Identifier: MIT

/// @file type_name.cc
/// @brief Implements TypeName.

#include "cpppoet/type_name.h"

namespace cpppoet {
using std::string;
using std::vector;

TypeName TypeName::Of(const string& name) {
    TypeName result;
    result.name_ = name;
    return result;
}

TypeName TypeName::Parameterized(const string& name,
                                 vector<TypeName> template_args) {
    TypeName result;
    result.name_ = name;
    result.template_args_ = std::move(template_args);
    return result;
}

TypeName& TypeName::WithSystemHeader(const string& header) {
    system_header_ = header;
    return *this;
}

TypeName& TypeName::WithLocalHeader(const string& header) {
    local_header_ = header;
    return *this;
}

TypeName TypeName::AsConst() const {
    TypeName result = *this;
    result.is_const_ = true;
    return result;
}

TypeName TypeName::AsRef() const {
    TypeName result = *this;
    result.is_ref_ = true;
    return result;
}

TypeName TypeName::AsPtr() const {
    TypeName result = *this;
    result.is_ptr_ = true;
    return result;
}

string TypeName::ToString() const {
    if (Empty()) {
        return "";
    }
    string result;
    if (is_const_) {
        result += "const ";
    }
    result += name_;
    if (!template_args_.empty()) {
        result += "<";
        for (size_t i = 0; i < template_args_.size(); ++i) {
            if (i > 0) {
                result += ", ";
            }
            result += template_args_[i].ToString();
        }
        result += ">";
    }
    if (is_ref_) {
        result += "&";
    }
    if (is_ptr_) {
        result += "*";
    }
    return result;
}

bool TypeName::Empty() const { return name_.empty(); }

const string& TypeName::Name() const { return name_; }

const vector<TypeName>& TypeName::TemplateArgs() const {
    return template_args_;
}

const string& TypeName::SystemHeader() const { return system_header_; }

const string& TypeName::LocalHeader() const { return local_header_; }

}  // namespace cpppoet
