// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#رهنمود once

#ضمن <string>
#ضمن <type_traits>

// for std::tolower or std::towlower
#اگر_تعریف_شده _WIN32
#ضمن <cwctype>
#وإلا
#ضمن <cctype>
#پایان_اگر

// for converting / printing ORT_TSTR path strings to std::string
#اگر_تعریف_شده _WIN32
#تعریف_کن ORT_TSTR_CONVERT_مقصد_PRINTABLE_STRING(X) std::wstring_convert<std::codecvt_utf8<wchar_t>>().مقصد_بایتها(X)
#تعریف_کن ORT_TSTR_CONVERT_FROM_STRING(X) std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_بایتها(X);
#وإلا
#تعریف_کن ORT_TSTR_CONVERT_مقصد_PRINTABLE_STRING(X) X
#تعریف_کن ORT_TSTR_CONVERT_FROM_STRING(X) X
#پایان_اگر

#ضمن "core/common/common.h"
#ضمن "core/session/onnxruntime_c_api.h"

namespace onnxruntime {

// char type for filesystem paths
using مسیر_Char = ORTCHAR_T;
// string type for filesystem paths
using مسیر_String = std::basic_string<مسیر_Char>;

مضمن مسیر_String مقصد_مسیر_String(ثابت مسیر_String& s) {
  أرجع s;
}

#اگر_تعریف_شده _WIN32

static_assert(std::is_same<مسیر_String, std::wstring>::مقدار, "PathString is not std::wstring!");

مضمن مسیر_String مقصد_مسیر_String(std::string_view s) {
  أرجع مقصد_Wide_String(s);
}
مضمن مسیر_String مقصد_مسیر_String(ثابت حرف* s) {
  أرجع مقصد_Wide_String(s);
}
مضمن مسیر_String مقصد_مسیر_String(ثابت std::string& s) {
  أرجع مقصد_Wide_String(s);
}

مضمن مسیر_Char مقصد_پایینی_مسیر_Char(مسیر_Char c) {
  أرجع std::towlower(c);
}

مضمن std::string مسیر_مقصد_UTF8_String(ثابت مسیر_String& s) {
  أرجع مقصد_UTF8_String(s);
}

#وإلا

static_assert(std::is_same<مسیر_String, std::string>::مقدار, "PathString is not std::string!");

مضمن مسیر_String مقصد_مسیر_String(ثابت حرف* s) {
  أرجع s;
}

مضمن مسیر_String مقصد_مسیر_String(std::string_view s) {
  أرجع مسیر_String{s};
}

مضمن مسیر_Char مقصد_پایینی_مسیر_Char(مسیر_Char c) {
  أرجع std::tolower(c);
}

مضمن std::string مسیر_مقصد_UTF8_String(ثابت مسیر_String& s) {
  أرجع s;
}

#پایان_اگر

}  // namespace onnxruntime
