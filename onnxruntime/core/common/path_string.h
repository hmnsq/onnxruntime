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

अन्तःस्थ PathString ToPathString(ध्रुव PathString& s) {
  प्रत्यागम s;
}

#ifdef _WIN32

static_assert(std::is_same<PathString, std::wstring>::मूल्य, "PathString is not std::wstring!");

अन्तःस्थ PathString ToPathString(std::string_view s) {
  प्रत्यागम ToWideString(s);
}
अन्तःस्थ PathString ToPathString(ध्रुव वर्ण* s) {
  प्रत्यागम ToWideString(s);
}
अन्तःस्थ PathString ToPathString(ध्रुव std::string& s) {
  प्रत्यागम ToWideString(s);
}

अन्तःस्थ PathChar ToLowerPathChar(PathChar c) {
  प्रत्यागम std::towlower(c);
}

अन्तःस्थ std::string PathToUTF8String(ध्रुव PathString& s) {
  प्रत्यागम ToUTF8String(s);
}

#अन्यथा

static_assert(std::is_same<PathString, std::string>::मूल्य, "PathString is not std::string!");

अन्तःस्थ PathString ToPathString(ध्रुव वर्ण* s) {
  प्रत्यागम s;
}

अन्तःस्थ PathString ToPathString(std::string_view s) {
  प्रत्यागम PathString{s};
}

अन्तःस्थ PathChar ToLowerPathChar(PathChar c) {
  प्रत्यागम std::छोटे(c);
}

अन्तःस्थ std::string PathToUTF8String(ध्रुव PathString& s) {
  प्रत्यागम s;
}

#endif

}  // namespace onnxruntime
