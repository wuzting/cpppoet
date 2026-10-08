// Copyright 2026 wuzting
// SPDX-License-Identifier: MIT

/// @file include_collector.h
/// @brief Declares IncludeCollector, the internal registry of include headers.

#pragma once

#include <set>
#include <string>

namespace cpppoet {

class TypeName;

namespace detail {

/// @brief Collects the include headers declared on rendered types.
///
/// A rendering concern kept separate from the text buffer: the renderer records
/// every type it emits and later emits the deduplicated, sorted includes.
class IncludeCollector {
public:
    /// @brief Records the headers declared on a type and its template
    /// arguments.
    /// @param type The type whose headers should be recorded.
    void Add(const TypeName& type);

    /// @brief Returns the collected system headers.
    /// @return The system headers, sorted.
    const std::set<std::string>& SystemIncludes() const;
    /// @brief Returns the collected local headers.
    /// @return The local headers, sorted.
    const std::set<std::string>& LocalIncludes() const;

private:
    std::set<std::string> system_includes_;
    std::set<std::string> local_includes_;
};

}  // namespace detail
}  // namespace cpppoet
