// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#ضمن <string>
#ضمن <type_traits>

// for std::tolower or std::towlower
#ifdef _WIN32
#ضمن <cwctype>
#وإلا
#ضمن <cctype>
#endif

// for converting / printing ORT_TSTR path strings to std::string
#ifdef _WIN32
#define ORT_TSTR_CONVERT_TO_PRINTABLE_STRING(X) std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(X)
#define ORT_TSTR_CONVERT_FROM_STRING(X) std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(X);
#وإلا
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
  إرجاع s;
}

#ifdef _WIN32

static_assert(std::is_same<PathString, std::wstring>::قيمة, "PathString is not std::wstring!");

مضمن PathString ToPathString(std::string_view s) {
  إرجاع ToWideString(s);
}
مضمن PathString ToPathString(ثابت حرف* s) {
  إرجاع ToWideString(s);
}
مضمن PathString ToPathString(ثابت std::string& s) {
  إرجاع ToWideString(s);
}

مضمن PathChar ToLowerPathChar(PathChar c) {
  إرجاع std::towlower(c);
}

مضمن std::string PathToUTF8String(ثابت PathString& s) {
  إرجاع ToUTF8String(s);
}

#وإلا

static_assert(std::is_same<PathString, std::string>::قيمة, "PathString is not std::string!");

مضمن PathString ToPathString(ثابت حرف* s) {
  إرجاع s;
}

مضمن PathString ToPathString(std::string_view s) {
  إرجاع PathString{s};
}

مضمن PathChar ToLowerPathChar(PathChar c) {
  إرجاع std::tolower(c);
}

مضمن std::string PathToUTF8String(ثابت PathString& s) {
  إرجاع s;
}

#endif

}  // namespace onnxruntime
