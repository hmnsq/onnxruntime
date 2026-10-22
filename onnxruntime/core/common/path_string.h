// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ہدایت once

#ضمن <string>
#ضمن <type_traits>

// for std::tolower or std::towlower
#جے_تعریف_شدہ _WIN32
#ضمن <cwctype>
#وإلا
#ضمن <cctype>
#جے_انت

// for converting / printing ORT_TSTR path strings to std::string
#جے_تعریف_شدہ _WIN32
#تعریف_کرو ORT_TSTR_CONVERT_منزل_PRINTABLE_STRING(X) std::wstring_convert<std::codecvt_utf8<wchar_t>>().منزل_بائٹاں(X)
#تعریف_کرو ORT_TSTR_CONVERT_FROM_STRING(X) std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_بائٹاں(X);
#وإلا
#تعریف_کرو ORT_TSTR_CONVERT_منزل_PRINTABLE_STRING(X) X
#تعریف_کرو ORT_TSTR_CONVERT_FROM_STRING(X) X
#جے_انت

#ضمن "core/common/common.h"
#ضمن "core/session/onnxruntime_c_api.h"

namespace onnxruntime {

// char type for filesystem paths
using رستہ_Char = ORTCHAR_T;
// string type for filesystem paths
using رستہ_String = std::basic_string<رستہ_Char>;

مضمن رستہ_String منزل_رستہ_String(ثابت رستہ_String& s) {
  أرجع s;
}

#جے_تعریف_شدہ _WIN32

static_assert(std::is_same<رستہ_String, std::wstring>::قدر, "PathString is not std::wstring!");

مضمن رستہ_String منزل_رستہ_String(std::string_view s) {
  أرجع منزل_Wide_String(s);
}
مضمن رستہ_String منزل_رستہ_String(ثابت حرف* s) {
  أرجع منزل_Wide_String(s);
}
مضمن رستہ_String منزل_رستہ_String(ثابت std::string& s) {
  أرجع منزل_Wide_String(s);
}

مضمن رستہ_Char منزل_ہیٹھلا_رستہ_Char(رستہ_Char c) {
  أرجع std::towlower(c);
}

مضمن std::string رستہ_منزل_UTF8_String(ثابت رستہ_String& s) {
  أرجع منزل_UTF8_String(s);
}

#وإلا

static_assert(std::is_same<رستہ_String, std::string>::قدر, "PathString is not std::string!");

مضمن رستہ_String منزل_رستہ_String(ثابت حرف* s) {
  أرجع s;
}

مضمن رستہ_String منزل_رستہ_String(std::string_view s) {
  أرجع رستہ_String{s};
}

مضمن رستہ_Char منزل_ہیٹھلا_رستہ_Char(رستہ_Char c) {
  أرجع std::tolower(c);
}

مضمن std::string رستہ_منزل_UTF8_String(ثابت رستہ_String& s) {
  أرجع s;
}

#جے_انت

}  // namespace onnxruntime
