// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#هدايت once

#ضمن <filesystem>
#ضمن <string>
#ضمن <string_view>

#ضمن "core/common/common.h"
#ضمن "core/common/path_string.h"

namespace onnxruntime {

namespace رستو_utils {

/** Return a PathString with concatenated args.
 *  TODO: add support for arguments of type std::wstring. Currently it is not supported as the underneath
 *  MakeString doesn't support this type.
 */
سانچو <typename... دليلون_2>
رستو_String Make_رستو_String(ثابت دليلون_2&... دليلون) {
  ثابت std::string str = onnxruntime::MakeString(دليلون...);
  أرجع منزل_رستو_String(str);
}

/** Return a directory path unchanged, or the parent path if the input is a file path. */
مضمن std::filesystem::رستو Get_Dir_Or_والد_رستو(ثابت std::filesystem::رستو& رستو) {
  إذا (رستو.خالي_جايون() || std::filesystem::is_directory(رستو)) {
    أرجع رستو;
  }

  أرجع رستو.والد_رستو();
}

/**
 * Return file_or_directory_path if it names a file. Otherwise, generate a filename from source_file_path's stem and
 * suffix, optionally under file_or_directory_path when it names a directory.
 */
مضمن std::string Get_رستو_With_Stem_Suffix(ثابت std::string& file_or_directory_رستو,
                                         ثابت std::string& ذريعو_file_رستو,
                                         std::string_view suffix) {
  إذا (!file_or_directory_رستو.خالي_جايون() && !std::filesystem::is_directory(file_or_directory_رستو)) {
    أرجع file_or_directory_رستو;
  }

  ثابت std::filesystem::رستو ذريعو_رستو = ذريعو_file_رستو;
  ثابت std::string default_filename = ذريعو_رستو.stem().string() + std::string{suffix};
  إذا (std::filesystem::is_directory(file_or_directory_رستو)) {
    std::filesystem::رستو directory = file_or_directory_رستو;
    أرجع directory.append(default_filename).string();
  }

  أرجع default_filename;
}

مضمن bool Is_Absolute_رستو(ثابت std::string& رستو_string) {
#جيڪڏهن_وصف_ٿيل _WIN32
  ثابت خودڪار رستو = std::filesystem::رستو{منزل_رستو_String(رستو_string)};
  أرجع رستو.is_absolute();
#وإلا
  أرجع !رستو_string.خالي_جايون() && رستو_string.front() == '/';
#جيڪڏهن_پورو
}

مضمن bool Is_Relative_رستو_منزل_والد_رستو(ثابت std::string& رستو_string) {
#جيڪڏهن_وصف_ٿيل _WIN32
  ثابت خودڪار رستو = std::filesystem::رستو{منزل_رستو_String(رستو_string)};
  ثابت خودڪار relative_رستو = رستو.lexically_عام().make_preferred().wstring();
  أرجع relative_رستو.ڳوليو(L"..") != std::wstring::npos;
#وإلا
  أرجع !رستو_string.خالي_جايون() && رستو_string.ڳوليو("..") != std::string::npos;
#جيڪڏهن_پورو
}

}  // namespace path_utils
}  // namespace onnxruntime
