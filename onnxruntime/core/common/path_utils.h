// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ہدایت once

#شامل <filesystem>
#شامل <string>
#شامل <string_view>

#شامل "core/common/common.h"
#شامل "core/common/path_string.h"

namespace onnxruntime {

namespace راستہ_utils {

/** Return a PathString with concatenated args.
 *  TODO: add support for arguments of type std::wstring. Currently it is not supported as the underneath
 *  MakeString doesn't support this type.
 */
سانچہ <typename... دلائل_2>
راستہ_String Make_راستہ_String(ثابت دلائل_2&... دلائل) {
  ثابت std::string str = onnxruntime::MakeString(دلائل...);
  واپس مقصد_راستہ_String(str);
}

/** Return a directory path unchanged, or the parent path if the input is a file path. */
اندرونی std::filesystem::راستہ Get_Dir_Or_والد_راستہ(ثابت std::filesystem::راستہ& راستہ) {
  اگر (راستہ.خالی_خانے() || std::filesystem::is_directory(راستہ)) {
    واپس راستہ;
  }

  واپس راستہ.والد_راستہ();
}

/**
 * Return file_or_directory_path if it names a file. Otherwise, generate a filename from source_file_path's stem and
 * suffix, optionally under file_or_directory_path when it names a directory.
 */
اندرونی std::string Get_راستہ_With_Stem_Suffix(ثابت std::string& file_or_directory_راستہ,
                                         ثابت std::string& ماخذ_file_راستہ,
                                         std::string_view suffix) {
  اگر (!file_or_directory_راستہ.خالی_خانے() && !std::filesystem::is_directory(file_or_directory_راستہ)) {
    واپس file_or_directory_راستہ;
  }

  ثابت std::filesystem::راستہ ماخذ_راستہ = ماخذ_file_راستہ;
  ثابت std::string default_filename = ماخذ_راستہ.stem().string() + std::string{suffix};
  اگر (std::filesystem::is_directory(file_or_directory_راستہ)) {
    std::filesystem::راستہ directory = file_or_directory_راستہ;
    واپس directory.append(default_filename).string();
  }

  واپس default_filename;
}

اندرونی bool Is_Absolute_راستہ(ثابت std::string& راستہ_string) {
#اگر_تعریف_شدہ _WIN32
  ثابت خودکار راستہ = std::filesystem::راستہ{مقصد_راستہ_String(راستہ_string)};
  واپس راستہ.is_absolute();
#ورنہ
  واپس !راستہ_string.خالی_خانے() && راستہ_string.front() == '/';
#اگر_ختم
}

اندرونی bool Is_Relative_راستہ_مقصد_والد_راستہ(ثابت std::string& راستہ_string) {
#اگر_تعریف_شدہ _WIN32
  ثابت خودکار راستہ = std::filesystem::راستہ{مقصد_راستہ_String(راستہ_string)};
  ثابت خودکار relative_راستہ = راستہ.lexically_معمول().make_preferred().wstring();
  واپس relative_راستہ.ڈھونڈو(L"..") != std::wstring::npos;
#ورنہ
  واپس !راستہ_string.خالی_خانے() && راستہ_string.ڈھونڈو("..") != std::string::npos;
#اگر_ختم
}

}  // namespace path_utils
}  // namespace onnxruntime
