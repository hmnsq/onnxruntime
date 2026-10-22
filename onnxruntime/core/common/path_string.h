// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#هدايت once

#ضمن <string>
#ضمن <type_traits>

// for std::tolower or std::towlower
#جيڪڏهن_وصف_ٿيل _WIN32
#ضمن <cwctype>
#وإلا
#ضمن <cctype>
#جيڪڏهن_پورو

// for converting / printing ORT_TSTR path strings to std::string
#جيڪڏهن_وصف_ٿيل _WIN32
#وصف_ڪريو ORT_TSTR_CONVERT_منزل_PRINTABLE_STRING(X) std::wstring_convert<std::codecvt_utf8<wchar_t>>().منزل_بائيٽون(X)
#وصف_ڪريو ORT_TSTR_CONVERT_FROM_STRING(X) std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_بائيٽون(X);
#وإلا
#وصف_ڪريو ORT_TSTR_CONVERT_منزل_PRINTABLE_STRING(X) X
#وصف_ڪريو ORT_TSTR_CONVERT_FROM_STRING(X) X
#جيڪڏهن_پورو

#ضمن "core/common/common.h"
#ضمن "core/session/onnxruntime_c_api.h"

namespace onnxruntime {

// char type for filesystem paths
using رستو_Char = ORTCHAR_T;
// string type for filesystem paths
using رستو_String = std::basic_string<رستو_Char>;

مضمن رستو_String منزل_رستو_String(ثابت رستو_String& s) {
  أرجع s;
}

#جيڪڏهن_وصف_ٿيل _WIN32

static_assert(std::is_same<رستو_String, std::wstring>::قدر, "PathString is not std::wstring!");

مضمن رستو_String منزل_رستو_String(std::string_view s) {
  أرجع منزل_Wide_String(s);
}
مضمن رستو_String منزل_رستو_String(ثابت حرف* s) {
  أرجع منزل_Wide_String(s);
}
مضمن رستو_String منزل_رستو_String(ثابت std::string& s) {
  أرجع منزل_Wide_String(s);
}

مضمن رستو_Char منزل_هيٺاهون_رستو_Char(رستو_Char c) {
  أرجع std::towlower(c);
}

مضمن std::string رستو_منزل_UTF8_String(ثابت رستو_String& s) {
  أرجع منزل_UTF8_String(s);
}

#وإلا

static_assert(std::is_same<رستو_String, std::string>::قدر, "PathString is not std::string!");

مضمن رستو_String منزل_رستو_String(ثابت حرف* s) {
  أرجع s;
}

مضمن رستو_String منزل_رستو_String(std::string_view s) {
  أرجع رستو_String{s};
}

مضمن رستو_Char منزل_هيٺاهون_رستو_Char(رستو_Char c) {
  أرجع std::tolower(c);
}

مضمن std::string رستو_منزل_UTF8_String(ثابت رستو_String& s) {
  أرجع s;
}

#جيڪڏهن_پورو

}  // namespace onnxruntime
