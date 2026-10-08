// Copyright 2026 wuzting
// SPDX-License-Identifier: MIT

/// @file
/// @brief Minimal example: build a Greeter class and write it to disk.
///
/// Generated files (default under ./generated): hello.h / hello.cc.
/// Usage: example_hello [output_dir]

#include <cpppoet/cpppoet.h>

#include <fstream>
#include <iostream>
#include <iterator>
#include <string>

using namespace cpppoet;

namespace {

TypeName StringType() {
    return TypeName::Of("std::string").WithSystemHeader("string");
}

ClassSpec BuildGreeter() {
    ClassSpec greeter = ClassSpec::Create(ClassSpec::Kind::kClass, "Greeter");
    greeter.SetAccessSpecifier(AccessSpecifier::kPublic);

    MemberFunctionSpec ctor = MemberFunctionSpec::CreateConstructor();
    ctor.AddParameter(ParameterSpec::Create(StringType(), "name"))
        .AddInitializer("name_", "name");

    MemberFunctionSpec greet = MemberFunctionSpec::Create("Greet");
    greet.SetReturnType(StringType()).AddModifier(Modifier::kConst);
    greet.AddStatement("return $S + name_ + $S", {"Hello, ", "!"});

    greeter.AddMemberFunction(ctor);
    greeter.AddMemberFunction(greet);
    greeter.AddDataMember(DataMemberSpec::Create(StringType(), "name_")
                              .SetAccessSpecifier(AccessSpecifier::kPrivate));
    return greeter;
}

FreeFunctionSpec BuildMakeGreeter() {
    FreeFunctionSpec fn = FreeFunctionSpec::Create("MakeGreeter");
    fn.SetReturnType(TypeName::Of("Greeter"))
        .AddParameter(ParameterSpec::Create(StringType(), "name"));
    fn.AddStatement("return Greeter(name)");
    return fn;
}

std::string ReadFile(const std::string& path) {
    std::ifstream in(path);
    return std::string(std::istreambuf_iterator<char>(in),
                       std::istreambuf_iterator<char>());
}

}  // namespace

int main(int argc, char** argv) {
    const std::string out_dir = (argc > 1) ? argv[1] : "generated";

    FileSpec file = FileSpec::Create("hello", {"myapp", "generated"});
    file.AddClass(BuildGreeter());
    file.AddFreeFunction(BuildMakeGreeter());

    FileWriter writer{EmitOptions{}};
    if (!writer.WriteHeaderAndSource(file, out_dir, out_dir)) {
        std::cerr << "failed to write generated files under: " << out_dir
                  << "\n";
        return 1;
    }

    std::cout << "wrote " << out_dir << "/hello.h and " << out_dir
              << "/hello.cc\n\n"
              << ReadFile(out_dir + "/hello.h");
    return 0;
}
