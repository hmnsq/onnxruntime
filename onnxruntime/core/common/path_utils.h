// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#आशय once

#समावेश <filesystem>
#समावेश <string>
#समावेश <string_view>

#समावेश "core/common/common.h"
#समावेश "core/common/path_string.h"

namespace onnxruntime {

namespace path_utils {

/** Return a PathString with concatenated args.
 *  TODO: add support for arguments of type std::wstring. Currently it is not supported as the underneath
 *  MakeString doesn't support this type.
 */
ढाँचा <typename... Args>
PathString MakePathString(स्थिर Args&... args) {
  स्थिर std::string str = onnxruntime::MakeString(args...);
  वापस ToPathString(str);
}

/** Return a directory path unchanged, or the parent path if the input is a file path. */
अंतरभूत std::filesystem::path GetDirOrParentPath(स्थिर std::filesystem::path& path) {
  अगर (path.empty() || std::filesystem::is_directory(path)) {
    वापस path;
  }

  वापस path.parent_path();
}

/**
 * Return file_or_directory_path if it names a file. Otherwise, generate a filename from source_file_path's stem and
 * suffix, optionally under file_or_directory_path when it names a directory.
 */
अंतरभूत std::string GetPathWithStemSuffix(स्थिर std::string& file_or_directory_path,
                                         स्थिर std::string& source_file_path,
                                         std::string_view suffix) {
  अगर (!file_or_directory_path.empty() && !std::filesystem::is_directory(file_or_directory_path)) {
    वापस file_or_directory_path;
  }

  स्थिर std::filesystem::path source_path = source_file_path;
  स्थिर std::string default_filename = source_path.stem().string() + std::string{suffix};
  अगर (std::filesystem::is_directory(file_or_directory_path)) {
    std::filesystem::path directory = file_or_directory_path;
    वापस directory.append(default_filename).string();
  }

  वापस default_filename;
}

अंतरभूत bool IsAbsolutePath(स्थिर std::string& path_string) {
#अगर_घोषित _WIN32
  स्थिर स्वतः path = std::filesystem::path{ToPathString(path_string)};
  वापस path.is_absolute();
#अन्यथा
  वापस !path_string.empty() && path_string.front() == '/';
#पूर्ण_अगर
}

अंतरभूत bool IsRelativePathToParentPath(स्थिर std::string& path_string) {
#अगर_घोषित _WIN32
  स्थिर स्वतः path = std::filesystem::path{ToPathString(path_string)};
  स्थिर स्वतः relative_path = path.lexically_normal().make_preferred().wstring();
  वापस relative_path.find(L"..") != std::wstring::npos;
#अन्यथा
  वापस !path_string.empty() && path_string.find("..") != std::string::npos;
#पूर्ण_अगर
}

}  // namespace path_utils
}  // namespace onnxruntime
