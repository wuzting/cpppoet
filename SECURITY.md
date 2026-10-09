# Security Policy

## Supported versions

cpppoet is pre-1.0. Security fixes are applied to the latest release on the
default branch.

| Version | Supported |
| ------- | --------- |
| 0.1.x   | ✅        |
| < 0.1   | ❌        |

## Reporting a vulnerability

Please report suspected vulnerabilities **privately** — do not open a public
issue, pull request, or discussion.

Use GitHub's private vulnerability reporting:

1. Go to the repository's **Security** tab.
2. Click **Report a vulnerability**.
3. Include a description, a minimal reproduction, affected version/commit, and
   any suggested fix.

Direct link: <https://github.com/wuzting/cpppoet/security/advisories/new>

If you cannot use GitHub, contact the maintainer, [@wuzting](https://github.com/wuzting),
to arrange a private channel.

## What to expect

- Acknowledgement of your report as soon as possible.
- An assessment and, when confirmed, a fix and coordinated disclosure.
- Credit in the release notes unless you prefer to remain anonymous.

## Scope

cpppoet generates C++ source text from trusted in-process input; it does not
parse untrusted input or handle secrets. Reports that are in scope include
memory-safety issues, path traversal in `FileWriter`, and injection into
generated code from `CodeBlock` arguments.
