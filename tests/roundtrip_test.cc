// Copyright 2026 wuzting
// SPDX-License-Identifier: MIT

/// @file roundtrip_test.cc
/// @brief Compiles the generated code with the host compiler.

#include <cpppoet/file_writer.h>

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>

#include "helpers.h"
#include "test_framework.h"

#if defined(_WIN32)

// The round-trip test shells out to a POSIX-style compiler command line
// (`-std=c++17 -I... -o...`), which MSVC does not accept. It is skipped on
// Windows; the rest of the suite still runs there.
CPPPOET_TEST(Roundtrip_generated_code_compiles) {
    std::cout << "    SKIP: round-trip test is not supported on Windows\n";
}

#else

using namespace cpppoet;

#ifndef CPPPOET_TEST_CXX_COMPILER
#define CPPPOET_TEST_CXX_COMPILER "c++"
#endif

#ifndef CPPPOET_TEST_INCLUDE_DIR
#define CPPPOET_TEST_INCLUDE_DIR "include"
#endif

namespace {

std::string ReadFile(const std::string& path) {
    std::ifstream in(path);
    return std::string(std::istreambuf_iterator<char>(in),
                       std::istreambuf_iterator<char>());
}

std::string Quote(const std::string& value) { return "\"" + value + "\""; }

int Run(const std::string& command) { return std::system(command.c_str()); }

ClassSpec BuildConfigStruct() {
    ClassSpec spec = ClassSpec::Create(ClassSpec::Kind::kStruct, "Config");
    spec.SetAccessSpecifier(AccessSpecifier::kPublic)
        .AddDataMember(DataMemberSpec::Create(TypeName::Of("Mode"), "mode")
                           .SetInitializer("Mode::kOff")
                           .SetAccessSpecifier(AccessSpecifier::kPublic))
        .AddDataMember(
            DataMemberSpec::Create(th::SysType("uint32_t", "cstdint"), "baud")
                .SetInitializer("9600")
                .SetAccessSpecifier(AccessSpecifier::kPublic));
    return spec;
}

ClassSpec BuildDeviceClass() {
    ClassSpec spec = ClassSpec::Create(ClassSpec::Kind::kClass, "Device");
    spec.SetAccessSpecifier(AccessSpecifier::kPublic)
        .AddDataMember(
            DataMemberSpec::Create(th::SysType("uint32_t", "cstdint"), "id_")
                .SetInitializer("0")
                .SetAccessSpecifier(AccessSpecifier::kPrivate))
        .AddDataMember(DataMemberSpec::Create(TypeName::Of("Config"), "cfg_")
                           .SetAccessSpecifier(AccessSpecifier::kPrivate));

    MemberFunctionSpec ctor = MemberFunctionSpec::CreateConstructor();
    ctor.SetAccessSpecifier(AccessSpecifier::kPublic)
        .AddModifier(Modifier::kDefaulted);
    spec.AddMemberFunction(ctor);

    MemberFunctionSpec id = MemberFunctionSpec::Create("Id");
    id.SetReturnType(th::SysType("uint32_t", "cstdint"))
        .SetAccessSpecifier(AccessSpecifier::kPublic)
        .AddModifier(Modifier::kConst);
    auto id_body = CodeBlock::NewBuilder();
    id_body.AddStatement("return id_");
    id.AddCode(id_body.Build());
    spec.AddMemberFunction(id);

    MemberFunctionSpec reset = MemberFunctionSpec::Create("Reset");
    reset.SetReturnType(TypeName::Of("void"))
        .SetAccessSpecifier(AccessSpecifier::kPublic);
    auto reset_body = CodeBlock::NewBuilder();
    reset_body.AddStatement("id_ = $L", {"0"});
    reset.AddCode(reset_body.Build());
    spec.AddMemberFunction(reset);

    MemberFunctionSpec is_on = MemberFunctionSpec::Create("IsOn");
    is_on.SetReturnType(TypeName::Of("bool"))
        .SetAccessSpecifier(AccessSpecifier::kPublic)
        .AddModifier(Modifier::kInline);
    auto on_body = CodeBlock::NewBuilder();
    on_body.AddStatement("return cfg_.mode == Mode::kOn");
    is_on.AddCode(on_body.Build());
    spec.AddMemberFunction(is_on);

    return spec;
}

FreeFunctionSpec BuildAddFreeFunction() {
    FreeFunctionSpec fn = FreeFunctionSpec::Create("add");
    fn.SetReturnType(TypeName::Of("int"))
        .AddParameter(ParameterSpec::Create(TypeName::Of("int"), "a"))
        .AddParameter(ParameterSpec::Create(TypeName::Of("int"), "b"));
    auto body = CodeBlock::NewBuilder();
    body.AddStatement("return a + b");
    fn.AddCode(body.Build());
    return fn;
}

FileSpec BuildSampleFile() {
    FileSpec file = FileSpec::Create("rt_sample", {"rtns"});
    file.AddForwardDeclaration(ForwardDeclSpec::Create("Fwd"));
    file.AddEnum(EnumSpec::Create("Mode")
                     .SetScope(EnumSpec::Scope::kScoped)
                     .SetUnderlyingType(th::SysType("uint8_t", "cstdint"))
                     .AddConstant("kOff", "0")
                     .AddConstant("kOn", "1"));
    file.AddClass(BuildConfigStruct());
    file.AddClass(BuildDeviceClass());
    file.AddFreeFunction(BuildAddFreeFunction());
    file.AddVariable(VariableSpec::Create(TypeName::Of("int"), "kLimit")
                         .AddModifier(Modifier::kConst)
                         .SetInitializer("42"));
    file.AddAlias(FileSpec::Placement::kBack,
                  TypeAliasSpec::Create("DevicePtr")
                      .SetTarget(TypeName::Of("Device").AsPtr()));
    return file;
}

}  // namespace

CPPPOET_TEST(Roundtrip_generated_code_compiles) {
    const std::filesystem::path dir =
        std::filesystem::temp_directory_path() / "cpppoet_roundtrip";
    std::error_code ec;
    std::filesystem::remove_all(dir, ec);
    std::filesystem::create_directories(dir);

    FileSpec file = BuildSampleFile();
    FileWriter writer{EmitOptions{}};
    EXPECT_TRUE(writer.WriteHeaderAndSource(file, dir.string(), dir.string()));

    {
        std::ofstream header_check(dir / "header_check.cc");
        header_check << "#include \"rt_sample.h\"\nint main() { return 0; }\n";
    }

    const std::string base = Quote(CPPPOET_TEST_CXX_COMPILER) +
                             " -std=c++17 -I" + Quote(dir.string()) + " -I" +
                             Quote(CPPPOET_TEST_INCLUDE_DIR);
    const std::string log = (dir / "compile.log").string();

    const int source_rc =
        Run(base + " -c " + Quote((dir / "rt_sample.cc").string()) + " -o " +
            Quote((dir / "rt_sample.o").string()) + " 2> " + Quote(log));
    EXPECT_EQ(0, source_rc);
    if (source_rc != 0) {
        std::cout << ReadFile(log) << "\n";
    }

    const int header_rc =
        Run(base + " " + Quote((dir / "header_check.cc").string()) + " -o " +
            Quote((dir / "header_check").string()) + " 2> " + Quote(log));
    EXPECT_EQ(0, header_rc);
    if (header_rc != 0) {
        std::cout << ReadFile(log) << "\n";
    }
}

#endif  // !defined(_WIN32)
