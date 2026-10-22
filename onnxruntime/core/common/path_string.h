// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#অন্তর্ভাব <string>
#অন্তর্ভাব <type_traits>

// for std::tolower or std::towlower
#ifdef _WIN32
#অন্তর্ভাব <cwctype>
#অন্যথা
#অন্তর্ভাব <cctype>
#endif

// for converting / printing ORT_TSTR path strings to std::string
#ifdef _WIN32
#define ORT_TSTR_CONVERT_TO_PRINTABLE_STRING(X) std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(X)
#define ORT_TSTR_CONVERT_FROM_STRING(X) std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(X);
#অন্যথা
#define ORT_TSTR_CONVERT_TO_PRINTABLE_STRING(X) X
#define ORT_TSTR_CONVERT_FROM_STRING(X) X
#endif

#অন্তর্ভাব "core/common/common.h"
#অন্তর্ভাব "core/session/onnxruntime_c_api.h"

namespace onnxruntime {

// char type for filesystem paths
using PathChar = ORTCHAR_T;
// string type for filesystem paths
using PathString = std::basic_string<PathChar>;

অন্তঃরেখ PathString ToPathString(ধ্রুবক PathString& s) {
  ফেরত s;
}

#ifdef _WIN32

static_assert(std::is_same<PathString, std::wstring>::মান, "PathString is not std::wstring!");

অন্তঃরেখ PathString ToPathString(std::string_view s) {
  ফেরত ToWideString(s);
}
অন্তঃরেখ PathString ToPathString(ধ্রুবক অক্ষর* s) {
  ফেরত ToWideString(s);
}
অন্তঃরেখ PathString ToPathString(ধ্রুবক std::string& s) {
  ফেরত ToWideString(s);
}

অন্তঃরেখ PathChar ToLowerPathChar(PathChar c) {
  ফেরত std::towlower(c);
}

অন্তঃরেখ std::string PathToUTF8String(ধ্রুবক PathString& s) {
  ফেরত ToUTF8String(s);
}

#অন্যথা

static_assert(std::is_same<PathString, std::string>::মান, "PathString is not std::string!");

অন্তঃরেখ PathString ToPathString(ধ্রুবক অক্ষর* s) {
  ফেরত s;
}

অন্তঃরেখ PathString ToPathString(std::string_view s) {
  ফেরত PathString{s};
}

অন্তঃরেখ PathChar ToLowerPathChar(PathChar c) {
  ফেরত std::tolower(c);
}

অন্তঃরেখ std::string PathToUTF8String(ধ্রুবক PathString& s) {
  ফেরত s;
}

#endif

}  // namespace onnxruntime
