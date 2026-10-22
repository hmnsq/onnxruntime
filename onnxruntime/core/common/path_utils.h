// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#अन्तर्भाव <filesystem>
#अन्तर्भाव <string>
#अन्तर्भाव <string_view>

#अन्तर्भाव "core/common/common.h"
#अन्तर्भाव "core/common/path_string.h"

namespace onnxruntime {

namespace path_utils {

/** Return a PathString with concatenated args.
 *  TODO: add support for arguments of type std::wstring. Currently it is not supported as the underneath
 *  MakeString doesn't support this type.
 */
ढाँचा <typename... Args>
PathString MakePathString(स्थिरांक Args&... args) {
  स्थिरांक std::string str = onnxruntime::MakeString(args...);
  परत ToPathString(str);
}

/** Return a directory path unchanged, or the parent path if the input is a file path. */
ओळीत std::filesystem::path GetDirOrParentPath(स्थिरांक std::filesystem::path& path) {
  जर (path.empty() || std::filesystem::is_directory(path)) {
    परत path;
  }

  परत path.parent_path();
}

/**
 * Return file_or_directory_path if it names a file. Otherwise, generate a filename from source_file_path's stem and
 * suffix, optionally under file_or_directory_path when it names a directory.
 */
ओळीत std::string GetPathWithStemSuffix(स्थिरांक std::string& file_or_directory_path,
                                         स्थिरांक std::string& source_file_path,
                                         std::string_view suffix) {
  जर (!file_or_directory_path.empty() && !std::filesystem::is_directory(file_or_directory_path)) {
    परत file_or_directory_path;
  }

  स्थिरांक std::filesystem::path source_path = source_file_path;
  स्थिरांक std::string default_filename = source_path.stem().string() + std::string{suffix};
  जर (std::filesystem::is_directory(file_or_directory_path)) {
    std::filesystem::path directory = file_or_directory_path;
    परत directory.append(default_filename).string();
  }

  परत default_filename;
}

ओळीत bool IsAbsolutePath(स्थिरांक std::string& path_string) {
#ifdef _WIN32
  स्थिरांक स्वतः path = std::filesystem::path{ToPathString(path_string)};
  परत path.is_absolute();
#अन्यथा
  परत !path_string.empty() && path_string.front() == '/';
#endif
}

ओळीत bool IsRelativePathToParentPath(स्थिरांक std::string& path_string) {
#ifdef _WIN32
  स्थिरांक स्वतः path = std::filesystem::path{ToPathString(path_string)};
  स्थिरांक स्वतः relative_path = path.lexically_normal().make_preferred().wstring();
  परत relative_path.find(L"..") != std::wstring::npos;
#अन्यथा
  परत !path_string.empty() && path_string.find("..") != std::string::npos;
#endif
}

}  // namespace path_utils
}  // namespace onnxruntime
