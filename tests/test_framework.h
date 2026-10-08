// Copyright 2026 wuzting
// SPDX-License-Identifier: MIT

/// @file test_framework.h
/// @brief A minimal self-registering test framework.

#pragma once

// Tests self-register via a static Registrar, so tests/ must be built as a
// single executable; otherwise the linker drops unreferenced translation units.
// Declare cases with CPPPOET_TEST(name) { ... } and use the EXPECT_* macros.

#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace cpppoet_test {

using TestFn = void (*)();

struct TestCase {
    std::string name;
    TestFn fn;
};

inline std::vector<TestCase>& Registry() {
    static std::vector<TestCase> cases;
    return cases;
}

struct Registrar {
    Registrar(const char* name, TestFn fn) { Registry().push_back({name, fn}); }
};

inline int& FailCount() {
    static int count = 0;
    return count;
}

inline int& CheckCount() {
    static int count = 0;
    return count;
}

inline void Fail(const char* file, int line, const std::string& message) {
    ++FailCount();
    std::cout << "    FAIL " << file << ":" << line << "\n      " << message
              << "\n";
}

inline std::string ToDebug(const std::string& value) {
    return "\"" + value + "\"";
}

inline std::string ToDebug(const char* value) {
    return std::string("\"") + (value ? value : "(null)") + "\"";
}

inline std::string ToDebug(bool value) { return value ? "true" : "false"; }

template <typename T>
inline std::string ToDebug(const T& value) {
    std::ostringstream os;
    os << value;
    return os.str();
}

inline int RunAll() {
    int failed_tests = 0;
    for (const auto& test : Registry()) {
        const int before = FailCount();
        std::cout << "[ RUN  ] " << test.name << "\n";
        try {
            test.fn();
        } catch (const std::exception& e) {
            Fail(__FILE__, __LINE__,
                 std::string("uncaught exception: ") + e.what());
        } catch (...) {
            Fail(__FILE__, __LINE__, "uncaught unknown exception");
        }
        if (FailCount() == before) {
            std::cout << "[  OK  ] " << test.name << "\n";
        } else {
            std::cout << "[ FAIL ] " << test.name << "\n";
            ++failed_tests;
        }
    }

    const int total = static_cast<int>(Registry().size());
    std::cout << "\n"
              << (total - failed_tests) << "/" << total << " tests passed, "
              << CheckCount() << " checks, " << FailCount() << " failures\n";
    return failed_tests == 0 ? 0 : 1;
}

}  // namespace cpppoet_test

#define CPPPOET_TEST(NAME)                                             \
    static void NAME();                                                \
    static ::cpppoet_test::Registrar cpppoet_reg_##NAME(#NAME, &NAME); \
    static void NAME()

#define EXPECT_TRUE(EXPR)                                          \
    do {                                                           \
        ++::cpppoet_test::CheckCount();                            \
        if (!(EXPR)) {                                             \
            ::cpppoet_test::Fail(__FILE__, __LINE__,               \
                                 "EXPECT_TRUE(" #EXPR ") failed"); \
        }                                                          \
    } while (0)

#define EXPECT_FALSE(EXPR)                                          \
    do {                                                            \
        ++::cpppoet_test::CheckCount();                             \
        if ((EXPR)) {                                               \
            ::cpppoet_test::Fail(__FILE__, __LINE__,                \
                                 "EXPECT_FALSE(" #EXPR ") failed"); \
        }                                                           \
    } while (0)

#define EXPECT_EQ(EXPECTED, ACTUAL)                                    \
    do {                                                               \
        ++::cpppoet_test::CheckCount();                                \
        const auto& cpppoet_expected_ = (EXPECTED);                    \
        const auto& cpppoet_actual_ = (ACTUAL);                        \
        if (!(cpppoet_expected_ == cpppoet_actual_)) {                 \
            ::cpppoet_test::Fail(                                      \
                __FILE__, __LINE__,                                    \
                std::string("EXPECT_EQ(" #EXPECTED ", " #ACTUAL ")") + \
                    "\n        expected: " +                           \
                    ::cpppoet_test::ToDebug(cpppoet_expected_) +       \
                    "\n        actual:   " +                           \
                    ::cpppoet_test::ToDebug(cpppoet_actual_));         \
        }                                                              \
    } while (0)

#define EXPECT_NE(A, B)                                               \
    do {                                                              \
        ++::cpppoet_test::CheckCount();                               \
        const auto& cpppoet_a_ = (A);                                 \
        const auto& cpppoet_b_ = (B);                                 \
        if (cpppoet_a_ == cpppoet_b_) {                               \
            ::cpppoet_test::Fail(__FILE__, __LINE__,                  \
                                 "EXPECT_NE(" #A ", " #B ") failed"); \
        }                                                             \
    } while (0)

#define TEST_FAIL(MESSAGE) ::cpppoet_test::Fail(__FILE__, __LINE__, (MESSAGE))
