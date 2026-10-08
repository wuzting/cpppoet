// Copyright 2026 wuzting
// SPDX-License-Identifier: MIT

/// @file code_block.cc
/// @brief Implements CodeBlock and CodeBlockArg.

#include "cpppoet/code_block.h"

#include <utility>

#include "render/renderer.h"

namespace cpppoet {
using std::string;
using std::vector;

CodeBlockArg::CodeBlockArg(string text)
    : kind_(Kind::kString), text_(std::move(text)) {}

CodeBlockArg::CodeBlockArg(const char* text)
    : kind_(Kind::kString), text_(text) {}

CodeBlockArg::CodeBlockArg(const TypeName& type)
    : kind_(Kind::kType), type_(type) {}

CodeBlockArg::Kind CodeBlockArg::GetKind() const { return kind_; }

const string& CodeBlockArg::Text() const { return text_; }

const TypeName& CodeBlockArg::Type() const { return type_; }

CodeBlock::Builder& CodeBlock::Builder::Add(const string& format,
                                            vector<CodeBlockArg> args) {
    size_t arg_index = 0;
    for (size_t i = 0; i < format.size();) {
        if (format[i] != '$') {
            size_t next = format.find('$', i);
            if (next == string::npos) {
                parts_.push_back(format.substr(i));
                break;
            }
            parts_.push_back(format.substr(i, next - i));
            i = next;
            continue;
        }
        i++;
        if (i >= format.size()) {
            break;
        }
        char c = format[i];
        switch (c) {
            case 'L':
            case 'N':
            case 'S':
            case 'T': {
                if (arg_index >= args.size()) {
                    parts_.push_back("$" + string(1, c));
                    break;
                }
                parts_.push_back("$" + string(1, c));
                args_.push_back(args[arg_index]);
                arg_index++;
                break;
            }
            case '$':
                parts_.push_back("$$");
                break;
            case '>':
                parts_.push_back("$>");
                break;
            case '<':
                parts_.push_back("$<");
                break;
            default:
                break;
        }
        i++;
    }
    return *this;
}

CodeBlock::Builder& CodeBlock::Builder::Add(const CodeBlock& block) {
    parts_.insert(parts_.end(), block.parts_.begin(), block.parts_.end());
    args_.insert(args_.end(), block.args_.begin(), block.args_.end());
    return *this;
}

CodeBlock::Builder& CodeBlock::Builder::BeginControlFlow(
    const string& control_flow, vector<CodeBlockArg> args) {
    Add(control_flow + " {\n", args);
    parts_.push_back("$>");
    return *this;
}

CodeBlock::Builder& CodeBlock::Builder::NextControlFlow(
    const string& control_flow, vector<CodeBlockArg> args) {
    parts_.push_back("$<");
    Add("} " + control_flow + " {\n", args);
    parts_.push_back("$>");
    return *this;
}

CodeBlock::Builder& CodeBlock::Builder::EndControlFlow() {
    parts_.push_back("$<");
    parts_.push_back("}\n");
    return *this;
}

CodeBlock::Builder& CodeBlock::Builder::EndControlFlow(
    const string& control_flow, vector<CodeBlockArg> args) {
    parts_.push_back("$<");
    Add("} " + control_flow + ";\n", args);
    return *this;
}

CodeBlock::Builder& CodeBlock::Builder::AddStatement(
    const string& format, vector<CodeBlockArg> args) {
    Add(format, args);
    parts_.push_back(";\n");
    return *this;
}

CodeBlock::Builder& CodeBlock::Builder::Indent() {
    parts_.push_back("$>");
    return *this;
}

CodeBlock::Builder& CodeBlock::Builder::Unindent() {
    parts_.push_back("$<");
    return *this;
}

CodeBlock CodeBlock::Builder::Build() {
    CodeBlock result;
    result.parts_ = std::move(parts_);
    result.args_ = std::move(args_);
    return result;
}

CodeBlock::Builder CodeBlock::NewBuilder() { return Builder(); }

bool CodeBlock::IsEmpty() const { return parts_.empty(); }

string CodeBlock::ToString() const {
    detail::Renderer renderer{EmitOptions{}};
    renderer.Render(*this);
    return renderer.ToString();
}

}  // namespace cpppoet
