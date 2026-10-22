// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#अन्तर्भाव <string>
#अन्तर्भाव <type_traits>

// for std::tolower or std::towlower
#ifdef _WIN32
#अन्तर्भाव <cwctype>
#अन्यथा
#अन्तर्भाव <cctype>
#endif

// for converting / printing ORT_TSTR path strings to std::string
#ifdef _WIN32
#define ORT_TSTR_CONVERT_TO_PRINTABLE_STRING(X) std::wstring_convert<std::codecvt_utf8<ब_अक्षर_प्रकार>>().to_bytes(X)
#define ORT_TSTR_CONVERT_FROM_STRING(X) std::wstring_convert<std::codecvt_utf8<ब_अक्षर_प्रकार>>().from_bytes(X);
#अन्यथा
#define ORT_TSTR_CONVERT_TO_PRINTABLE_STRING(X) X
#define ORT_TSTR_CONVERT_FROM_STRING(X) X
#endif

#अन्तर्भाव "core/common/common.h"
#अन्तर्भाव "core/session/onnxruntime_c_api.h"

namespace onnxruntime {

// char type for filesystem paths
using PathChar = ORTCHAR_T;
// string type for filesystem paths
using PathString = std::basic_string<PathChar>;

ओळीत PathString ToPathString(स्थिरांक PathString& s) {
  परत s;
}

#ifdef _WIN32

static_assert(std::is_same<PathString, std::wstring>::मूल्य, "PathString is not std::wstring!");

ओळीत PathString ToPathString(std::string_view s) {
  परत ToWideString(s);
}
ओळीत PathString ToPathString(स्थिरांक अक्षर* s) {
  परत ToWideString(s);
}
ओळीत PathString ToPathString(स्थिरांक std::string& s) {
  परत ToWideString(s);
}

ओळीत PathChar ToLowerPathChar(PathChar c) {
  परत std::towlower(c);
}

ओळीत std::string PathToUTF8String(स्थिरांक PathString& s) {
  परत ToUTF8String(s);
}

#अन्यथा

static_assert(std::is_same<PathString, std::string>::मूल्य, "PathString is not std::string!");

ओळीत PathString ToPathString(स्थिरांक अक्षर* s) {
  परत s;
}

ओळीत PathString ToPathString(std::string_view s) {
  परत PathString{s};
}

ओळीत PathChar ToLowerPathChar(PathChar c) {
  परत std::छोटे(c);
}

ओळीत std::string PathToUTF8String(स्थिरांक PathString& s) {
  परत s;
}

#endif

}  // namespace onnxruntime
