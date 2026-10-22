// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#অন্তর্ভাব <filesystem>
#অন্তর্ভাব <string>
#অন্তর্ভাব <string_view>

#অন্তর্ভাব "core/common/common.h"
#অন্তর্ভাব "core/common/path_string.h"

namespace onnxruntime {

namespace path_utils {

/** Return a PathString with concatenated args.
 *  TODO: add support for arguments of type std::wstring. Currently it is not supported as the underneath
 *  MakeString doesn't support this type.
 */
template <typename... Args>
PathString MakePathString(ধ্রুবক Args&... args) {
  ধ্রুবক std::string str = onnxruntime::MakeString(args...);
  ফেরত ToPathString(str);
}

/** Return a directory path unchanged, or the parent path if the input is a file path. */
অন্তঃরেখ std::filesystem::path GetDirOrParentPath(ধ্রুবক std::filesystem::path& path) {
  যদি (path.empty() || std::filesystem::is_directory(path)) {
    ফেরত path;
  }

  ফেরত path.parent_path();
}

/**
 * Return file_or_directory_path if it names a file. Otherwise, generate a filename from source_file_path's stem and
 * suffix, optionally under file_or_directory_path when it names a directory.
 */
অন্তঃরেখ std::string GetPathWithStemSuffix(ধ্রুবক std::string& file_or_directory_path,
                                         ধ্রুবক std::string& source_file_path,
                                         std::string_view suffix) {
  যদি (!file_or_directory_path.empty() && !std::filesystem::is_directory(file_or_directory_path)) {
    ফেরত file_or_directory_path;
  }

  ধ্রুবক std::filesystem::path source_path = source_file_path;
  ধ্রুবক std::string default_filename = source_path.stem().string() + std::string{suffix};
  যদি (std::filesystem::is_directory(file_or_directory_path)) {
    std::filesystem::path directory = file_or_directory_path;
    ফেরত directory.append(default_filename).string();
  }

  ফেরত default_filename;
}

অন্তঃরেখ bool IsAbsolutePath(ধ্রুবক std::string& path_string) {
#ifdef _WIN32
  ধ্রুবক auto path = std::filesystem::path{ToPathString(path_string)};
  ফেরত path.is_absolute();
#অন্যথা
  ফেরত !path_string.empty() && path_string.front() == '/';
#endif
}

অন্তঃরেখ bool IsRelativePathToParentPath(ধ্রুবক std::string& path_string) {
#ifdef _WIN32
  ধ্রুবক auto path = std::filesystem::path{ToPathString(path_string)};
  ধ্রুবক auto relative_path = path.lexically_normal().make_preferred().wstring();
  ফেরত relative_path.find(L"..") != std::wstring::npos;
#অন্যথা
  ফেরত !path_string.empty() && path_string.find("..") != std::string::npos;
#endif
}

}  // namespace path_utils
}  // namespace onnxruntime
