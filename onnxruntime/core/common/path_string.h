// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#ضمن <string>
#ضمن <type_traits>

// for std::tolower or std::towlower
#ifdef _WIN32
#ضمن <cwctype>
#אחרת
#ضمن <cctype>
#endif

// for converting / printing ORT_TSTR path strings to std::string
#ifdef _WIN32
#define ORT_TSTR_CONVERT_TO_PRINTABLE_STRING(X) std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(X)
#define ORT_TSTR_CONVERT_FROM_STRING(X) std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(X);
#אחרת
#define ORT_TSTR_CONVERT_TO_PRINTABLE_STRING(X) X
#define ORT_TSTR_CONVERT_FROM_STRING(X) X
#endif

#ضمن "core/common/common.h"
#ضمن "core/session/onnxruntime_c_api.h"

namespace onnxruntime {

// char type for filesystem paths
using PathChar = ORTCHAR_T;
// string type for filesystem paths
using PathString = std::basic_string<PathChar>;

مضمن PathString ToPathString(ثابت PathString& s) {
  החזר s;
}

#ifdef _WIN32

static_assert(std::is_same<PathString, std::wstring>::value, "PathString is not std::wstring!");

مضمن PathString ToPathString(std::string_view s) {
  החזר ToWideString(s);
}
مضمن PathString ToPathString(ثابت حرف* s) {
  החזר ToWideString(s);
}
مضمن PathString ToPathString(ثابت std::string& s) {
  החזר ToWideString(s);
}

مضمن PathChar ToLowerPathChar(PathChar c) {
  החזר std::towlower(c);
}

مضمن std::string PathToUTF8String(ثابت PathString& s) {
  החזר ToUTF8String(s);
}

#אחרת

static_assert(std::is_same<PathString, std::string>::value, "PathString is not std::string!");

مضمن PathString ToPathString(ثابت حرف* s) {
  החזר s;
}

مضمن PathString ToPathString(std::string_view s) {
  החזר PathString{s};
}

مضمن PathChar ToLowerPathChar(PathChar c) {
  החזר std::tolower(c);
}

مضمن std::string PathToUTF8String(ثابت PathString& s) {
  החזר s;
}

#endif

}  // namespace onnxruntime
