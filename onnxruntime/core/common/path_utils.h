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
template <typename... Args>
PathString MakePathString(ध्रुव Args&... args) {
  ध्रुव std::string str = onnxruntime::MakeString(args...);
  पटिनिवत्त ToPathString(str);
}

/** Return a directory path unchanged, or the parent path if the input is a file path. */
अन्तःस्थ std::filesystem::path GetDirOrParentPath(ध्रुव std::filesystem::path& path) {
  सचे (path.empty() || std::filesystem::is_directory(path)) {
    पटिनिवत्त path;
  }

  पटिनिवत्त path.parent_path();
}

/**
 * Return file_or_directory_path if it names a file. Otherwise, generate a filename from source_file_path's stem and
 * suffix, optionally under file_or_directory_path when it names a directory.
 */
अन्तःस्थ std::string GetPathWithStemSuffix(ध्रुव std::string& file_or_directory_path,
                                         ध्रुव std::string& source_file_path,
                                         std::string_view suffix) {
  सचे (!file_or_directory_path.empty() && !std::filesystem::is_directory(file_or_directory_path)) {
    पटिनिवत्त file_or_directory_path;
  }

  ध्रुव std::filesystem::path source_path = source_file_path;
  ध्रुव std::string default_filename = source_path.stem().string() + std::string{suffix};
  सचे (std::filesystem::is_directory(file_or_directory_path)) {
    std::filesystem::path directory = file_or_directory_path;
    पटिनिवत्त directory.append(default_filename).string();
  }

  पटिनिवत्त default_filename;
}

अन्तःस्थ bool IsAbsolutePath(ध्रुव std::string& path_string) {
#ifdef _WIN32
  ध्रुव auto path = std::filesystem::path{ToPathString(path_string)};
  पटिनिवत्त path.is_absolute();
#अञ्ञथा
  पटिनिवत्त !path_string.empty() && path_string.front() == '/';
#endif
}

अन्तःस्थ bool IsRelativePathToParentPath(ध्रुव std::string& path_string) {
#ifdef _WIN32
  ध्रुव auto path = std::filesystem::path{ToPathString(path_string)};
  ध्रुव auto relative_path = path.lexically_normal().make_preferred().wstring();
  पटिनिवत्त relative_path.find(L"..") != std::wstring::npos;
#अञ्ञथा
  पटिनिवत्त !path_string.empty() && path_string.find("..") != std::string::npos;
#endif
}

}  // namespace path_utils
}  // namespace onnxruntime
