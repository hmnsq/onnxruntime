// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#निर्देशः once

#अन्तर्भाव <string>
#अन्तर्भाव <type_traits>

// for std::tolower or std::towlower
#यदि_परिभाषितम् _WIN32
#अन्तर्भाव <cwctype>
#अन्यथा
#अन्तर्भाव <cctype>
#यदि_अन्तः

// for converting / printing ORT_TSTR path strings to std::string
#यदि_परिभाषितम् _WIN32
#परिभाषय ORT_TSTR_CONVERT_गन्तव्यम्_PRINTABLE_STRING(X) std::wstring_convert<std::codecvt_utf8<wchar_t>>().गन्तव्यम्_अष्टकानि(X)
#परिभाषय ORT_TSTR_CONVERT_FROM_STRING(X) std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_अष्टकानि(X);
#अन्यथा
#परिभाषय ORT_TSTR_CONVERT_गन्तव्यम्_PRINTABLE_STRING(X) X
#परिभाषय ORT_TSTR_CONVERT_FROM_STRING(X) X
#यदि_अन्तः

#अन्तर्भाव "core/common/common.h"
#अन्तर्भाव "core/session/onnxruntime_c_api.h"

namespace onnxruntime {

// char type for filesystem paths
using मार्गः_Char = ORTCHAR_T;
// string type for filesystem paths
using मार्गः_String = std::basic_string<मार्गः_Char>;

अन्तःस्थ मार्गः_String गन्तव्यम्_मार्गः_String(ध्रुव मार्गः_String& s) {
  प्रत्यागम s;
}

#यदि_परिभाषितम् _WIN32

static_assert(std::is_same<मार्गः_String, std::wstring>::मानम्, "PathString is not std::wstring!");

अन्तःस्थ मार्गः_String गन्तव्यम्_मार्गः_String(std::string_view s) {
  प्रत्यागम गन्तव्यम्_Wide_String(s);
}
अन्तःस्थ मार्गः_String गन्तव्यम्_मार्गः_String(ध्रुव वर्ण* s) {
  प्रत्यागम गन्तव्यम्_Wide_String(s);
}
अन्तःस्थ मार्गः_String गन्तव्यम्_मार्गः_String(ध्रुव std::string& s) {
  प्रत्यागम गन्तव्यम्_Wide_String(s);
}

अन्तःस्थ मार्गः_Char गन्तव्यम्_अधस्तनम्_मार्गः_Char(मार्गः_Char c) {
  प्रत्यागम std::towlower(c);
}

अन्तःस्थ std::string मार्गः_गन्तव्यम्_UTF8_String(ध्रुव मार्गः_String& s) {
  प्रत्यागम गन्तव्यम्_UTF8_String(s);
}

#अन्यथा

static_assert(std::is_same<मार्गः_String, std::string>::मानम्, "PathString is not std::string!");

अन्तःस्थ मार्गः_String गन्तव्यम्_मार्गः_String(ध्रुव वर्ण* s) {
  प्रत्यागम s;
}

अन्तःस्थ मार्गः_String गन्तव्यम्_मार्गः_String(std::string_view s) {
  प्रत्यागम मार्गः_String{s};
}

अन्तःस्थ मार्गः_Char गन्तव्यम्_अधस्तनम्_मार्गः_Char(मार्गः_Char c) {
  प्रत्यागम std::tolower(c);
}

अन्तःस्थ std::string मार्गः_गन्तव्यम्_UTF8_String(ध्रुव मार्गः_String& s) {
  प्रत्यागम s;
}

#यदि_अन्तः

}  // namespace onnxruntime
