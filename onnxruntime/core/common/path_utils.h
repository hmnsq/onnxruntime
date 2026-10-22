// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#അന്തര്ഭാവ <filesystem>
#അന്തര്ഭാവ <string>
#അന്തര്ഭാവ <string_view>

#അന്തര്ഭാവ "core/common/common.h"
#അന്തര്ഭാവ "core/common/path_string.h"

namespace onnxruntime {

namespace path_utils {

/** Return a PathString with concatenated args.
 *  TODO: add support for arguments of type std::wstring. Currently it is not supported as the underneath
 *  MakeString doesn't support this type.
 */
template <typename... Args>
PathString MakePathString(സ്ഥിരം Args&... args) {
  സ്ഥിരം std::string str = onnxruntime::MakeString(args...);
  തിരികെ ToPathString(str);
}

/** Return a directory path unchanged, or the parent path if the input is a file path. */
വരിയിൽ std::filesystem::path GetDirOrParentPath(സ്ഥിരം std::filesystem::path& path) {
  എങ്കിൽ (path.empty() || std::filesystem::is_directory(path)) {
    തിരികെ path;
  }

  തിരികെ path.parent_path();
}

/**
 * Return file_or_directory_path if it names a file. Otherwise, generate a filename from source_file_path's stem and
 * suffix, optionally under file_or_directory_path when it names a directory.
 */
വരിയിൽ std::string GetPathWithStemSuffix(സ്ഥിരം std::string& file_or_directory_path,
                                         സ്ഥിരം std::string& source_file_path,
                                         std::string_view suffix) {
  എങ്കിൽ (!file_or_directory_path.empty() && !std::filesystem::is_directory(file_or_directory_path)) {
    തിരികെ file_or_directory_path;
  }

  സ്ഥിരം std::filesystem::path source_path = source_file_path;
  സ്ഥിരം std::string default_filename = source_path.stem().string() + std::string{suffix};
  എങ്കിൽ (std::filesystem::is_directory(file_or_directory_path)) {
    std::filesystem::path directory = file_or_directory_path;
    തിരികെ directory.append(default_filename).string();
  }

  തിരികെ default_filename;
}

വരിയിൽ bool IsAbsolutePath(സ്ഥിരം std::string& path_string) {
#ifdef _WIN32
  സ്ഥിരം auto path = std::filesystem::path{ToPathString(path_string)};
  തിരികെ path.is_absolute();
#അല്ലെങ്കിൽ
  തിരികെ !path_string.empty() && path_string.front() == '/';
#endif
}

വരിയിൽ bool IsRelativePathToParentPath(സ്ഥിരം std::string& path_string) {
#ifdef _WIN32
  സ്ഥിരം auto path = std::filesystem::path{ToPathString(path_string)};
  സ്ഥിരം auto relative_path = path.lexically_normal().make_preferred().wstring();
  തിരികെ relative_path.find(L"..") != std::wstring::npos;
#അല്ലെങ്കിൽ
  തിരികെ !path_string.empty() && path_string.find("..") != std::string::npos;
#endif
}

}  // namespace path_utils
}  // namespace onnxruntime
