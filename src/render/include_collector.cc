// Copyright 2026 wuzting
// SPDX-License-Identifier: MIT

/// @file include_collector.cc
/// @brief Implements IncludeCollector.

#include "include_collector.h"

#include "cpppoet/type_name.h"

namespace cpppoet {
namespace detail {

void IncludeCollector::Add(const TypeName& type) {
    if (!type.SystemHeader().empty()) {
        system_includes_.insert(type.SystemHeader());
    }
    if (!type.LocalHeader().empty()) {
        local_includes_.insert(type.LocalHeader());
    }
    for (const auto& arg : type.TemplateArgs()) {
        Add(arg);
    }
}

const std::set<std::string>& IncludeCollector::SystemIncludes() const {
    return system_includes_;
}

const std::set<std::string>& IncludeCollector::LocalIncludes() const {
    return local_includes_;
}

}  // namespace detail
}  // namespace cpppoet
