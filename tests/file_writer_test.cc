// Copyright 2026 wuzting
// SPDX-License-Identifier: MIT

/// @file file_writer_test.cc
/// @brief Tests for FileWriter output and include collection.

#include <cpppoet/file_writer.h>

#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>

#include "helpers.h"
#include "test_framework.h"

using namespace cpppoet;

namespace {

std::string ReadFile(const std::string& path) {
    std::ifstream in(path);
    return std::string(std::istreambuf_iterator<char>(in),
                       std::istreambuf_iterator<char>());
}

struct TempDir {
    std::filesystem::path path;

    TempDir() {
        path = std::filesystem::temp_directory_path() / "cpppoet_test_out";
        std::error_code ec;
        std::filesystem::remove_all(path, ec);
        std::filesystem::create_directories(path);
    }

    std::string File(const std::string& name) const {
        return (path / name).string();
    }
};

}  // namespace

CPPPOET_TEST(FileWriter_header_pragma_once_and_includes) {
    TempDir tmp;
    FileSpec file = FileSpec::Create("sample", {"a", "b"});
    file.AddEnum(EnumSpec::Create("E")
                     .SetUnderlyingType(th::SysType("uint8_t", "cstdint"))
                     .AddConstant("kA", "0"));

    FileWriter writer{EmitOptions{}};
    EXPECT_TRUE(writer.WriteHeaderAndSource(file, tmp.path.string(),
                                            tmp.path.string()));

    EXPECT_EQ(std::string("#pragma once\n"
                          "\n"
                          "#include <cstdint>\n"
                          "\n"
                          "namespace a {\n"
                          "namespace b {\n"
                          "\n"
                          "enum class E : uint8_t {\n"
                          "    kA = 0\n"
                          "};\n"
                          "\n"
                          "}  // namespace b\n"
                          "}  // namespace a\n"),
              ReadFile(tmp.File("sample.h")));
}

CPPPOET_TEST(FileWriter_include_guard_mode) {
    TempDir tmp;
    FileSpec file = FileSpec::Create("legacy", {"ns"});
    file.AddClass(ClassSpec::Create(ClassSpec::Kind::kStruct, "P"));

    EmitOptions options;
    options.use_pragma_once = false;
    FileWriter writer{options};
    EXPECT_TRUE(writer.WriteHeaderAndSource(file, tmp.path.string(),
                                            tmp.path.string()));

    const std::string header = ReadFile(tmp.File("legacy.h"));
    EXPECT_EQ(static_cast<size_t>(0),
              header.rfind("#ifndef legacy_H\n#define legacy_H\n", 0));
    EXPECT_TRUE(header.find("#endif // legacy_H\n") != std::string::npos);
}

CPPPOET_TEST(FileWriter_collects_local_and_system_includes) {
    TempDir tmp;
    FileSpec file = FileSpec::Create("inc", {"ns"});
    ClassSpec spec = ClassSpec::Create(ClassSpec::Kind::kClass, "C");
    spec.AddDataMember(
            DataMemberSpec::Create(th::SysType("uint8_t", "cstdint"), "a")
                .SetAccessSpecifier(AccessSpecifier::kPublic))
        .AddDataMember(
            DataMemberSpec::Create(
                TypeName::Of("Widget").WithLocalHeader("widget.h"), "w")
                .SetAccessSpecifier(AccessSpecifier::kPublic));
    file.AddClass(spec);

    FileWriter writer{EmitOptions{}};
    EXPECT_TRUE(writer.WriteHeader(file, tmp.path.string()));

    const std::string header = ReadFile(tmp.File("inc.h"));
    EXPECT_TRUE(header.find("#include <cstdint>\n") != std::string::npos);
    EXPECT_TRUE(header.find("#include \"widget.h\"\n") != std::string::npos);
}

CPPPOET_TEST(FileWriter_source_separates_declaration_and_definition) {
    TempDir tmp;
    FileSpec file = FileSpec::Create("funcs", {"ns"});

    FreeFunctionSpec fn = FreeFunctionSpec::Create("add");
    fn.SetReturnType(TypeName::Of("int"))
        .AddParameter(ParameterSpec::Create(TypeName::Of("int"), "a"))
        .AddParameter(ParameterSpec::Create(TypeName::Of("int"), "b"));
    auto body = CodeBlock::NewBuilder();
    body.AddStatement("return a + b");
    fn.AddCode(body.Build());
    file.AddFreeFunction(fn);

    FileWriter writer{EmitOptions{}};
    EXPECT_TRUE(writer.WriteHeaderAndSource(file, tmp.path.string(),
                                            tmp.path.string()));

    const std::string header = ReadFile(tmp.File("funcs.h"));
    const std::string source = ReadFile(tmp.File("funcs.cc"));
    EXPECT_TRUE(header.find("int add(int a, int b);") != std::string::npos);
    EXPECT_TRUE(source.find("#include \"funcs.h\"") != std::string::npos);
    EXPECT_TRUE(source.find("int add(int a, int b) {") != std::string::npos);
}

CPPPOET_TEST(FileWriter_source_file_golden) {
    TempDir tmp;
    FileSpec file = FileSpec::Create("funcs", {"ns"});

    FreeFunctionSpec fn = FreeFunctionSpec::Create("add");
    fn.SetReturnType(TypeName::Of("int"))
        .AddParameter(ParameterSpec::Create(TypeName::Of("int"), "a"))
        .AddParameter(ParameterSpec::Create(TypeName::Of("int"), "b"));
    auto body = CodeBlock::NewBuilder();
    body.AddStatement("return a + b");
    fn.AddCode(body.Build());
    file.AddFreeFunction(fn);

    FileWriter writer{EmitOptions{}};
    EXPECT_TRUE(writer.WriteHeaderAndSource(file, tmp.path.string(),
                                            tmp.path.string()));

    EXPECT_EQ(std::string("#include \"funcs.h\"\n"
                          "\n"
                          "namespace ns {\n"
                          "\n"
                          "int add(int a, int b) {\n"
                          "    return a + b;\n"
                          "}\n"
                          "}  // namespace ns\n"),
              ReadFile(tmp.File("funcs.cc")));
}
