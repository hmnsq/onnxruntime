// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#ضمن <string>
#ضمن <type_traits>

// for std::tolower or std::towlower
#ifdef _WIN32
#ضمن <cwctype>
#ܐܚܪܢܐ
#ضمن <cctype>
#endif

// for converting / printing ORT_TSTR path strings to std::string
#ifdef _WIN32
#define ORT_TSTR_CONVERT_TO_PRINTABLE_STRING(X) std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(X)
#define ORT_TSTR_CONVERT_FROM_STRING(X) std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(X);
#ܐܚܪܢܐ
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
  ܦܢܐ s;
}

#ifdef _WIN32

static_assert(std::is_same<PathString, std::wstring>::value, "PathString is not std::wstring!");

مضمن PathString ToPathString(std::string_view s) {
  ܦܢܐ ToWideString(s);
}
مضمن PathString ToPathString(ثابت حرف* s) {
  ܦܢܐ ToWideString(s);
}
مضمن PathString ToPathString(ثابت std::string& s) {
  ܦܢܐ ToWideString(s);
}

مضمن PathChar ToLowerPathChar(PathChar c) {
  ܦܢܐ std::towlower(c);
}

مضمن std::string PathToUTF8String(ثابت PathString& s) {
  ܦܢܐ ToUTF8String(s);
}

#ܐܚܪܢܐ

static_assert(std::is_same<PathString, std::string>::value, "PathString is not std::string!");

مضمن PathString ToPathString(ثابت حرف* s) {
  ܦܢܐ s;
}

مضمن PathString ToPathString(std::string_view s) {
  ܦܢܐ PathString{s};
}

مضمن PathChar ToLowerPathChar(PathChar c) {
  ܦܢܐ std::tolower(c);
}

مضمن std::string PathToUTF8String(ثابت PathString& s) {
  ܦܢܐ s;
}

#endif

}  // namespace onnxruntime
