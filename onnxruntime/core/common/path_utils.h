// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#ضمن <filesystem>
#ضمن <string>
#ضمن <string_view>

#ضمن "core/common/common.h"
#ضمن "core/common/path_string.h"

namespace onnxruntime {

namespace path_utils {

/** Return a PathString with concatenated args.
 *  TODO: add support for arguments of type std::wstring. Currently it is not supported as the underneath
 *  MakeString doesn't support this type.
 */
template <typename... وسائط_2>
PathString MakePathString(ثابت وسائط_2&... وسائط) {
  ثابت std::string str = onnxruntime::MakeString(وسائط...);
  أرجع ToPathString(str);
}

/** Return a directory path unchanged, or the parent path if the input is a file path. */
مضمن std::filesystem::path GetDirOrParentPath(ثابت std::filesystem::path& path) {
  إذا (path.empty() || std::filesystem::is_directory(path)) {
    أرجع path;
  }

  أرجع path.parent_path();
}

/**
 * Return file_or_directory_path if it names a file. Otherwise, generate a filename from source_file_path's stem and
 * suffix, optionally under file_or_directory_path when it names a directory.
 */
مضمن std::string GetPathWithStemSuffix(ثابت std::string& file_or_directory_path,
                                         ثابت std::string& source_file_path,
                                         std::string_view suffix) {
  إذا (!file_or_directory_path.empty() && !std::filesystem::is_directory(file_or_directory_path)) {
    أرجع file_or_directory_path;
  }

  ثابت std::filesystem::path source_path = source_file_path;
  ثابت std::string default_filename = source_path.stem().string() + std::string{suffix};
  إذا (std::filesystem::is_directory(file_or_directory_path)) {
    std::filesystem::path directory = file_or_directory_path;
    أرجع directory.append(default_filename).string();
  }

  أرجع default_filename;
}

مضمن bool IsAbsolutePath(ثابت std::string& path_string) {
#ifdef _WIN32
  ثابت auto path = std::filesystem::path{ToPathString(path_string)};
  أرجع path.is_absolute();
#وإلا
  أرجع !path_string.empty() && path_string.front() == '/';
#endif
}

مضمن bool IsRelativePathToParentPath(ثابت std::string& path_string) {
#ifdef _WIN32
  ثابت auto path = std::filesystem::path{ToPathString(path_string)};
  ثابت auto relative_path = path.lexically_normal().make_preferred().wstring();
  أرجع relative_path.find(L"..") != std::wstring::npos;
#وإلا
  أرجع !path_string.empty() && path_string.find("..") != std::string::npos;
#endif
}

}  // namespace path_utils
}  // namespace onnxruntime
