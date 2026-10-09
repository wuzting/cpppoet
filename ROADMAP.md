# Roadmap

This roadmap records where cpppoet is heading. It is a living document; items
move as priorities change. Contributions toward any of these are welcome — see
[CONTRIBUTING.md](CONTRIBUTING.md).

## Near term

- **Restore a green MSVC build.** The Windows job is currently `experimental`
  in CI; fix the compiler/linker error and make it blocking again.
- **Fail loudly instead of silently.** Validate `CodeBlock` placeholder/argument
  arity and reject modifiers a spec cannot represent, rather than dropping them.
- **Centralize modifier emission** into a single table so every `Modifier` is
  either emitted or explicitly rejected for each spec kind.
- **Structured error reporting** for `FileWriter` (error code/message instead of
  a lossy `bool`), and check `create_directories` and stream state.
- **Close test gaps**: cover `WriteSource` and the self-contained header path in
  the round-trip test, and wire the `tests/consumer` package smoke test into
  CTest.

## Medium term

- **Documentation comments in generated code** (Javadoc-style `///` blocks).
- **Richer templates and inheritance**: default/non-type template parameters,
  variadic templates, and non-public/virtual base classes.
- **Name sanitization** for include guards and generated file names.
- **Column-limit aware wrapping** (currently `EmitOptions::column_limit` is a
  reserved, inert field).
- **Package distribution**: submit a `cpppoet` port to vcpkg and publish the
  Conan recipe alongside the in-repo `vcpkg.json`/`conanfile.py`.

## Longer term

- **Pluggable render backends** behind a model access contract, so the emitter
  can be swapped or extended without touching every spec.
- **`CodeBlock` expression model** instead of raw strings for initializers,
  default values and enum values.
- **Fuzzing** of `CodeBlock` and type rendering.

## Done

- Core fluent builders and render engine.
- `FileWriter` header/source/self-contained output modes.
- CMake install/export, examples, unit tests and round-trip compilation.
- CI (build/test matrix + formatting), docs and coverage workflows.
