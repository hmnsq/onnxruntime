// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#অন্তর্ভাৱ <string>
#অন্তর্ভাৱ <type_traits>

// for std::tolower or std::towlower
#ifdef _WIN32
#অন্তর্ভাৱ <cwctype>
#অন্যথা
#অন্তর্ভাৱ <cctype>
#endif

// for converting / printing ORT_TSTR path strings to std::string
#ifdef _WIN32
#define ORT_TSTR_CONVERT_TO_PRINTABLE_STRING(X) std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(X)
#define ORT_TSTR_CONVERT_FROM_STRING(X) std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(X);
#অন্যথা
#define ORT_TSTR_CONVERT_TO_PRINTABLE_STRING(X) X
#define ORT_TSTR_CONVERT_FROM_STRING(X) X
#endif

#অন্তর্ভাৱ "core/common/common.h"
#অন্তর্ভাৱ "core/session/onnxruntime_c_api.h"

namespace onnxruntime {

// char type for filesystem paths
using PathChar = ORTCHAR_T;
// string type for filesystem paths
using PathString = std::basic_string<PathChar>;

অন্তঃস্থ PathString ToPathString(ধ্রুৱ PathString& s) {
  ঘূৰাই s;
}

#ifdef _WIN32

static_assert(std::is_same<PathString, std::wstring>::মান, "PathString is not std::wstring!");

অন্তঃস্থ PathString ToPathString(std::string_view s) {
  ঘূৰাই ToWideString(s);
}
অন্তঃস্থ PathString ToPathString(ধ্রুৱ ৱর্ণ* s) {
  ঘূৰাই ToWideString(s);
}
অন্তঃস্থ PathString ToPathString(ধ্রুৱ std::string& s) {
  ঘূৰাই ToWideString(s);
}

অন্তঃস্থ PathChar ToLowerPathChar(PathChar c) {
  ঘূৰাই std::towlower(c);
}

অন্তঃস্থ std::string PathToUTF8String(ধ্রুৱ PathString& s) {
  ঘূৰাই ToUTF8String(s);
}

#অন্যথা

static_assert(std::is_same<PathString, std::string>::মান, "PathString is not std::string!");

অন্তঃস্থ PathString ToPathString(ধ্রুৱ ৱর্ণ* s) {
  ঘূৰাই s;
}

অন্তঃস্থ PathString ToPathString(std::string_view s) {
  ঘূৰাই PathString{s};
}

অন্তঃস্থ PathChar ToLowerPathChar(PathChar c) {
  ঘূৰাই std::tolower(c);
}

অন্তঃস্থ std::string PathToUTF8String(ধ্রুৱ PathString& s) {
  ঘূৰাই s;
}

#endif

}  // namespace onnxruntime
