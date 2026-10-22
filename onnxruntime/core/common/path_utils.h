// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#निर्देशः once

#अन्तर्भाव <filesystem>
#अन्तर्भाव <string>
#अन्तर्भाव <string_view>

#अन्तर्भाव "core/common/common.h"
#अन्तर्भाव "core/common/path_string.h"

namespace onnxruntime {

namespace मार्गः_utils {

/** Return a PathString with concatenated args.
 *  TODO: add support for arguments of type std::wstring. Currently it is not supported as the underneath
 *  MakeString doesn't support this type.
 */
प्रतिरूपम् <typename... तर्काः_2>
मार्गः_String Make_मार्गः_String(ध्रुव तर्काः_2&... तर्काः) {
  ध्रुव std::string str = onnxruntime::MakeString(तर्काः...);
  प्रत्यागम गन्तव्यम्_मार्गः_String(str);
}

/** Return a directory path unchanged, or the parent path if the input is a file path. */
अन्तःस्थ std::filesystem::मार्गः Get_Dir_Or_Parent_मार्गः(ध्रुव std::filesystem::मार्गः& मार्गः) {
  यदि (मार्गः.रिक्तानि() || std::filesystem::is_directory(मार्गः)) {
    प्रत्यागम मार्गः;
  }

  प्रत्यागम मार्गः.parent_मार्गः();
}

/**
 * Return file_or_directory_path if it names a file. Otherwise, generate a filename from source_file_path's stem and
 * suffix, optionally under file_or_directory_path when it names a directory.
 */
अन्तःस्थ std::string Get_मार्गः_With_Stem_Suffix(ध्रुव std::string& file_or_directory_मार्गः,
                                         ध्रुव std::string& source_file_मार्गः,
                                         std::string_view suffix) {
  यदि (!file_or_directory_मार्गः.रिक्तानि() && !std::filesystem::is_directory(file_or_directory_मार्गः)) {
    प्रत्यागम file_or_directory_मार्गः;
  }

  ध्रुव std::filesystem::मार्गः source_मार्गः = source_file_मार्गः;
  ध्रुव std::string default_filename = source_मार्गः.stem().string() + std::string{suffix};
  यदि (std::filesystem::is_directory(file_or_directory_मार्गः)) {
    std::filesystem::मार्गः directory = file_or_directory_मार्गः;
    प्रत्यागम directory.append(default_filename).string();
  }

  प्रत्यागम default_filename;
}

अन्तःस्थ bool Is_Absolute_मार्गः(ध्रुव std::string& मार्गः_string) {
#यदि_परिभाषितम् _WIN32
  ध्रुव स्वचालित मार्गः = std::filesystem::मार्गः{गन्तव्यम्_मार्गः_String(मार्गः_string)};
  प्रत्यागम मार्गः.is_absolute();
#अन्यथा
  प्रत्यागम !मार्गः_string.रिक्तानि() && मार्गः_string.front() == '/';
#यदि_अन्तः
}

अन्तःस्थ bool Is_Relative_मार्गः_गन्तव्यम्_Parent_मार्गः(ध्रुव std::string& मार्गः_string) {
#यदि_परिभाषितम् _WIN32
  ध्रुव स्वचालित मार्गः = std::filesystem::मार्गः{गन्तव्यम्_मार्गः_String(मार्गः_string)};
  ध्रुव स्वचालित relative_मार्गः = मार्गः.lexically_प्रसामान्यम्().make_preferred().wstring();
  प्रत्यागम relative_मार्गः.find(L"..") != std::wstring::npos;
#अन्यथा
  प्रत्यागम !मार्गः_string.रिक्तानि() && मार्गः_string.find("..") != std::string::npos;
#यदि_अन्तः
}

}  // namespace path_utils
}  // namespace onnxruntime
