// Copyright 2026 wuzting
// SPDX-License-Identifier: MIT

/// @file types.h
/// @brief Declares shared public enums and small value types.

#pragma once

#include <string>

#include "cpppoet/export.h"

namespace cpppoet {

/// @brief C++ member access control.
///
/// Distinct from functional modifiers; C++ emits these as public:, protected:
/// and private: sections.
enum class AccessSpecifier {
    kPublic,     ///< public:
    kProtected,  ///< protected:
    kPrivate,    ///< private:
};

/// @brief C++ functional modifiers.
///
/// Emission order follows declaration order.
enum class Modifier {
    kStatic,       ///< static
    kExtern,       ///< extern
    kThreadLocal,  ///< thread_local
    kVirtual,      ///< virtual
    kPureVirtual,  ///< = 0 (pure virtual, implies virtual)
    kConst,        ///< trailing const (member function)
    kConstexpr,    ///< constexpr
    kInline,       ///< inline
    kExplicit,     ///< explicit (constructor)
    kOverride,     ///< override
    kFinal,        ///< final (virtual function)
    kNoexcept,     ///< noexcept
    kDefaulted,    ///< = default
    kDeleted,      ///< = delete
    kFriend,       ///< friend
};

/// @brief Configuration for code emission.
struct CPPPOET_EXPORT EmitOptions {
    std::string indent = "    ";  ///< Indentation for one level.
    int column_limit = 80;        ///< Target column limit (reserved).
    bool use_pragma_once =
        true;  ///< Emit \#pragma once instead of an include guard.
    std::string header_ext = ".h";   ///< Extension for generated headers.
    std::string source_ext = ".cc";  ///< Extension for generated sources.
};

}  // namespace cpppoet
