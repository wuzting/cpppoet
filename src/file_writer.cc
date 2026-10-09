// Copyright 2026 wuzting
// SPDX-License-Identifier: MIT

/// @file file_writer.cc
/// @brief Implements FileWriter.

#include "cpppoet/file_writer.h"

#include <filesystem>
#include <fstream>
#include <string>

#include "render/renderer.h"

namespace cpppoet {
using detail::Renderer;
using std::string;

bool WriteFile(const string& path, const string& content) {
    std::filesystem::path full(path);
    if (full.has_parent_path()) {
        std::error_code ec;
        std::filesystem::create_directories(full.parent_path(), ec);
    }
    std::ofstream out(path);
    if (!out.is_open()) {
        return false;
    }

    out << content;
    return true;
}

FileWriter::FileWriter(const EmitOptions& options) : options_(options) {}

string FileWriter::RenderHeader(const FileSpec& file_spec) const {
    Renderer renderer(options_);
    renderer.RenderHeaderFile(file_spec);
    return renderer.ToString();
}

string FileWriter::RenderSource(const FileSpec& file_spec,
                                bool as_inline) const {
    Renderer renderer(options_);
    renderer.RenderSourceFile(file_spec, as_inline);
    return renderer.ToString();
}

string FileWriter::FilePath(const FileSpec& file_spec, const string& output_dir,
                            const string& ext) const {
    string path = output_dir;
    if (!path.empty() && path.back() != '/') {
        path += "/";
    }

    return path + file_spec.Name() + ext;
}

bool FileWriter::WriteHeader(const FileSpec& file_spec,
                             const string& output_dir) const {
    Renderer renderer(options_);
    renderer.RenderSelfContainedHeader(file_spec);
    return WriteFile(FilePath(file_spec, output_dir, options_.header_ext),
                     renderer.ToString());
}

bool FileWriter::WriteHeaderAndSource(const FileSpec& file_spec,
                                      const string& header_output_dir,
                                      const string& source_output_dir) const {
    return WriteFile(
               FilePath(file_spec, header_output_dir, options_.header_ext),
               RenderHeader(file_spec)) &&
           WriteFile(
               FilePath(file_spec, source_output_dir, options_.source_ext),
               RenderSource(file_spec));
}

bool FileWriter::WriteSource(const FileSpec& file_spec,
                             const string& output_dir) const {
    Renderer renderer(options_);
    renderer.RenderSelfContainedSource(file_spec);
    return WriteFile(FilePath(file_spec, output_dir, options_.source_ext),
                     renderer.ToString());
}

}  // namespace cpppoet
