// Copyright 2026 wuzting
// SPDX-License-Identifier: MIT

/// @file util.h
/// @brief Internal string helpers for the code emitters.

#pragma once

#include <string>

namespace cpppoet {

/// @brief Escapes a string literal for the $S placeholder.
/// @param value The raw string.
/// @return The quoted and escaped literal.
std::string StringLiteral(const std::string& value);

}  // namespace cpppoet
