// Copyright 2026 wuzting
// SPDX-License-Identifier: MIT

/// @file classes_test.cc
/// @brief Tests for ClassSpec emission.

#include "helpers.h"
#include "test_framework.h"

using namespace cpppoet;

CPPPOET_TEST(Class_public_and_private_sections) {
    ClassSpec spec = ClassSpec::Create(ClassSpec::Kind::kClass, "Foo");
    spec.SetAccessSpecifier(AccessSpecifier::kPublic);

    MemberFunctionSpec member_function = MemberFunctionSpec::Create("Bar");
    member_function.SetReturnType(TypeName::Of("void"))
        .SetAccessSpecifier(AccessSpecifier::kPublic);
    spec.AddMemberFunction(member_function);

    spec.AddDataMember(DataMemberSpec::Create(TypeName::Of("int"), "x_")
                           .SetAccessSpecifier(AccessSpecifier::kPrivate));

    EXPECT_EQ(std::string("class Foo {\n"
                          "public:\n"
                          "    void Bar();\n"
                          "private:\n"
                          "    int x_;\n"
                          "};\n"),
              th::RenderHeader(spec));
}

CPPPOET_TEST(Class_simple_struct_has_no_access_labels) {
    ClassSpec spec = ClassSpec::Create(ClassSpec::Kind::kStruct, "Point");
    spec.AddDataMember(DataMemberSpec::Create(TypeName::Of("int"), "x")
                           .SetAccessSpecifier(AccessSpecifier::kPublic))
        .AddDataMember(DataMemberSpec::Create(TypeName::Of("int"), "y")
                           .SetAccessSpecifier(AccessSpecifier::kPublic));
    EXPECT_EQ(std::string("struct Point {\n"
                          "    int x;\n"
                          "    int y;\n"
                          "};\n"),
              th::RenderHeader(spec));
}

CPPPOET_TEST(Class_struct_non_public_field_keeps_access_label) {
    ClassSpec spec = ClassSpec::Create(ClassSpec::Kind::kStruct, "S");
    spec.AddDataMember(DataMemberSpec::Create(TypeName::Of("int"), "a")
                           .SetAccessSpecifier(AccessSpecifier::kPublic))
        .AddDataMember(DataMemberSpec::Create(TypeName::Of("int"), "secret")
                           .SetAccessSpecifier(AccessSpecifier::kPrivate));
    EXPECT_EQ(std::string("struct S {\n"
                          "    int a;\n"
                          "private:\n"
                          "    int secret;\n"
                          "};\n"),
              th::RenderHeader(spec));
}

CPPPOET_TEST(Class_inheritance) {
    ClassSpec spec = ClassSpec::Create(ClassSpec::Kind::kClass, "Box");
    spec.SetSuperclass(TypeName::Of("Base"));
    EXPECT_EQ(std::string("class Box : public Base {\n};\n"),
              th::RenderHeader(spec));
}

CPPPOET_TEST(Class_template_parameter) {
    ClassSpec spec = ClassSpec::Create(ClassSpec::Kind::kClass, "Holder");
    spec.AddTemplateParameter("T");
    EXPECT_EQ(std::string("template <typename T>\nclass Holder {\n};\n"),
              th::RenderHeader(spec));
}

CPPPOET_TEST(Class_nested_enum_and_struct) {
    ClassSpec device = ClassSpec::Create(ClassSpec::Kind::kClass, "Device");
    device.SetAccessSpecifier(AccessSpecifier::kPublic);
    device.AddEnum(EnumSpec::Create("Mode")
                       .SetScope(EnumSpec::Scope::kScoped)
                       .SetAccessSpecifier(AccessSpecifier::kPublic)
                       .AddConstant("kOff", "0"));

    ClassSpec config = ClassSpec::Create(ClassSpec::Kind::kStruct, "Config");
    config.SetAccessSpecifier(AccessSpecifier::kPublic)
        .AddDataMember(DataMemberSpec::Create(TypeName::Of("int"), "v")
                           .SetAccessSpecifier(AccessSpecifier::kPublic));
    device.AddClass(config);

    EXPECT_EQ(std::string("class Device {\n"
                          "public:\n"
                          "    enum class Mode {\n"
                          "        kOff = 0\n"
                          "    };\n"
                          "\n"
                          "    struct Config {\n"
                          "        int v;\n"
                          "    };\n"
                          "\n"
                          "};\n"),
              th::RenderHeader(device));
}
