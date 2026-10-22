// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#ଅନ୍ତର୍ଭାଵ <filesystem>
#ଅନ୍ତର୍ଭାଵ <string>
#ଅନ୍ତର୍ଭାଵ <string_view>

#ଅନ୍ତର୍ଭାଵ "core/common/common.h"
#ଅନ୍ତର୍ଭାଵ "core/common/path_string.h"

namespace onnxruntime {

namespace path_utils {

/** Return a PathString with concatenated args.
 *  TODO: add support for arguments of type std::wstring. Currently it is not supported as the underneath
 *  MakeString doesn't support this type.
 */
template <typename... Args>
PathString MakePathString(ଅଚଳ Args&... args) {
  ଅଚଳ std::string str = onnxruntime::MakeString(args...);
  ଫେରନ୍ତୁ ToPathString(str);
}

/** Return a directory path unchanged, or the parent path if the input is a file path. */
ଧାଡ଼ିଭିତରେ std::filesystem::path GetDirOrParentPath(ଅଚଳ std::filesystem::path& path) {
  ଯଦି (path.empty() || std::filesystem::is_directory(path)) {
    ଫେରନ୍ତୁ path;
  }

  ଫେରନ୍ତୁ path.parent_path();
}

/**
 * Return file_or_directory_path if it names a file. Otherwise, generate a filename from source_file_path's stem and
 * suffix, optionally under file_or_directory_path when it names a directory.
 */
ଧାଡ଼ିଭିତରେ std::string GetPathWithStemSuffix(ଅଚଳ std::string& file_or_directory_path,
                                         ଅଚଳ std::string& source_file_path,
                                         std::string_view suffix) {
  ଯଦି (!file_or_directory_path.empty() && !std::filesystem::is_directory(file_or_directory_path)) {
    ଫେରନ୍ତୁ file_or_directory_path;
  }

  ଅଚଳ std::filesystem::path source_path = source_file_path;
  ଅଚଳ std::string default_filename = source_path.stem().string() + std::string{suffix};
  ଯଦି (std::filesystem::is_directory(file_or_directory_path)) {
    std::filesystem::path directory = file_or_directory_path;
    ଫେରନ୍ତୁ directory.append(default_filename).string();
  }

  ଫେରନ୍ତୁ default_filename;
}

ଧାଡ଼ିଭିତରେ bool IsAbsolutePath(ଅଚଳ std::string& path_string) {
#ifdef _WIN32
  ଅଚଳ auto path = std::filesystem::path{ToPathString(path_string)};
  ଫେରନ୍ତୁ path.is_absolute();
#ଅନ୍ୟଥା
  ଫେରନ୍ତୁ !path_string.empty() && path_string.front() == '/';
#endif
}

ଧାଡ଼ିଭିତରେ bool IsRelativePathToParentPath(ଅଚଳ std::string& path_string) {
#ifdef _WIN32
  ଅଚଳ auto path = std::filesystem::path{ToPathString(path_string)};
  ଅଚଳ auto relative_path = path.lexically_normal().make_preferred().wstring();
  ଫେରନ୍ତୁ relative_path.find(L"..") != std::wstring::npos;
#ଅନ୍ୟଥା
  ଫେରନ୍ତୁ !path_string.empty() && path_string.find("..") != std::string::npos;
#endif
}

}  // namespace path_utils
}  // namespace onnxruntime
