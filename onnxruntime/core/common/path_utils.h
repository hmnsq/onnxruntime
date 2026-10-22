// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ਹਦਾਇਤ once

#ਅਨ੍ਤਰ੍ਭਾਵ <filesystem>
#ਅਨ੍ਤਰ੍ਭਾਵ <string>
#ਅਨ੍ਤਰ੍ਭਾਵ <string_view>

#ਅਨ੍ਤਰ੍ਭਾਵ "core/common/common.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "core/common/path_string.h"

namespace onnxruntime {

namespace ਰਸਤਾ_utils {

/** Return a PathString with concatenated args.
 *  TODO: add support for arguments of type std::wstring. Currently it is not supported as the underneath
 *  MakeString doesn't support this type.
 */
ਸਾਂਚਾ <typename... ਦਲੀਲਾਂ_2>
ਰਸਤਾ_String Make_ਰਸਤਾ_String(ਅਚਲ ਦਲੀਲਾਂ_2&... ਦਲੀਲਾਂ) {
  ਅਚਲ std::string str = onnxruntime::MakeString(ਦਲੀਲਾਂ...);
  ਵਾਪਸ ਮੰਜ਼ਿਲ_ਰਸਤਾ_String(str);
}

/** Return a directory path unchanged, or the parent path if the input is a file path. */
ਸਤਰਅੰਦਰ std::filesystem::ਰਸਤਾ Get_Dir_Or_ਮਾਪਾ_ਰਸਤਾ(ਅਚਲ std::filesystem::ਰਸਤਾ& ਰਸਤਾ) {
  ਜੇ (ਰਸਤਾ.ਖਾਲੀ_ਥਾਵਾਂ() || std::filesystem::is_directory(ਰਸਤਾ)) {
    ਵਾਪਸ ਰਸਤਾ;
  }

  ਵਾਪਸ ਰਸਤਾ.ਮਾਪਾ_ਰਸਤਾ();
}

/**
 * Return file_or_directory_path if it names a file. Otherwise, generate a filename from source_file_path's stem and
 * suffix, optionally under file_or_directory_path when it names a directory.
 */
ਸਤਰਅੰਦਰ std::string Get_ਰਸਤਾ_With_Stem_Suffix(ਅਚਲ std::string& file_or_directory_ਰਸਤਾ,
                                         ਅਚਲ std::string& ਸਰੋਤ_file_ਰਸਤਾ,
                                         std::string_view suffix) {
  ਜੇ (!file_or_directory_ਰਸਤਾ.ਖਾਲੀ_ਥਾਵਾਂ() && !std::filesystem::is_directory(file_or_directory_ਰਸਤਾ)) {
    ਵਾਪਸ file_or_directory_ਰਸਤਾ;
  }

  ਅਚਲ std::filesystem::ਰਸਤਾ ਸਰੋਤ_ਰਸਤਾ = ਸਰੋਤ_file_ਰਸਤਾ;
  ਅਚਲ std::string default_filename = ਸਰੋਤ_ਰਸਤਾ.stem().string() + std::string{suffix};
  ਜੇ (std::filesystem::is_directory(file_or_directory_ਰਸਤਾ)) {
    std::filesystem::ਰਸਤਾ directory = file_or_directory_ਰਸਤਾ;
    ਵਾਪਸ directory.append(default_filename).string();
  }

  ਵਾਪਸ default_filename;
}

ਸਤਰਅੰਦਰ bool Is_Absolute_ਰਸਤਾ(ਅਚਲ std::string& ਰਸਤਾ_string) {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _WIN32
  ਅਚਲ ਸਵੈਚਾਲਿਤ ਰਸਤਾ = std::filesystem::ਰਸਤਾ{ਮੰਜ਼ਿਲ_ਰਸਤਾ_String(ਰਸਤਾ_string)};
  ਵਾਪਸ ਰਸਤਾ.is_absolute();
#ਨਹੀਂਤਾਂ
  ਵਾਪਸ !ਰਸਤਾ_string.ਖਾਲੀ_ਥਾਵਾਂ() && ਰਸਤਾ_string.front() == '/';
#ਜੇ_ਅੰਤ
}

ਸਤਰਅੰਦਰ bool Is_Relative_ਰਸਤਾ_ਮੰਜ਼ਿਲ_ਮਾਪਾ_ਰਸਤਾ(ਅਚਲ std::string& ਰਸਤਾ_string) {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _WIN32
  ਅਚਲ ਸਵੈਚਾਲਿਤ ਰਸਤਾ = std::filesystem::ਰਸਤਾ{ਮੰਜ਼ਿਲ_ਰਸਤਾ_String(ਰਸਤਾ_string)};
  ਅਚਲ ਸਵੈਚਾਲਿਤ relative_ਰਸਤਾ = ਰਸਤਾ.lexically_ਸਾਧਾਰਨ().make_preferred().wstring();
  ਵਾਪਸ relative_ਰਸਤਾ.ਲੱਭੋ(L"..") != std::wstring::npos;
#ਨਹੀਂਤਾਂ
  ਵਾਪਸ !ਰਸਤਾ_string.ਖਾਲੀ_ਥਾਵਾਂ() && ਰਸਤਾ_string.ਲੱਭੋ("..") != std::string::npos;
#ਜੇ_ਅੰਤ
}

}  // namespace path_utils
}  // namespace onnxruntime
