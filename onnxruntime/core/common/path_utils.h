// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#அந்தர்பாவ <filesystem>
#அந்தர்பாவ <string>
#அந்தர்பாவ <string_view>

#அந்தர்பாவ "core/common/common.h"
#அந்தர்பாவ "core/common/path_string.h"

namespace onnxruntime {

namespace path_utils {

/** Return a PathString with concatenated args.
 *  TODO: add support for arguments of type std::wstring. Currently it is not supported as the underneath
 *  MakeString doesn't support this type.
 */
template <typename... Args>
PathString MakePathString(மாறிலி Args&... args) {
  மாறிலி std::string str = onnxruntime::MakeString(args...);
  திரும்பு ToPathString(str);
}

/** Return a directory path unchanged, or the parent path if the input is a file path. */
உள்வரி std::filesystem::path GetDirOrParentPath(மாறிலி std::filesystem::path& path) {
  எனில் (path.empty() || std::filesystem::is_directory(path)) {
    திரும்பு path;
  }

  திரும்பு path.parent_path();
}

/**
 * Return file_or_directory_path if it names a file. Otherwise, generate a filename from source_file_path's stem and
 * suffix, optionally under file_or_directory_path when it names a directory.
 */
உள்வரி std::string GetPathWithStemSuffix(மாறிலி std::string& file_or_directory_path,
                                         மாறிலி std::string& source_file_path,
                                         std::string_view suffix) {
  எனில் (!file_or_directory_path.empty() && !std::filesystem::is_directory(file_or_directory_path)) {
    திரும்பு file_or_directory_path;
  }

  மாறிலி std::filesystem::path source_path = source_file_path;
  மாறிலி std::string default_filename = source_path.stem().string() + std::string{suffix};
  எனில் (std::filesystem::is_directory(file_or_directory_path)) {
    std::filesystem::path directory = file_or_directory_path;
    திரும்பு directory.append(default_filename).string();
  }

  திரும்பு default_filename;
}

உள்வரி bool IsAbsolutePath(மாறிலி std::string& path_string) {
#ifdef _WIN32
  மாறிலி auto path = std::filesystem::path{ToPathString(path_string)};
  திரும்பு path.is_absolute();
#இல்லையெனில்
  திரும்பு !path_string.empty() && path_string.front() == '/';
#endif
}

உள்வரி bool IsRelativePathToParentPath(மாறிலி std::string& path_string) {
#ifdef _WIN32
  மாறிலி auto path = std::filesystem::path{ToPathString(path_string)};
  மாறிலி auto relative_path = path.lexically_normal().make_preferred().wstring();
  திரும்பு relative_path.find(L"..") != std::wstring::npos;
#இல்லையெனில்
  திரும்பு !path_string.empty() && path_string.find("..") != std::string::npos;
#endif
}

}  // namespace path_utils
}  // namespace onnxruntime
