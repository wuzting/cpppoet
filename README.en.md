# cpppoet

[![CI](https://github.com/wuzting/cpppoet/actions/workflows/ci.yml/badge.svg)](https://github.com/wuzting/cpppoet/actions/workflows/ci.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](#requirements)
[![Version](https://img.shields.io/badge/version-0.1.0-blue.svg)](CHANGELOG.md)

[简体中文](README.md) | English

cpppoet is a C++17 library for generating C++ source code. Its API is inspired by JavaPoet: describe classes, enums, functions and code blocks with fluent builders, then render them to compilable `.h` / `.cc` files instead of stitching strings together by hand.

## Features

- **Fluent builder API**: `FileSpec`, `ClassSpec`, `EnumSpec`, `FreeFunctionSpec`, `MemberFunctionSpec`, `DataMemberSpec`, `ParameterSpec`, `VariableSpec`, `TypeAliasSpec`, `ForwardDeclSpec`.
- **JavaPoet-style code blocks**: `CodeBlock` supports placeholders and control flow with automatic indentation.
- **Type and include tracking**: `TypeName` carries system/local header information; the renderer deduplicates, sorts and emits the `#include` lines for you.
- **Header/source split**: `FileWriter` can write a self-contained header, a self-contained source, or a header plus its matching source.
- **Configurable output**: `EmitOptions` controls indentation, `#pragma once` and file extensions.
- **Modern CMake**: installs and exports `cpppoet::cpppoet` for `find_package`.

## Requirements

- A C++17 compiler (GCC / Clang / AppleClang / MSVC)
- CMake 3.8 or newer

## Build and Install

```bash
cmake -S . -B build -DENABLE_CPPPOET_EXAMPLE=ON -DENABLE_CPPPOET_TEST=ON
cmake --build build
ctest --test-dir build --output-on-failure
cmake --install build --prefix /your/prefix
```

Options:

| Option | Default | Description |
|--------|---------|-------------|
| `ENABLE_CPPPOET_MODULE` | `ON` | Build the core library |
| `ENABLE_CPPPOET_EXAMPLE` | `OFF` | Build the examples |
| `ENABLE_CPPPOET_TEST` | `OFF` | Build the tests |
| `BUILD_SHARED_LIBS` | `ON` | Shared library; set `OFF` for a static build |
| `CPPPOET_WERROR` | `OFF` | Treat warnings as errors |

Consume it from another CMake project:

```cmake
find_package(cpppoet REQUIRED)
target_link_libraries(my_generator PRIVATE cpppoet::cpppoet)
```

Or pull the sources with `FetchContent`:

```cmake
include(FetchContent)
FetchContent_Declare(
    cpppoet
    GIT_REPOSITORY https://github.com/wuzting/cpppoet.git
    GIT_TAG        v0.1.0
)
FetchContent_MakeAvailable(cpppoet)
target_link_libraries(my_generator PRIVATE cpppoet::cpppoet)
```

## Quick Start

```cpp
#include <cpppoet/cpppoet.h>

using namespace cpppoet;

int main() {
    // class Greeter { ... };
    ClassSpec greeter = ClassSpec::Create(ClassSpec::Kind::kClass, "Greeter");
    greeter.SetAccessSpecifier(AccessSpecifier::kPublic);

    MemberFunctionSpec ctor = MemberFunctionSpec::CreateConstructor();
    ctor.AddParameter(ParameterSpec::Create(
                          TypeName::Of("std::string").WithSystemHeader("string"),
                          "name"))
        .AddInitializer("name_", "name");

    MemberFunctionSpec greet = MemberFunctionSpec::Create("Greet");
    greet.SetReturnType(TypeName::Of("std::string").WithSystemHeader("string"))
        .AddModifier(Modifier::kConst);
    greet.AddStatement("return $S + name_ + $S", {"Hello, ", "!"});

    greeter.AddMemberFunction(ctor);
    greeter.AddMemberFunction(greet);
    greeter.AddDataMember(
        DataMemberSpec::Create(
            TypeName::Of("std::string").WithSystemHeader("string"), "name_")
            .SetAccessSpecifier(AccessSpecifier::kPrivate));

    // Greeter MakeGreeter(std::string name);
    FreeFunctionSpec make = FreeFunctionSpec::Create("MakeGreeter");
    make.SetReturnType(TypeName::Of("Greeter"))
        .AddParameter(ParameterSpec::Create(
            TypeName::Of("std::string").WithSystemHeader("string"), "name"));
    make.AddStatement("return Greeter(name)");

    FileSpec file = FileSpec::Create("hello", {"myapp", "generated"});
    file.AddClass(greeter);
    file.AddFreeFunction(make);

    FileWriter writer{EmitOptions{}};
    return writer.WriteHeaderAndSource(file, "generated", "generated") ? 0 : 1;
}
```

The generated `generated/hello.h`:

```cpp
#pragma once

#include <string>

namespace myapp {
namespace generated {

class Greeter {
public:
    Greeter(std::string name);
    std::string Greet() const;
private:
    std::string name_;
};

Greeter MakeGreeter(std::string name);

}  // namespace generated
}  // namespace myapp
```

The matching `generated/hello.cc` includes `#include "hello.h"`, the same `#include <string>`, and the out-of-line definitions of the constructor, `Greet` and `MakeGreeter`.

Runnable examples live in `examples/`.

## Core Concepts

| Type | Purpose |
|------|---------|
| `FileSpec` | Top-level translation unit: file name, namespaces, and its enums, classes, aliases, variables, free functions and forward declarations. |
| `ClassSpec` | A `class` / `struct`: base classes, interfaces, template parameters, data members, member functions and nested types. |
| `EnumSpec` | An `enum` / `enum class`: scope, underlying type and enumerators. |
| `FreeFunctionSpec` | A namespace-scope function. |
| `MemberFunctionSpec` | A member function or constructor (including initializer lists). |
| `DataMemberSpec` / `VariableSpec` | Class data member / namespace-scope variable. |
| `ParameterSpec` | A function parameter: by value, reference, pointer or rvalue reference, with optional `const` and default value. |
| `TypeAliasSpec` | A `using` or `typedef` alias. |
| `ForwardDeclSpec` | A class / struct forward declaration, with optional template parameters. |
| `TypeName` | A type expression: template arguments, `const` / reference / pointer, and associated include headers. |
| `CodeBlock` | A code fragment with placeholders and control flow. |
| `FileWriter` | Renders a `FileSpec` and writes it to disk. |

### Access Levels and Modifiers

- `AccessSpecifier`: `kPublic`, `kProtected`, `kPrivate`
- `Modifier`: `kStatic`, `kExtern`, `kThreadLocal`, `kVirtual`, `kPureVirtual`, `kConst`, `kConstexpr`, `kInline`, `kExplicit`, `kOverride`, `kFinal`, `kNoexcept`, `kDefaulted`, `kDeleted`, `kFriend`

### CodeBlock Placeholders

| Placeholder | Meaning |
|-------------|---------|
| `$L` | Literal (emitted verbatim) |
| `$N` | Name |
| `$S` | String literal (quoted and escaped) |
| `$T` | Type (also records its include header) |
| `$$` | A dollar sign `$` |
| `$>` / `$<` | Indent / unindent one level |

Use `BeginControlFlow` / `NextControlFlow` / `EndControlFlow` for control flow and `AddStatement` for statements (it appends the `;` for you).

### Output Modes

`FileWriter` offers three ways to write:

| Method | Description |
|--------|-------------|
| `WriteHeader` | Writes only a header, with definitions inlined. |
| `WriteSource` | Writes only a self-contained source file. |
| `WriteHeaderAndSource` | Writes a header and its matching source, separating declarations from definitions. |

`EmitOptions` configures the indentation (4 spaces by default), `#pragma once` (on by default) and the header/source extensions (`.h` / `.cc` by default). `column_limit` is reserved for future use.

## Examples

```bash
cmake -S . -B build -DENABLE_CPPPOET_EXAMPLE=ON
cmake --build build
./build/examples/example_hello generated
./build/examples/example_enum generated
./build/examples/example_options generated
```

- `example_hello`: builds a `Greeter` class and a free function.
- `example_enum`: turns a status-code table into a scoped enum and `to_string()`.
- `example_options`: turns an options table into a struct with a `Reset()`.

## Tests

```bash
cmake -S . -B build -DENABLE_CPPPOET_TEST=ON
cmake --build build
ctest --test-dir build --output-on-failure
```

The suite covers type names, declarations, member functions, classes, code blocks and file writing. On non-Windows platforms it also hands the generated code to the host compiler to prove it actually compiles (round-trip).

## Project Layout

```
include/cpppoet/   Public headers (one per spec; cpppoet.h is the umbrella)
src/               Spec implementations, renderer and file writer
src/render/        Internal renderer and include collector
examples/          Runnable examples (hello / enum / options)
tests/             Unit tests and the consumer project
cmake/             CMake package config template
```

## Contributing

Issues and pull requests are welcome. Please read
[CONTRIBUTING.md](CONTRIBUTING.md) and our
[Code of Conduct](CODE_OF_CONDUCT.md) first. Report security issues privately
as described in [SECURITY.md](SECURITY.md) — never in a public issue. See
[CHANGELOG.md](CHANGELOG.md) for notable changes.

## License

This project is licensed under the MIT License. See [LICENSE](LICENSE) for the
full text; each source file also carries an `SPDX-License-Identifier: MIT`
header.
