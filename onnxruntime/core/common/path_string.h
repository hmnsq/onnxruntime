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

অন্তঃস্থ PathString ToPathString(ধ্রুব PathString& s) {
  প্রত্যাগম s;
}

#ifdef _WIN32

static_assert(std::is_same<PathString, std::wstring>::মান, "PathString is not std::wstring!");

অন্তঃস্থ PathString ToPathString(std::string_view s) {
  প্রত্যাগম ToWideString(s);
}
অন্তঃস্থ PathString ToPathString(ধ্রুব বর্ণ* s) {
  প্রত্যাগম ToWideString(s);
}
অন্তঃস্থ PathString ToPathString(ধ্রুব std::string& s) {
  প্রত্যাগম ToWideString(s);
}

অন্তঃস্থ PathChar ToLowerPathChar(PathChar c) {
  প্রত্যাগম std::towlower(c);
}

অন্তঃস্থ std::string PathToUTF8String(ধ্রুব PathString& s) {
  প্রত্যাগম ToUTF8String(s);
}

#অন্যথা

static_assert(std::is_same<PathString, std::string>::মান, "PathString is not std::string!");

অন্তঃস্থ PathString ToPathString(ধ্রুব বর্ণ* s) {
  প্রত্যাগম s;
}

অন্তঃস্থ PathString ToPathString(std::string_view s) {
  প্রত্যাগম PathString{s};
}

অন্তঃস্থ PathChar ToLowerPathChar(PathChar c) {
  প্রত্যাগম std::tolower(c);
}

অন্তঃস্থ std::string PathToUTF8String(ধ্রুব PathString& s) {
  প্রত্যাগম s;
}

#endif

}  // namespace onnxruntime
