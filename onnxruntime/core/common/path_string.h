// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#لارښوونه once

#ضمن <string>
#ضمن <type_traits>

// for std::tolower or std::towlower
#که_تعریف_شوی _WIN32
#ضمن <cwctype>
#وإلا
#ضمن <cctype>
#د_که_پای

// for converting / printing ORT_TSTR path strings to std::string
#که_تعریف_شوی _WIN32
#تعریف_کړه ORT_TSTR_CONVERT_منزل_PRINTABLE_STRING(X) std::wstring_convert<std::codecvt_utf8<wchar_t>>().منزل_بایټونه(X)
#تعریف_کړه ORT_TSTR_CONVERT_FROM_STRING(X) std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_بایټونه(X);
#وإلا
#تعریف_کړه ORT_TSTR_CONVERT_منزل_PRINTABLE_STRING(X) X
#تعریف_کړه ORT_TSTR_CONVERT_FROM_STRING(X) X
#د_که_پای

#ضمن "core/common/common.h"
#ضمن "core/session/onnxruntime_c_api.h"

namespace onnxruntime {

// char type for filesystem paths
using لار_Char = ORTCHAR_T;
// string type for filesystem paths
using لار_String = std::basic_string<لار_Char>;

مضمن لار_String منزل_لار_String(ثابت لار_String& s) {
  أرجع s;
}

#که_تعریف_شوی _WIN32

static_assert(std::is_same<لار_String, std::wstring>::ارزښت, "PathString is not std::wstring!");

مضمن لار_String منزل_لار_String(std::string_view s) {
  أرجع منزل_Wide_String(s);
}
مضمن لار_String منزل_لار_String(ثابت حرف* s) {
  أرجع منزل_Wide_String(s);
}
مضمن لار_String منزل_لار_String(ثابت std::string& s) {
  أرجع منزل_Wide_String(s);
}

مضمن لار_Char منزل_ښکتنی_لار_Char(لار_Char c) {
  أرجع std::towlower(c);
}

مضمن std::string لار_منزل_UTF8_String(ثابت لار_String& s) {
  أرجع منزل_UTF8_String(s);
}

#وإلا

static_assert(std::is_same<لار_String, std::string>::ارزښت, "PathString is not std::string!");

مضمن لار_String منزل_لار_String(ثابت حرف* s) {
  أرجع s;
}

مضمن لار_String منزل_لار_String(std::string_view s) {
  أرجع لار_String{s};
}

مضمن لار_Char منزل_ښکتنی_لار_Char(لار_Char c) {
  أرجع std::tolower(c);
}

مضمن std::string لار_منزل_UTF8_String(ثابت لار_String& s) {
  أرجع s;
}

#د_که_پای

}  // namespace onnxruntime
