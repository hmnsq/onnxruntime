// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#അന്തര്ഭാവ <string>
#അന്തര്ഭാവ <type_traits>

// for std::tolower or std::towlower
#ifdef _WIN32
#അന്തര്ഭാവ <cwctype>
#അല്ലെങ്കിൽ
#അന്തര്ഭാവ <cctype>
#endif

// for converting / printing ORT_TSTR path strings to std::string
#ifdef _WIN32
#define ORT_TSTR_CONVERT_TO_PRINTABLE_STRING(X) std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(X)
#define ORT_TSTR_CONVERT_FROM_STRING(X) std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(X);
#അല്ലെങ്കിൽ
#define ORT_TSTR_CONVERT_TO_PRINTABLE_STRING(X) X
#define ORT_TSTR_CONVERT_FROM_STRING(X) X
#endif

#അന്തര്ഭാവ "core/common/common.h"
#അന്തര്ഭാവ "core/session/onnxruntime_c_api.h"

namespace onnxruntime {

// char type for filesystem paths
using PathChar = ORTCHAR_T;
// string type for filesystem paths
using PathString = std::basic_string<PathChar>;

വരിയിൽ PathString ToPathString(സ്ഥിരം PathString& s) {
  തിരികെ s;
}

#ifdef _WIN32

static_assert(std::is_same<PathString, std::wstring>::മൂല്യം, "PathString is not std::wstring!");

വരിയിൽ PathString ToPathString(std::string_view s) {
  തിരികെ ToWideString(s);
}
വരിയിൽ PathString ToPathString(സ്ഥിരം അക്ഷരം* s) {
  തിരികെ ToWideString(s);
}
വരിയിൽ PathString ToPathString(സ്ഥിരം std::string& s) {
  തിരികെ ToWideString(s);
}

വരിയിൽ PathChar ToLowerPathChar(PathChar c) {
  തിരികെ std::towlower(c);
}

വരിയിൽ std::string PathToUTF8String(സ്ഥിരം PathString& s) {
  തിരികെ ToUTF8String(s);
}

#അല്ലെങ്കിൽ

static_assert(std::is_same<PathString, std::string>::മൂല്യം, "PathString is not std::string!");

വരിയിൽ PathString ToPathString(സ്ഥിരം അക്ഷരം* s) {
  തിരികെ s;
}

വരിയിൽ PathString ToPathString(std::string_view s) {
  തിരികെ PathString{s};
}

വരിയിൽ PathChar ToLowerPathChar(PathChar c) {
  തിരികെ std::tolower(c);
}

വരിയിൽ std::string PathToUTF8String(സ്ഥിരം PathString& s) {
  തിരികെ s;
}

#endif

}  // namespace onnxruntime
