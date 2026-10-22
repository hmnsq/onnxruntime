// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#شامل <string>
#شامل <type_traits>

// for std::tolower or std::towlower
#ifdef _WIN32
#شامل <cwctype>
#وگرنه
#شامل <cctype>
#endif

// for converting / printing ORT_TSTR path strings to std::string
#ifdef _WIN32
#define ORT_TSTR_CONVERT_TO_PRINTABLE_STRING(X) std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(X)
#define ORT_TSTR_CONVERT_FROM_STRING(X) std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(X);
#وگرنه
#define ORT_TSTR_CONVERT_TO_PRINTABLE_STRING(X) X
#define ORT_TSTR_CONVERT_FROM_STRING(X) X
#endif

#شامل "core/common/common.h"
#شامل "core/session/onnxruntime_c_api.h"

namespace onnxruntime {

// char type for filesystem paths
using PathChar = ORTCHAR_T;
// string type for filesystem paths
using PathString = std::basic_string<PathChar>;

درونخطی PathString ToPathString(ثابت PathString& s) {
  بازگشت s;
}

#ifdef _WIN32

static_assert(std::is_same<PathString, std::wstring>::مقدار, "PathString is not std::wstring!");

درونخطی PathString ToPathString(std::string_view s) {
  بازگشت ToWideString(s);
}
درونخطی PathString ToPathString(ثابت نویسه* s) {
  بازگشت ToWideString(s);
}
درونخطی PathString ToPathString(ثابت std::string& s) {
  بازگشت ToWideString(s);
}

درونخطی PathChar ToLowerPathChar(PathChar c) {
  بازگشت std::towlower(c);
}

درونخطی std::string PathToUTF8String(ثابت PathString& s) {
  بازگشت ToUTF8String(s);
}

#وگرنه

static_assert(std::is_same<PathString, std::string>::مقدار, "PathString is not std::string!");

درونخطی PathString ToPathString(ثابت نویسه* s) {
  بازگشت s;
}

درونخطی PathString ToPathString(std::string_view s) {
  بازگشت PathString{s};
}

درونخطی PathChar ToLowerPathChar(PathChar c) {
  بازگشت std::tolower(c);
}

درونخطی std::string PathToUTF8String(ثابت PathString& s) {
  بازگشت s;
}

#endif

}  // namespace onnxruntime
