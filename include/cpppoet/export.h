// Copyright 2026 wuzting
// SPDX-License-Identifier: MIT

/// @file export.h
/// @brief Export macro for the public cpppoet API.

#pragma once

// On Windows a shared library must mark each public symbol as dllexport while
// building and dllimport while consuming; CPPPOET_STATIC disables this for
// static builds. Other compilers rely on default symbol visibility, which is
// only meaningful because the library itself is compiled with hidden
// visibility.
#if defined(_WIN32) && !defined(CPPPOET_STATIC)
#ifdef CPPPOET_BUILDING_LIBRARY
#define CPPPOET_EXPORT __declspec(dllexport)
#else
#define CPPPOET_EXPORT __declspec(dllimport)
#endif
#elif defined(__GNUC__) || defined(__clang__)
#define CPPPOET_EXPORT __attribute__((visibility("default")))
#else
#define CPPPOET_EXPORT
#endif
