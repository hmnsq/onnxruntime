// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#رهنمود once

#ضمن <filesystem>
#ضمن <string>
#ضمن <string_view>

#ضمن "core/common/common.h"
#ضمن "core/common/path_string.h"

namespace onnxruntime {

namespace مسیر_utils {

/** Return a PathString with concatenated args.
 *  TODO: add support for arguments of type std::wstring. Currently it is not supported as the underneath
 *  MakeString doesn't support this type.
 */
قالب <typename... آرگومانها_2>
مسیر_String Make_مسیر_String(ثابت آرگومانها_2&... آرگومانها) {
  ثابت std::string str = onnxruntime::MakeString(آرگومانها...);
  أرجع مقصد_مسیر_String(str);
}

/** Return a directory path unchanged, or the parent path if the input is a file path. */
مضمن std::filesystem::مسیر Get_Dir_Or_والد_مسیر(ثابت std::filesystem::مسیر& مسیر) {
  إذا (مسیر.خالی() || std::filesystem::is_directory(مسیر)) {
    أرجع مسیر;
  }

  أرجع مسیر.والد_مسیر();
}

/**
 * Return file_or_directory_path if it names a file. Otherwise, generate a filename from source_file_path's stem and
 * suffix, optionally under file_or_directory_path when it names a directory.
 */
مضمن std::string Get_مسیر_With_Stem_Suffix(ثابت std::string& file_or_directory_مسیر,
                                         ثابت std::string& منبع_file_مسیر,
                                         std::string_view suffix) {
  إذا (!file_or_directory_مسیر.خالی() && !std::filesystem::is_directory(file_or_directory_مسیر)) {
    أرجع file_or_directory_مسیر;
  }

  ثابت std::filesystem::مسیر منبع_مسیر = منبع_file_مسیر;
  ثابت std::string default_filename = منبع_مسیر.stem().string() + std::string{suffix};
  إذا (std::filesystem::is_directory(file_or_directory_مسیر)) {
    std::filesystem::مسیر directory = file_or_directory_مسیر;
    أرجع directory.append(default_filename).string();
  }

  أرجع default_filename;
}

مضمن bool Is_Absolute_مسیر(ثابت std::string& مسیر_string) {
#اگر_تعریف_شده _WIN32
  ثابت خودکار مسیر = std::filesystem::مسیر{مقصد_مسیر_String(مسیر_string)};
  أرجع مسیر.is_absolute();
#وإلا
  أرجع !مسیر_string.خالی() && مسیر_string.front() == '/';
#پایان_اگر
}

مضمن bool Is_Relative_مسیر_مقصد_والد_مسیر(ثابت std::string& مسیر_string) {
#اگر_تعریف_شده _WIN32
  ثابت خودکار مسیر = std::filesystem::مسیر{مقصد_مسیر_String(مسیر_string)};
  ثابت خودکار relative_مسیر = مسیر.lexically_نرمال().make_preferred().wstring();
  أرجع relative_مسیر.پیدا_کن(L"..") != std::wstring::npos;
#وإلا
  أرجع !مسیر_string.خالی() && مسیر_string.پیدا_کن("..") != std::string::npos;
#پایان_اگر
}

}  // namespace path_utils
}  // namespace onnxruntime
