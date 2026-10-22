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
PathString MakePathString(अचर Args&... args) {
  अचर std::string str = onnxruntime::MakeString(args...);
  फर्काऊ ToPathString(str);
}

/** Return a directory path unchanged, or the parent path if the input is a file path. */
अन्तःस्थ std::filesystem::path GetDirOrParentPath(अचर std::filesystem::path& path) {
  यदि (path.empty() || std::filesystem::is_directory(path)) {
    फर्काऊ path;
  }

  फर्काऊ path.parent_path();
}

/**
 * Return file_or_directory_path if it names a file. Otherwise, generate a filename from source_file_path's stem and
 * suffix, optionally under file_or_directory_path when it names a directory.
 */
अन्तःस्थ std::string GetPathWithStemSuffix(अचर std::string& file_or_directory_path,
                                         अचर std::string& source_file_path,
                                         std::string_view suffix) {
  यदि (!file_or_directory_path.empty() && !std::filesystem::is_directory(file_or_directory_path)) {
    फर्काऊ file_or_directory_path;
  }

  अचर std::filesystem::path source_path = source_file_path;
  अचर std::string default_filename = source_path.stem().string() + std::string{suffix};
  यदि (std::filesystem::is_directory(file_or_directory_path)) {
    std::filesystem::path directory = file_or_directory_path;
    फर्काऊ directory.append(default_filename).string();
  }

  फर्काऊ default_filename;
}

अन्तःस्थ bool IsAbsolutePath(अचर std::string& path_string) {
#ifdef _WIN32
  अचर स्वतः path = std::filesystem::path{ToPathString(path_string)};
  फर्काऊ path.is_absolute();
#अन्यथा
  फर्काऊ !path_string.empty() && path_string.front() == '/';
#endif
}

अन्तःस्थ bool IsRelativePathToParentPath(अचर std::string& path_string) {
#ifdef _WIN32
  अचर स्वतः path = std::filesystem::path{ToPathString(path_string)};
  अचर स्वतः relative_path = path.lexically_normal().make_preferred().wstring();
  फर्काऊ relative_path.find(L"..") != std::wstring::npos;
#अन्यथा
  फर्काऊ !path_string.empty() && path_string.find("..") != std::string::npos;
#endif
}

}  // namespace path_utils
}  // namespace onnxruntime
