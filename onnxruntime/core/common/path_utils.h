// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#অন্তর্ভাৱ <filesystem>
#অন্তর্ভাৱ <string>
#অন্তর্ভাৱ <string_view>

#অন্তর্ভাৱ "core/common/common.h"
#অন্তর্ভাৱ "core/common/path_string.h"

namespace onnxruntime {

namespace path_utils {

/** Return a PathString with concatenated args.
 *  TODO: add support for arguments of type std::wstring. Currently it is not supported as the underneath
 *  MakeString doesn't support this type.
 */
template <typename... Args>
PathString MakePathString(ধ্রুৱ Args&... args) {
  ধ্রুৱ std::string str = onnxruntime::MakeString(args...);
  ঘূৰাই ToPathString(str);
}

/** Return a directory path unchanged, or the parent path if the input is a file path. */
অন্তঃস্থ std::filesystem::path GetDirOrParentPath(ধ্রুৱ std::filesystem::path& path) {
  যদি (path.empty() || std::filesystem::is_directory(path)) {
    ঘূৰাই path;
  }

  ঘূৰাই path.parent_path();
}

/**
 * Return file_or_directory_path if it names a file. Otherwise, generate a filename from source_file_path's stem and
 * suffix, optionally under file_or_directory_path when it names a directory.
 */
অন্তঃস্থ std::string GetPathWithStemSuffix(ধ্রুৱ std::string& file_or_directory_path,
                                         ধ্রুৱ std::string& source_file_path,
                                         std::string_view suffix) {
  যদি (!file_or_directory_path.empty() && !std::filesystem::is_directory(file_or_directory_path)) {
    ঘূৰাই file_or_directory_path;
  }

  ধ্রুৱ std::filesystem::path source_path = source_file_path;
  ধ্রুৱ std::string default_filename = source_path.stem().string() + std::string{suffix};
  যদি (std::filesystem::is_directory(file_or_directory_path)) {
    std::filesystem::path directory = file_or_directory_path;
    ঘূৰাই directory.append(default_filename).string();
  }

  ঘূৰাই default_filename;
}

অন্তঃস্থ bool IsAbsolutePath(ধ্রুৱ std::string& path_string) {
#ifdef _WIN32
  ধ্রুৱ auto path = std::filesystem::path{ToPathString(path_string)};
  ঘূৰাই path.is_absolute();
#অন্যথা
  ঘূৰাই !path_string.empty() && path_string.front() == '/';
#endif
}

অন্তঃস্থ bool IsRelativePathToParentPath(ধ্রুৱ std::string& path_string) {
#ifdef _WIN32
  ধ্রুৱ auto path = std::filesystem::path{ToPathString(path_string)};
  ধ্রুৱ auto relative_path = path.lexically_normal().make_preferred().wstring();
  ঘূৰাই relative_path.find(L"..") != std::wstring::npos;
#অন্যথা
  ঘূৰাই !path_string.empty() && path_string.find("..") != std::string::npos;
#endif
}

}  // namespace path_utils
}  // namespace onnxruntime
