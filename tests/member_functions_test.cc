// Copyright 2026 wuzting
// SPDX-License-Identifier: MIT

/// @file member_functions_test.cc
/// @brief Tests for MemberFunctionSpec and FreeFunctionSpec emission.

#include "helpers.h"
#include "test_framework.h"

using namespace cpppoet;

CPPPOET_TEST(MemberFunction_const_accessor_declaration) {
    MemberFunctionSpec member_function = MemberFunctionSpec::Create("count");
    member_function.SetReturnType(th::SysType("size_t", "cstddef"))
        .SetAccessSpecifier(AccessSpecifier::kPublic)
        .AddModifier(Modifier::kConst);
    EXPECT_EQ(std::string("size_t count() const;\n"),
              th::RenderDecl(member_function, "Sample"));
}

CPPPOET_TEST(MemberFunction_defaulted_constructor) {
    MemberFunctionSpec ctor = MemberFunctionSpec::CreateConstructor();
    ctor.SetAccessSpecifier(AccessSpecifier::kPublic)
        .AddModifier(Modifier::kDefaulted);
    EXPECT_EQ(std::string("Sample() = default;\n"),
              th::RenderDecl(ctor, "Sample"));
}

CPPPOET_TEST(MemberFunction_deleted_copy_constructor) {
    MemberFunctionSpec ctor = MemberFunctionSpec::CreateConstructor();
    ctor.SetAccessSpecifier(AccessSpecifier::kPublic)
        .AddParameter(ParameterSpec::Create(
            TypeName::Of("Sample").AsConst().AsRef(), "other"))
        .AddModifier(Modifier::kDeleted);
    EXPECT_EQ(std::string("Sample(const Sample& other) = delete;\n"),
              th::RenderDecl(ctor, "Sample"));
}

CPPPOET_TEST(MemberFunction_pure_virtual) {
    MemberFunctionSpec member_function = MemberFunctionSpec::Create("Run");
    member_function.SetReturnType(TypeName::Of("void"))
        .AddModifier(Modifier::kPureVirtual);
    EXPECT_EQ(std::string("virtual void Run() = 0;\n"),
              th::RenderDecl(member_function, "Base"));
}

CPPPOET_TEST(MemberFunction_definition_with_initializer_list) {
    MemberFunctionSpec ctor = MemberFunctionSpec::CreateConstructor();
    ctor.SetAccessSpecifier(AccessSpecifier::kPublic)
        .AddModifier(Modifier::kExplicit)
        .AddParameter(
            ParameterSpec::Create(th::SysType("size_t", "cstddef"), "count"))
        .AddInitializer("count_", "count");
    EXPECT_EQ(
        std::string("Sample::Sample(size_t count) : count_(count) {\n}\n"),
        th::RenderDef(ctor, "Sample", "Sample"));
}

CPPPOET_TEST(MemberFunction_add_statement_appends) {
    MemberFunctionSpec member_function = MemberFunctionSpec::Create("Run");
    member_function.SetReturnType(TypeName::Of("void"));
    member_function.AddStatement("int a = $L", {"1"});
    member_function.AddStatement("int b = $L", {"2"});
    const std::string def = th::RenderDef(member_function, "C", "C");
    EXPECT_TRUE(def.find("int a = 1;") != std::string::npos);
    EXPECT_TRUE(def.find("int b = 2;") != std::string::npos);
}

CPPPOET_TEST(FreeFunction_declaration_and_definition_defaults) {
    FreeFunctionSpec fn = FreeFunctionSpec::Create("clamp");
    fn.SetReturnType(TypeName::Of("int"))
        .AddParameter(ParameterSpec::Create(TypeName::Of("int"), "value"))
        .AddParameter(ParameterSpec::Create(TypeName::Of("int"), "lo")
                          .SetDefaultValue("0"));
    EXPECT_EQ(std::string("int clamp(int value, int lo = 0);\n"),
              th::RenderDecl(fn));
    EXPECT_EQ(std::string("int clamp(int value, int lo) {\n}\n"),
              th::RenderDef(fn));
}

CPPPOET_TEST(FreeFunction_inline_stays_in_header) {
    FreeFunctionSpec fn = FreeFunctionSpec::Create("sum");
    fn.SetReturnType(TypeName::Of("int"))
        .AddModifier(Modifier::kInline)
        .AddParameter(ParameterSpec::Create(TypeName::Of("int"), "a"));
    auto body = CodeBlock::NewBuilder();
    body.AddStatement("return a");
    fn.AddCode(body.Build());
    EXPECT_EQ(std::string("inline int sum(int a) {\n    return a;\n}\n"),
              th::RenderDecl(fn));
    EXPECT_EQ(std::string(""), th::RenderDef(fn));
}

CPPPOET_TEST(FreeFunction_add_statement_appends) {
    FreeFunctionSpec fn = FreeFunctionSpec::Create("Run");
    fn.SetReturnType(TypeName::Of("void"));
    fn.AddStatement("int a = $L", {"1"});
    fn.AddStatement("int b = $L", {"2"});
    const std::string def = th::RenderDef(fn);
    EXPECT_TRUE(def.find("int a = 1;") != std::string::npos);
    EXPECT_TRUE(def.find("int b = 2;") != std::string::npos);
}

CPPPOET_TEST(MemberFunction_add_code_appends) {
    MemberFunctionSpec member_function = MemberFunctionSpec::Create("Run");
    member_function.SetReturnType(TypeName::Of("void"));

    auto first = CodeBlock::NewBuilder();
    first.AddStatement("first()");
    member_function.AddCode(first.Build());

    auto second = CodeBlock::NewBuilder();
    second.AddStatement("second()");
    member_function.AddCode(second.Build());

    const std::string def = th::RenderDef(member_function, "C", "C");
    EXPECT_TRUE(def.find("first();") != std::string::npos);
    EXPECT_TRUE(def.find("second();") != std::string::npos);
}

CPPPOET_TEST(FreeFunction_add_code_appends) {
    FreeFunctionSpec fn = FreeFunctionSpec::Create("Run");
    fn.SetReturnType(TypeName::Of("void"));

    auto first = CodeBlock::NewBuilder();
    first.AddStatement("first()");
    fn.AddCode(first.Build());

    auto second = CodeBlock::NewBuilder();
    second.AddStatement("second()");
    fn.AddCode(second.Build());

    const std::string def = th::RenderDef(fn);
    EXPECT_TRUE(def.find("first();") != std::string::npos);
    EXPECT_TRUE(def.find("second();") != std::string::npos);
}
