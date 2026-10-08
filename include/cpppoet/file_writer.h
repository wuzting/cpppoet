// Copyright 2026 wuzting
// SPDX-License-Identifier: MIT

/// @file file_writer.h
/// @brief Declares FileWriter, which renders a FileSpec to .h and .cc
/// files.

#pragma once

#include <string>

#include "cpppoet/export.h"
#include "file_spec.h"
#include "types.h"

namespace cpppoet {

class CPPPOET_EXPORT FileWriter {
public:
    /// @brief Creates a writer.
    /// @param options The emission options.
    FileWriter(const EmitOptions& options);

    /// @brief Writes a self-contained header.
    /// @param file_spec The file to render.
    /// @param output_dir The directory to write into.
    /// @return True on success.
    bool WriteHeader(const FileSpec& file_spec,
                     const std::string& output_dir) const;
    /// @brief Writes a header and a matching source file.
    /// @param file_spec The file to render.
    /// @param header_output_dir The directory for the header.
    /// @param source_output_dir The directory for the source.
    /// @return True on success.
    bool WriteHeaderAndSource(const FileSpec& file_spec,
                              const std::string& header_output_dir,
                              const std::string& source_output_dir) const;
    /// @brief Writes a self-contained source file.
    /// @param file_spec The file to render.
    /// @param output_dir The directory to write into.
    /// @return True on success.
    bool WriteSource(const FileSpec& file_spec,
                     const std::string& output_dir) const;

private:
    std::string RenderHeader(const FileSpec& file_spec) const;
    std::string RenderSource(const FileSpec& file_spec,
                             bool as_inline = false) const;
    std::string FilePath(const FileSpec& file_spec,
                         const std::string& output_dir,
                         const std::string& ext) const;

    EmitOptions options_;
};

}  // namespace cpppoet
