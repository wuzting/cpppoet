# Changelog

All notable changes to this project are documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

## [0.1.0] - 2026-10-09

### Added

- Fluent, JavaPoet-style builders: `FileSpec`, `ClassSpec`, `EnumSpec`,
  `FreeFunctionSpec`, `MemberFunctionSpec`, `DataMemberSpec`, `ParameterSpec`,
  `VariableSpec`, `TypeAliasSpec`, and `ForwardDeclSpec`.
- `TypeName` type expressions with system/local include tracking.
- `CodeBlock` with `$L`/`$N`/`$S`/`$T`/`$$`/`$>`/`$<` placeholders and control
  flow helpers.
- `FileWriter` with self-contained header, self-contained source, and
  header-plus-source output modes, configured by `EmitOptions`.
- CMake build that installs and exports the `cpppoet::cpppoet` package target.
- Example generators (`hello`, `enum`, `options`).
- Unit test suite, including a round-trip test that compiles the generated code
  with the host compiler.
- Documentation in Chinese (`README.md`) and English (`README.en.md`).

[Unreleased]: https://github.com/wuzting/cpppoet/compare/v0.1.0...HEAD
[0.1.0]: https://github.com/wuzting/cpppoet/releases/tag/v0.1.0
