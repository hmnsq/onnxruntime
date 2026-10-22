// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ਹਦਾਇਤ once

#ਅਨ੍ਤਰ੍ਭਾਵ <string>
#ਅਨ੍ਤਰ੍ਭਾਵ <type_traits>

// for std::tolower or std::towlower
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _WIN32
#ਅਨ੍ਤਰ੍ਭਾਵ <cwctype>
#ਨਹੀਂਤਾਂ
#ਅਨ੍ਤਰ੍ਭਾਵ <cctype>
#ਜੇ_ਅੰਤ

// for converting / printing ORT_TSTR path strings to std::string
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _WIN32
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ORT_TSTR_CONVERT_ਮੰਜ਼ਿਲ_PRINTABLE_STRING(X) std::wstring_convert<std::codecvt_utf8<wchar_t>>().ਮੰਜ਼ਿਲ_ਬਾਈਟਾਂ(X)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ORT_TSTR_CONVERT_FROM_STRING(X) std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_ਬਾਈਟਾਂ(X);
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ORT_TSTR_CONVERT_ਮੰਜ਼ਿਲ_PRINTABLE_STRING(X) X
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ORT_TSTR_CONVERT_FROM_STRING(X) X
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "core/common/common.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "core/session/onnxruntime_c_api.h"

namespace onnxruntime {

// char type for filesystem paths
using ਰਸਤਾ_Char = ORTCHAR_T;
// string type for filesystem paths
using ਰਸਤਾ_String = std::basic_string<ਰਸਤਾ_Char>;

ਸਤਰਅੰਦਰ ਰਸਤਾ_String ਮੰਜ਼ਿਲ_ਰਸਤਾ_String(ਅਚਲ ਰਸਤਾ_String& s) {
  ਵਾਪਸ s;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _WIN32

static_assert(std::is_same<ਰਸਤਾ_String, std::wstring>::ਮੁੱਲ, "PathString is not std::wstring!");

ਸਤਰਅੰਦਰ ਰਸਤਾ_String ਮੰਜ਼ਿਲ_ਰਸਤਾ_String(std::string_view s) {
  ਵਾਪਸ ਮੰਜ਼ਿਲ_Wide_String(s);
}
ਸਤਰਅੰਦਰ ਰਸਤਾ_String ਮੰਜ਼ਿਲ_ਰਸਤਾ_String(ਅਚਲ ਅੱਖਰ* s) {
  ਵਾਪਸ ਮੰਜ਼ਿਲ_Wide_String(s);
}
ਸਤਰਅੰਦਰ ਰਸਤਾ_String ਮੰਜ਼ਿਲ_ਰਸਤਾ_String(ਅਚਲ std::string& s) {
  ਵਾਪਸ ਮੰਜ਼ਿਲ_Wide_String(s);
}

ਸਤਰਅੰਦਰ ਰਸਤਾ_Char ਮੰਜ਼ਿਲ_ਹੇਠਲਾ_ਰਸਤਾ_Char(ਰਸਤਾ_Char c) {
  ਵਾਪਸ std::towlower(c);
}

ਸਤਰਅੰਦਰ std::string ਰਸਤਾ_ਮੰਜ਼ਿਲ_UTF8_String(ਅਚਲ ਰਸਤਾ_String& s) {
  ਵਾਪਸ ਮੰਜ਼ਿਲ_UTF8_String(s);
}

#ਨਹੀਂਤਾਂ

static_assert(std::is_same<ਰਸਤਾ_String, std::string>::ਮੁੱਲ, "PathString is not std::string!");

ਸਤਰਅੰਦਰ ਰਸਤਾ_String ਮੰਜ਼ਿਲ_ਰਸਤਾ_String(ਅਚਲ ਅੱਖਰ* s) {
  ਵਾਪਸ s;
}

ਸਤਰਅੰਦਰ ਰਸਤਾ_String ਮੰਜ਼ਿਲ_ਰਸਤਾ_String(std::string_view s) {
  ਵਾਪਸ ਰਸਤਾ_String{s};
}

ਸਤਰਅੰਦਰ ਰਸਤਾ_Char ਮੰਜ਼ਿਲ_ਹੇਠਲਾ_ਰਸਤਾ_Char(ਰਸਤਾ_Char c) {
  ਵਾਪਸ std::tolower(c);
}

ਸਤਰਅੰਦਰ std::string ਰਸਤਾ_ਮੰਜ਼ਿਲ_UTF8_String(ਅਚਲ ਰਸਤਾ_String& s) {
  ਵਾਪਸ s;
}

#ਜੇ_ਅੰਤ

}  // namespace onnxruntime
