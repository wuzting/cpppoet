// Copyright 2026 wuzting
// SPDX-License-Identifier: MIT

/// @file
/// @brief Data-driven example: an options table becomes a struct with a
/// Reset().
///
/// Generated files (default under ./generated): options.h / options.cc.
/// Usage: example_options [output_dir]

#include <cpppoet/cpppoet.h>

#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

using namespace cpppoet;

namespace {

struct OptionDef {
    TypeName type;
    std::string name;
    std::string default_value;
};

std::vector<OptionDef> OptionTable() {
    return {
        {TypeName::Of("int"), "timeout_seconds", "30"},
        {TypeName::Of("bool"), "verbose", "false"},
        {TypeName::Of("std::string").WithSystemHeader("string"), "log_file",
         "\"\""},
    };
}

ClassSpec BuildOptions(const std::vector<OptionDef>& defs) {
    ClassSpec options = ClassSpec::Create(ClassSpec::Kind::kStruct, "Options");
    options.SetAccessSpecifier(AccessSpecifier::kPublic);

    MemberFunctionSpec reset = MemberFunctionSpec::Create("Reset");
    reset.SetReturnType(TypeName::Of("void"));

    auto body = CodeBlock::NewBuilder();
    for (const OptionDef& def : defs) {
        options.AddDataMember(
            DataMemberSpec::Create(def.type, def.name)
                .SetInitializer(def.default_value)
                .SetAccessSpecifier(AccessSpecifier::kPublic));
        body.AddStatement("$N = $L", {def.name, def.default_value});
    }

    reset.AddCode(body.Build());
    options.AddMemberFunction(reset);
    return options;
}

std::string ReadFile(const std::string& path) {
    std::ifstream in(path);
    return std::string(std::istreambuf_iterator<char>(in),
                       std::istreambuf_iterator<char>());
}

}  // namespace

int main(int argc, char** argv) {
    const std::string out_dir = (argc > 1) ? argv[1] : "generated";

    FileSpec file = FileSpec::Create("options", {"myapp", "generated"});
    file.AddClass(BuildOptions(OptionTable()));

    FileWriter writer{EmitOptions{}};
    if (!writer.WriteHeaderAndSource(file, out_dir, out_dir)) {
        std::cerr << "failed to write generated files under: " << out_dir
                  << "\n";
        return 1;
    }

    std::cout << "wrote " << out_dir << "/options.h and " << out_dir
              << "/options.cc\n\n"
              << ReadFile(out_dir + "/options.h");
    return 0;
}
