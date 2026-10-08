// Copyright 2026 wuzting
// SPDX-License-Identifier: MIT

/// @file
/// @brief Table-driven example: status codes become a scoped enum and a
/// to_string().
///
/// Generated files (default under ./generated): status.h / status.cc.
/// Usage: example_enum [output_dir]

#include <cpppoet/cpppoet.h>

#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

using namespace cpppoet;

namespace {

struct StatusDef {
    const char* name;
    int value;
};

const std::vector<StatusDef> kStatuses = {
    {"kOk", 0},
    {"kCancelled", 1},
    {"kUnknown", 2},
};

EnumSpec BuildStatus() {
    EnumSpec status =
        EnumSpec::Create("Status")
            .SetScope(EnumSpec::Scope::kScoped)
            .SetUnderlyingType(
                TypeName::Of("uint8_t").WithSystemHeader("cstdint"));
    for (const StatusDef& def : kStatuses) {
        status.AddConstant(def.name, std::to_string(def.value));
    }
    return status;
}

FreeFunctionSpec BuildToString() {
    FreeFunctionSpec fn = FreeFunctionSpec::Create("to_string");
    fn.SetReturnType(TypeName::Of("const char *"))
        .AddParameter(ParameterSpec::Create(TypeName::Of("Status"), "status"));

    auto body = CodeBlock::NewBuilder();
    body.BeginControlFlow("switch (status)");
    for (const StatusDef& def : kStatuses) {
        body.AddStatement("case Status::$N: return $S", {def.name, def.name});
    }
    body.AddStatement("default: return $S", {"unknown"});
    body.EndControlFlow();
    fn.AddCode(body.Build());
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

    FileSpec file = FileSpec::Create("status", {"myapp", "generated"});
    file.AddEnum(BuildStatus());
    file.AddFreeFunction(BuildToString());

    FileWriter writer{EmitOptions{}};
    if (!writer.WriteHeaderAndSource(file, out_dir, out_dir)) {
        std::cerr << "failed to write generated files under: " << out_dir
                  << "\n";
        return 1;
    }

    std::cout << "wrote " << out_dir << "/status.h and " << out_dir
              << "/status.cc\n\n"
              << ReadFile(out_dir + "/status.h");
    return 0;
}
