// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#لارښوونه once

#ضمن <filesystem>
#ضمن <string>
#ضمن <string_view>

#ضمن "core/common/common.h"
#ضمن "core/common/path_string.h"

namespace onnxruntime {

namespace لار_utils {

/** Return a PathString with concatenated args.
 *  TODO: add support for arguments of type std::wstring. Currently it is not supported as the underneath
 *  MakeString doesn't support this type.
 */
کينډۍ <typename... دلیلونه_2>
لار_String Make_لار_String(ثابت دلیلونه_2&... دلیلونه) {
  ثابت std::string str = onnxruntime::MakeString(دلیلونه...);
  أرجع منزل_لار_String(str);
}

/** Return a directory path unchanged, or the parent path if the input is a file path. */
مضمن std::filesystem::لار Get_Dir_Or_پلار_لار(ثابت std::filesystem::لار& لار) {
  إذا (لار.تشه() || std::filesystem::is_directory(لار)) {
    أرجع لار;
  }

  أرجع لار.پلار_لار();
}

/**
 * Return file_or_directory_path if it names a file. Otherwise, generate a filename from source_file_path's stem and
 * suffix, optionally under file_or_directory_path when it names a directory.
 */
مضمن std::string Get_لار_With_Stem_Suffix(ثابت std::string& file_or_directory_لار,
                                         ثابت std::string& سرچینه_file_لار,
                                         std::string_view suffix) {
  إذا (!file_or_directory_لار.تشه() && !std::filesystem::is_directory(file_or_directory_لار)) {
    أرجع file_or_directory_لار;
  }

  ثابت std::filesystem::لار سرچینه_لار = سرچینه_file_لار;
  ثابت std::string default_filename = سرچینه_لار.stem().string() + std::string{suffix};
  إذا (std::filesystem::is_directory(file_or_directory_لار)) {
    std::filesystem::لار directory = file_or_directory_لار;
    أرجع directory.append(default_filename).string();
  }

  أرجع default_filename;
}

مضمن bool Is_Absolute_لار(ثابت std::string& لار_string) {
#که_تعریف_شوی _WIN32
  ثابت خپلکاری لار = std::filesystem::لار{منزل_لار_String(لار_string)};
  أرجع لار.is_absolute();
#وإلا
  أرجع !لار_string.تشه() && لار_string.front() == '/';
#د_که_پای
}

مضمن bool Is_Relative_لار_منزل_پلار_لار(ثابت std::string& لار_string) {
#که_تعریف_شوی _WIN32
  ثابت خپلکاری لار = std::filesystem::لار{منزل_لار_String(لار_string)};
  ثابت خپلکاری relative_لار = لار.lexically_نورمال().make_preferred().wstring();
  أرجع relative_لار.پیدا_کړه(L"..") != std::wstring::npos;
#وإلا
  أرجع !لار_string.تشه() && لار_string.پیدا_کړه("..") != std::string::npos;
#د_که_پای
}

}  // namespace path_utils
}  // namespace onnxruntime
