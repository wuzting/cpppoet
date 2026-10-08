// Copyright 2026 wuzting
// SPDX-License-Identifier: MIT

/// @file declarations_test.cc
/// @brief Tests for enum, data member, variable, parameter, alias and
/// forward-declaration emission.

#include "helpers.h"
#include "test_framework.h"

using namespace cpppoet;

CPPPOET_TEST(Enum_scoped_with_underlying_type) {
    EnumSpec spec = EnumSpec::Create("Mode")
                        .SetScope(EnumSpec::Scope::kScoped)
                        .SetUnderlyingType(th::SysType("uint8_t", "cstdint"))
                        .AddConstant("kIdle", "0")
                        .AddConstant("kRun", "1");
    EXPECT_EQ(std::string("enum class Mode : uint8_t {\n"
                          "    kIdle = 0,\n"
                          "    kRun = 1\n"
                          "};\n"),
              th::RenderHeader(spec));
}

CPPPOET_TEST(Enum_unscoped_optional_values) {
    EnumSpec spec = EnumSpec::Create("Level")
                        .SetScope(EnumSpec::Scope::kUnscoped)
                        .AddConstant("kLow")
                        .AddConstant("kHigh", "9");
    EXPECT_EQ(std::string("enum Level {\n"
                          "    kLow,\n"
                          "    kHigh = 9\n"
                          "};\n"),
              th::RenderHeader(spec));
}

CPPPOET_TEST(DataMember_modifiers_and_initializer) {
    DataMemberSpec data_member =
        DataMemberSpec::Create(TypeName::Of("uint32_t"), "count")
            .AddModifier(Modifier::kStatic)
            .AddModifier(Modifier::kConstexpr)
            .SetInitializer("3");
    EXPECT_EQ(std::string("static constexpr uint32_t count = 3;"),
              th::Render(data_member));
}

CPPPOET_TEST(DataMember_without_initializer) {
    DataMemberSpec data_member =
        DataMemberSpec::Create(TypeName::Of("int"), "x");
    EXPECT_EQ(std::string("int x;"), th::Render(data_member));
}

CPPPOET_TEST(Variable_modifiers_and_initializer) {
    VariableSpec variable = VariableSpec::Create(TypeName::Of("int"), "kMax")
                                .AddModifier(Modifier::kExtern)
                                .AddModifier(Modifier::kConst);
    EXPECT_EQ(std::string("extern const int kMax;"), th::Render(variable));
}

CPPPOET_TEST(Variable_thread_local_with_initializer) {
    VariableSpec variable = VariableSpec::Create(TypeName::Of("int"), "counter")
                                .AddModifier(Modifier::kThreadLocal)
                                .SetInitializer("0");
    EXPECT_EQ(std::string("thread_local int counter = 0;"),
              th::Render(variable));
}

CPPPOET_TEST(Parameter_pass_styles) {
    EXPECT_EQ(std::string("const Config& cfg"),
              th::Render(ParameterSpec::Create(TypeName::Of("Config"), "cfg")
                             .SetConst(true)
                             .SetPassType(ParameterSpec::PassType::kByRef),
                         false));
    EXPECT_EQ(std::string("const uint8_t* data"),
              th::Render(ParameterSpec::Create(
                             th::SysType("uint8_t", "cstdint"), "data")
                             .SetConst(true)
                             .SetPassType(ParameterSpec::PassType::kByPtr),
                         false));
    EXPECT_EQ(
        std::string("std::string&& other"),
        th::Render(ParameterSpec::Create(TypeName::Of("std::string"), "other")
                       .SetPassType(ParameterSpec::PassType::kByRvalueRef),
                   false));
}

CPPPOET_TEST(Parameter_default_only_in_declaration) {
    ParameterSpec param =
        ParameterSpec::Create(TypeName::Of("int"), "lo").SetDefaultValue("0");
    EXPECT_EQ(std::string("int lo = 0"), th::Render(param, true));
    EXPECT_EQ(std::string("int lo"), th::Render(param, false));
}

CPPPOET_TEST(Alias_using_and_typedef) {
    EXPECT_EQ(std::string("using SensorId = uint32_t;\n"),
              th::RenderHeader(TypeAliasSpec::Create("SensorId")
                                   .SetTarget(TypeName::Of("uint32_t"))));
    EXPECT_EQ(std::string("typedef int LegacyId;\n"),
              th::RenderHeader(TypeAliasSpec::Create("LegacyId")
                                   .SetStyle(TypeAliasSpec::Style::kTypedef)
                                   .SetTarget(TypeName::Of("int"))));
}

CPPPOET_TEST(Alias_template_using) {
    TypeAliasSpec alias =
        TypeAliasSpec::Create("RefCount")
            .AddTemplateParameter("T")
            .SetTarget(
                TypeName::Parameterized("std::shared_ptr", {TypeName::Of("T")})
                    .WithSystemHeader("memory"));
    EXPECT_EQ(std::string("template <typename T>\nusing RefCount = "
                          "std::shared_ptr<T>;\n"),
              th::RenderHeader(alias));
}

CPPPOET_TEST(Alias_typedef_template_rewritten_to_using) {
    TypeAliasSpec alias = TypeAliasSpec::Create("Vec")
                              .SetStyle(TypeAliasSpec::Style::kTypedef)
                              .AddTemplateParameter("T")
                              .SetTarget(TypeName::Parameterized(
                                             "std::vector", {TypeName::Of("T")})
                                             .WithSystemHeader("vector"));
    EXPECT_EQ(std::string("template <typename T>\n"
                          "// typedef cannot express a template alias; "
                          "rewritten as using\n"
                          "using Vec = std::vector<T>;\n"),
              th::RenderHeader(alias));
}

CPPPOET_TEST(Forward_declarations) {
    EXPECT_EQ(std::string("struct Connection;\n"),
              th::Render(ForwardDeclSpec::Create("Connection")));
    EXPECT_EQ(std::string("class Writer;\n"),
              th::Render(ForwardDeclSpec::Create(
                  "Writer", ForwardDeclSpec::Key::kClass)));
    EXPECT_EQ(std::string("template <typename T>\nstruct Buffer;\n"),
              th::Render(
                  ForwardDeclSpec::Create("Buffer").AddTemplateParameter("T")));
}
