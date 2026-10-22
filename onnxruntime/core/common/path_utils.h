// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#ಅನ್ತರ್ಭಾವ <filesystem>
#ಅನ್ತರ್ಭಾವ <string>
#ಅನ್ತರ್ಭಾವ <string_view>

#ಅನ್ತರ್ಭಾವ "core/common/common.h"
#ಅನ್ತರ್ಭಾವ "core/common/path_string.h"

namespace onnxruntime {

namespace path_utils {

/** Return a PathString with concatenated args.
 *  TODO: add support for arguments of type std::wstring. Currently it is not supported as the underneath
 *  MakeString doesn't support this type.
 */
template <typename... Args>
PathString MakePathString(ಧ್ರುವ Args&... args) {
  ಧ್ರುವ std::string str = onnxruntime::MakeString(args...);
  ಪ್ರತ್ಯಾಗಮ ToPathString(str);
}

/** Return a directory path unchanged, or the parent path if the input is a file path. */
ಅನ್ತಃಸ್ಥ std::filesystem::path GetDirOrParentPath(ಧ್ರುವ std::filesystem::path& path) {
  ಯದಿ (path.empty() || std::filesystem::is_directory(path)) {
    ಪ್ರತ್ಯಾಗಮ path;
  }

  ಪ್ರತ್ಯಾಗಮ path.parent_path();
}

/**
 * Return file_or_directory_path if it names a file. Otherwise, generate a filename from source_file_path's stem and
 * suffix, optionally under file_or_directory_path when it names a directory.
 */
ಅನ್ತಃಸ್ಥ std::string GetPathWithStemSuffix(ಧ್ರುವ std::string& file_or_directory_path,
                                         ಧ್ರುವ std::string& source_file_path,
                                         std::string_view suffix) {
  ಯದಿ (!file_or_directory_path.empty() && !std::filesystem::is_directory(file_or_directory_path)) {
    ಪ್ರತ್ಯಾಗಮ file_or_directory_path;
  }

  ಧ್ರುವ std::filesystem::path source_path = source_file_path;
  ಧ್ರುವ std::string default_filename = source_path.stem().string() + std::string{suffix};
  ಯದಿ (std::filesystem::is_directory(file_or_directory_path)) {
    std::filesystem::path directory = file_or_directory_path;
    ಪ್ರತ್ಯಾಗಮ directory.append(default_filename).string();
  }

  ಪ್ರತ್ಯಾಗಮ default_filename;
}

ಅನ್ತಃಸ್ಥ bool IsAbsolutePath(ಧ್ರುವ std::string& path_string) {
#ifdef _WIN32
  ಧ್ರುವ auto path = std::filesystem::path{ToPathString(path_string)};
  ಪ್ರತ್ಯಾಗಮ path.is_absolute();
#ಅನ್ಯಥಾ
  ಪ್ರತ್ಯಾಗಮ !path_string.empty() && path_string.front() == '/';
#endif
}

ಅನ್ತಃಸ್ಥ bool IsRelativePathToParentPath(ಧ್ರುವ std::string& path_string) {
#ifdef _WIN32
  ಧ್ರುವ auto path = std::filesystem::path{ToPathString(path_string)};
  ಧ್ರುವ auto relative_path = path.lexically_normal().make_preferred().wstring();
  ಪ್ರತ್ಯಾಗಮ relative_path.find(L"..") != std::wstring::npos;
#ಅನ್ಯಥಾ
  ಪ್ರತ್ಯಾಗಮ !path_string.empty() && path_string.find("..") != std::string::npos;
#endif
}

}  // namespace path_utils
}  // namespace onnxruntime
