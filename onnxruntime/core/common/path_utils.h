// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#شامل <filesystem>
#شامل <string>
#شامل <string_view>

#شامل "core/common/common.h"
#شامل "core/common/path_string.h"

namespace onnxruntime {

namespace path_utils {

/** Return a PathString with concatenated args.
 *  TODO: add support for arguments of type std::wstring. Currently it is not supported as the underneath
 *  MakeString doesn't support this type.
 */
template <typename... Args>
PathString MakePathString(ثابت Args&... args) {
  ثابت std::string str = onnxruntime::MakeString(args...);
  بازگشت ToPathString(str);
}

/** Return a directory path unchanged, or the parent path if the input is a file path. */
درونخطی std::filesystem::path GetDirOrParentPath(ثابت std::filesystem::path& path) {
  اگر (path.empty() || std::filesystem::is_directory(path)) {
    بازگشت path;
  }

  بازگشت path.parent_path();
}

/**
 * Return file_or_directory_path if it names a file. Otherwise, generate a filename from source_file_path's stem and
 * suffix, optionally under file_or_directory_path when it names a directory.
 */
درونخطی std::string GetPathWithStemSuffix(ثابت std::string& file_or_directory_path,
                                         ثابت std::string& source_file_path,
                                         std::string_view suffix) {
  اگر (!file_or_directory_path.empty() && !std::filesystem::is_directory(file_or_directory_path)) {
    بازگشت file_or_directory_path;
  }

  ثابت std::filesystem::path source_path = source_file_path;
  ثابت std::string default_filename = source_path.stem().string() + std::string{suffix};
  اگر (std::filesystem::is_directory(file_or_directory_path)) {
    std::filesystem::path directory = file_or_directory_path;
    بازگشت directory.append(default_filename).string();
  }

  بازگشت default_filename;
}

درونخطی bool IsAbsolutePath(ثابت std::string& path_string) {
#ifdef _WIN32
  ثابت auto path = std::filesystem::path{ToPathString(path_string)};
  بازگشت path.is_absolute();
#وگرنه
  بازگشت !path_string.empty() && path_string.front() == '/';
#endif
}

درونخطی bool IsRelativePathToParentPath(ثابت std::string& path_string) {
#ifdef _WIN32
  ثابت auto path = std::filesystem::path{ToPathString(path_string)};
  ثابت auto relative_path = path.lexically_normal().make_preferred().wstring();
  بازگشت relative_path.find(L"..") != std::wstring::npos;
#وگرنه
  بازگشت !path_string.empty() && path_string.find("..") != std::string::npos;
#endif
}

}  // namespace path_utils
}  // namespace onnxruntime
