# Contributing to cpppoet

Thanks for your interest in improving cpppoet. This guide explains how to build
and test the project, the code style, and how to get a change merged.

By participating you agree to follow our [Code of Conduct](CODE_OF_CONDUCT.md).

## Development environment

cpppoet is a C++17 library built with CMake.

- A C++17 compiler: GCC, Clang/AppleClang, or MSVC
- CMake 3.8 or newer
- (optional) `clang-format` 18.1.8, the version pinned in CI
- (optional) `clang-tidy` and Doxygen for local checks and docs

## Build and test

```bash
cmake -S . -B build \
    -DCMAKE_BUILD_TYPE=Release \
    -DENABLE_CPPPOET_EXAMPLE=ON \
    -DENABLE_CPPPOET_TEST=ON
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

On multi-config generators (Visual Studio, Xcode) add `--config Release` to the
build and `-C Release` to `ctest`.

Treating warnings as errors is recommended while developing:

```bash
cmake -S . -B build -DCPPPOET_WERROR=ON -DENABLE_CPPPOET_TEST=ON
```

## Coding style

- Formatting is defined by [`.clang-format`](.clang-format) (Google baseline,
  4-space indent, 80 columns). Do not hand-format; run the formatter:

  ```bash
  clang-format -i $(git ls-files '*.cc' '*.h')
  ```

  CI checks formatting with a pinned `clang-format==18.1.8`. If your local
  version differs, expect small differences.

- Prefer the existing conventions: one public header per spec, fluent
  `Create()`/`Add*`/`Set*` builders returning `*this`, `CPPPOET_EXPORT` on
  public symbols, and internal helpers under `namespace cpppoet::detail`.
- Public API changes should come with tests and, when notable, a
  [CHANGELOG](CHANGELOG.md) entry.

## Commit messages

This project uses [Conventional Commits](https://www.conventionalcommits.org/)
with a minimal format: `type: subject`, **no scope**, written in English.

```
<type>: <imperative subject in English>

[optional body: explain why, not just what]

[optional footer: Closes #12 / BREAKING CHANGE: ...]
```

- Types: `feat`, `fix`, `refactor`, `perf`, `docs`, `test`, `build`, `ci`,
  `chore`, `revert`.
- Subject: imperative mood, lowercase after the colon, max ~50 characters, no
  trailing period.

Examples:

```
feat: add constexpr support for member functions
fix: stop dropping unsupported modifiers silently
docs: document the CodeBlock placeholder vocabulary
```

## Pull requests

1. Fork the repository and create a topic branch.
2. Make focused commits; keep unrelated changes out of the PR.
3. Ensure the build and test suite pass and the code is formatted.
4. Open a pull request against `master` and fill in the template.
5. A maintainer will review. Address feedback with additional commits (avoid
   force-pushing over review history until asked).

## Reporting bugs

Use the [bug report template](.github/ISSUE_TEMPLATE/bug_report.yml) and include
a minimal reproduction. For security issues, do **not** open a public issue —
see [SECURITY.md](SECURITY.md).

## License

By contributing, you agree that your contributions are licensed under the
[MIT License](LICENSE).
