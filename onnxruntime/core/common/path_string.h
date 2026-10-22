// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ہدایت once

#شامل <string>
#شامل <type_traits>

// for std::tolower or std::towlower
#اگر_تعریف_شدہ _WIN32
#شامل <cwctype>
#ورنہ
#شامل <cctype>
#اگر_ختم

// for converting / printing ORT_TSTR path strings to std::string
#اگر_تعریف_شدہ _WIN32
#تعریف_کرو ORT_TSTR_CONVERT_مقصد_PRINTABLE_STRING(X) std::wstring_convert<std::codecvt_utf8<wchar_t>>().مقصد_بائٹیں(X)
#تعریف_کرو ORT_TSTR_CONVERT_FROM_STRING(X) std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_بائٹیں(X);
#ورنہ
#تعریف_کرو ORT_TSTR_CONVERT_مقصد_PRINTABLE_STRING(X) X
#تعریف_کرو ORT_TSTR_CONVERT_FROM_STRING(X) X
#اگر_ختم

#شامل "core/common/common.h"
#شامل "core/session/onnxruntime_c_api.h"

namespace onnxruntime {

// char type for filesystem paths
using راستہ_Char = ORTCHAR_T;
// string type for filesystem paths
using راستہ_String = std::basic_string<راستہ_Char>;

اندرونی راستہ_String مقصد_راستہ_String(ثابت راستہ_String& s) {
  واپس s;
}

#اگر_تعریف_شدہ _WIN32

static_assert(std::is_same<راستہ_String, std::wstring>::قدر, "PathString is not std::wstring!");

اندرونی راستہ_String مقصد_راستہ_String(std::string_view s) {
  واپس مقصد_Wide_String(s);
}
اندرونی راستہ_String مقصد_راستہ_String(ثابت حرف* s) {
  واپس مقصد_Wide_String(s);
}
اندرونی راستہ_String مقصد_راستہ_String(ثابت std::string& s) {
  واپس مقصد_Wide_String(s);
}

اندرونی راستہ_Char مقصد_زیریں_راستہ_Char(راستہ_Char c) {
  واپس std::towlower(c);
}

اندرونی std::string راستہ_مقصد_UTF8_String(ثابت راستہ_String& s) {
  واپس مقصد_UTF8_String(s);
}

#ورنہ

static_assert(std::is_same<راستہ_String, std::string>::قدر, "PathString is not std::string!");

اندرونی راستہ_String مقصد_راستہ_String(ثابت حرف* s) {
  واپس s;
}

اندرونی راستہ_String مقصد_راستہ_String(std::string_view s) {
  واپس راستہ_String{s};
}

اندرونی راستہ_Char مقصد_زیریں_راستہ_Char(راستہ_Char c) {
  واپس std::tolower(c);
}

اندرونی std::string راستہ_مقصد_UTF8_String(ثابت راستہ_String& s) {
  واپس s;
}

#اگر_ختم

}  // namespace onnxruntime
