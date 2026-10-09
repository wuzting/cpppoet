# cpppoet

[![CI](https://github.com/wuzting/cpppoet/actions/workflows/ci.yml/badge.svg)](https://github.com/wuzting/cpppoet/actions/workflows/ci.yml)
[![Coverage](https://github.com/wuzting/cpppoet/actions/workflows/coverage.yml/badge.svg)](https://github.com/wuzting/cpppoet/actions/workflows/coverage.yml)
[![Docs](https://github.com/wuzting/cpppoet/actions/workflows/docs.yml/badge.svg)](https://wuzting.github.io/cpppoet/)
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](#环境要求)
[![Version](https://img.shields.io/badge/version-0.1.0-blue.svg)](CHANGELOG.md)

简体中文 | [English](README.en.md)

cpppoet 是一个用 C++17 编写的 C++ 代码生成库，API 设计借鉴 JavaPoet：用流畅（fluent）构建器以结构化方式描述类、枚举、函数与代码块，再渲染为可直接编译的 `.h` / `.cc` 文件，而不是手工拼接字符串。

## 特性

- **流畅构建器 API**：`FileSpec`、`ClassSpec`、`EnumSpec`、`FreeFunctionSpec`、`MemberFunctionSpec`、`DataMemberSpec`、`ParameterSpec`、`VariableSpec`、`TypeAliasSpec`、`ForwardDeclSpec`。
- **JavaPoet 风格代码块**：`CodeBlock` 支持占位符与控制流，并自动处理缩进。
- **类型与头文件追踪**：`TypeName` 携带系统 / 本地头文件信息，渲染时自动去重、排序并生成 `#include`。
- **头文件 / 源文件分离**：`FileWriter` 可写自包含头文件、自包含源文件，或同时生成头文件与配套源文件。
- **可配置输出**：`EmitOptions` 控制缩进、`#pragma once` 与文件扩展名。
- **现代 CMake**：安装并导出 `cpppoet::cpppoet`，支持 `find_package`。

## 环境要求

- C++17 编译器（GCC / Clang / AppleClang / MSVC）
- CMake 3.8 或更高

## 构建与安装

```bash
cmake -S . -B build -DENABLE_CPPPOET_EXAMPLE=ON -DENABLE_CPPPOET_TEST=ON
cmake --build build
ctest --test-dir build --output-on-failure
cmake --install build --prefix /your/prefix
```

或使用 [CMake Presets](CMakePresets.json)：

```bash
cmake --preset dev          # Debug + 测试 + 示例 + Werror
cmake --build --preset dev
ctest --preset dev
```

可配置选项：

| 选项 | 默认 | 说明 |
|------|------|------|
| `ENABLE_CPPPOET_MODULE` | `ON` | 构建核心库 |
| `ENABLE_CPPPOET_EXAMPLE` | `OFF` | 构建示例 |
| `ENABLE_CPPPOET_TEST` | `OFF` | 构建测试 |
| `BUILD_SHARED_LIBS` | `ON` | 共享库；设为 `OFF` 构建静态库 |
| `CPPPOET_WERROR` | `OFF` | 将警告视为错误 |

在其它 CMake 工程中使用：

```cmake
find_package(cpppoet REQUIRED)
target_link_libraries(my_generator PRIVATE cpppoet::cpppoet)
```

或用 `FetchContent` 直接引入源码：

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

也可以用仓库内的 [Conan recipe](conanfile.py) 构建并打包：

```bash
conan create . --build=missing
```

运行时可通过 `cpppoet::Version()`（或宏 `CPPPOET_VERSION`）查询版本。

## 快速开始

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

生成的 `generated/hello.h`：

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

配套的 `generated/hello.cc` 会包含 `#include "hello.h"`、相同的 `#include <string>`、以及分离的构造函数 / `Greet` / `MakeGreeter` 定义。

完整可运行示例见 `examples/`。

## 核心概念

| 类型 | 作用 |
|------|------|
| `FileSpec` | 顶层翻译单元：文件名、命名空间，以及其中的枚举、类、别名、变量、自由函数与前向声明。 |
| `ClassSpec` | `class` / `struct`：基类、接口、模板参数、数据成员、成员函数与嵌套类型。 |
| `EnumSpec` | `enum` / `enum class`：作用域、底层类型与枚举值。 |
| `FreeFunctionSpec` | 命名空间作用域函数。 |
| `MemberFunctionSpec` | 成员函数与构造函数（含初始化列表）。 |
| `DataMemberSpec` / `VariableSpec` | 类数据成员 / 命名空间作用域变量。 |
| `ParameterSpec` | 函数参数：按值、引用、指针或右值引用，可设 `const` 与默认值。 |
| `TypeAliasSpec` | `using` 或 `typedef` 别名。 |
| `ForwardDeclSpec` | 类 / 结构体前向声明，支持模板参数。 |
| `TypeName` | 类型表达式：模板参数、`const` / 引用 / 指针，以及关联头文件。 |
| `CodeBlock` | 带占位符与控制流的代码片段。 |
| `FileWriter` | 将 `FileSpec` 渲染并写入磁盘。 |

### 访问级别与修饰符

- `AccessSpecifier`：`kPublic`、`kProtected`、`kPrivate`
- `Modifier`：`kStatic`、`kExtern`、`kThreadLocal`、`kVirtual`、`kPureVirtual`、`kConst`、`kConstexpr`、`kInline`、`kExplicit`、`kOverride`、`kFinal`、`kNoexcept`、`kDefaulted`、`kDeleted`、`kFriend`

### CodeBlock 占位符

| 占位符 | 含义 |
|--------|------|
| `$L` | 字面量（原样输出） |
| `$N` | 名称 |
| `$S` | 字符串字面量（自动加引号并转义） |
| `$T` | 类型（同时记录其头文件） |
| `$$` | 美元符号 `$` |
| `$>` / `$<` | 增加 / 减少一级缩进 |

控制流使用 `BeginControlFlow` / `NextControlFlow` / `EndControlFlow`；语句使用 `AddStatement`（自动追加 `;`）。

### 输出模式

`FileWriter` 提供三种写出方式：

| 方法 | 说明 |
|------|------|
| `WriteHeader` | 只写头文件，定义以 inline 形式内联其中。 |
| `WriteSource` | 只写源文件（自包含）。 |
| `WriteHeaderAndSource` | 同时写头文件与配套源文件，声明与定义分离。 |

`EmitOptions` 可配置缩进（默认 4 空格）、`#pragma once`（默认开启）、头 / 源文件扩展名（默认 `.h` / `.cc`）。`column_limit` 为预留字段。

## 示例

```bash
cmake -S . -B build -DENABLE_CPPPOET_EXAMPLE=ON
cmake --build build
./build/examples/example_hello generated
./build/examples/example_enum generated
./build/examples/example_options generated
```

- `example_hello`：构建 `Greeter` 类与自由函数。
- `example_enum`：由状态码表生成作用域枚举与 `to_string()`。
- `example_options`：由选项表生成带 `Reset()` 的结构体。

## 测试

```bash
cmake -S . -B build -DENABLE_CPPPOET_TEST=ON
cmake --build build
ctest --test-dir build --output-on-failure
```

测试覆盖类型名、声明、成员函数、类、代码块与文件写出；在非 Windows 平台还会把生成的代码交给宿主编译器，验证其确实可以编译（roundtrip）。

## 项目结构

```
include/cpppoet/   公开头文件（每个 spec 一个头，cpppoet.h 为总头）
src/               spec 实现、渲染器与文件写出
src/render/        内部渲染器与 include 收集器
examples/          可运行示例（hello / enum / options）
tests/             单元测试与 consumer 工程
cmake/             CMake 包配置模板
```

## 文档

- 在线 API 文档：<https://wuzting.github.io/cpppoet/>
- 路线图：[ROADMAP.md](ROADMAP.md)
- 变更记录：[CHANGELOG.md](CHANGELOG.md)

本地生成 API 文档（需要 Doxygen）：

```bash
cmake -S . -B build -DENABLE_CPPPOET_DOCS=ON
cmake --build build --target docs
```

## 参与贡献

欢迎提交 issue 与 PR。请先阅读 [CONTRIBUTING.md](CONTRIBUTING.md) 与[行为准则](CODE_OF_CONDUCT.md)；安全问题请按 [SECURITY.md](SECURITY.md) 私下上报，切勿开公开 issue。变更记录见 [CHANGELOG.md](CHANGELOG.md)。

## 许可证

本项目采用 MIT 许可，全文见 [LICENSE](LICENSE)；各源文件头部亦标注了 `SPDX-License-Identifier: MIT`。
