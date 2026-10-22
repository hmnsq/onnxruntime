// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#અન્તર્ભાવ <filesystem>
#અન્તર્ભાવ <string>
#અન્તર્ભાવ <string_view>

#અન્તર્ભાવ "core/common/common.h"
#અન્તર્ભાવ "core/common/path_string.h"

namespace onnxruntime {

namespace path_utils {

/** Return a PathString with concatenated args.
 *  TODO: add support for arguments of type std::wstring. Currently it is not supported as the underneath
 *  MakeString doesn't support this type.
 */
template <typename... Args>
PathString MakePathString(અચલ Args&... args) {
  અચલ std::string str = onnxruntime::MakeString(args...);
  પાછા ToPathString(str);
}

/** Return a directory path unchanged, or the parent path if the input is a file path. */
અંતઃરેખ std::filesystem::path GetDirOrParentPath(અચલ std::filesystem::path& path) {
  જો (path.empty() || std::filesystem::is_directory(path)) {
    પાછા path;
  }

  પાછા path.parent_path();
}

/**
 * Return file_or_directory_path if it names a file. Otherwise, generate a filename from source_file_path's stem and
 * suffix, optionally under file_or_directory_path when it names a directory.
 */
અંતઃરેખ std::string GetPathWithStemSuffix(અચલ std::string& file_or_directory_path,
                                         અચલ std::string& source_file_path,
                                         std::string_view suffix) {
  જો (!file_or_directory_path.empty() && !std::filesystem::is_directory(file_or_directory_path)) {
    પાછા file_or_directory_path;
  }

  અચલ std::filesystem::path source_path = source_file_path;
  અચલ std::string default_filename = source_path.stem().string() + std::string{suffix};
  જો (std::filesystem::is_directory(file_or_directory_path)) {
    std::filesystem::path directory = file_or_directory_path;
    પાછા directory.append(default_filename).string();
  }

  પાછા default_filename;
}

અંતઃરેખ bool IsAbsolutePath(અચલ std::string& path_string) {
#ifdef _WIN32
  અચલ auto path = std::filesystem::path{ToPathString(path_string)};
  પાછા path.is_absolute();
#નહીંતર
  પાછા !path_string.empty() && path_string.front() == '/';
#endif
}

અંતઃરેખ bool IsRelativePathToParentPath(અચલ std::string& path_string) {
#ifdef _WIN32
  અચલ auto path = std::filesystem::path{ToPathString(path_string)};
  અચલ auto relative_path = path.lexically_normal().make_preferred().wstring();
  પાછા relative_path.find(L"..") != std::wstring::npos;
#નહીંતર
  પાછા !path_string.empty() && path_string.find("..") != std::string::npos;
#endif
}

}  // namespace path_utils
}  // namespace onnxruntime
