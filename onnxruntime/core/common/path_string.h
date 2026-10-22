// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#अन्तर्भाव <string>
#अन्तर्भाव <type_traits>

// for std::tolower or std::towlower
#ifdef _WIN32
#अन्तर्भाव <cwctype>
#अञ्ञथा
#अन्तर्भाव <cctype>
#endif

// for converting / printing ORT_TSTR path strings to std::string
#ifdef _WIN32
#define ORT_TSTR_CONVERT_TO_PRINTABLE_STRING(X) std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(X)
#define ORT_TSTR_CONVERT_FROM_STRING(X) std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(X);
#अञ्ञथा
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
  पटिनिवत्त s;
}

#ifdef _WIN32

static_assert(std::is_same<PathString, std::wstring>::मानम्, "PathString is not std::wstring!");

अन्तःस्थ PathString ToPathString(std::string_view s) {
  पटिनिवत्त ToWideString(s);
}
अन्तःस्थ PathString ToPathString(ध्रुव वर्ण* s) {
  पटिनिवत्त ToWideString(s);
}
अन्तःस्थ PathString ToPathString(ध्रुव std::string& s) {
  पटिनिवत्त ToWideString(s);
}

अन्तःस्थ PathChar ToLowerPathChar(PathChar c) {
  पटिनिवत्त std::towlower(c);
}

अन्तःस्थ std::string PathToUTF8String(ध्रुव PathString& s) {
  पटिनिवत्त ToUTF8String(s);
}

#अञ्ञथा

static_assert(std::is_same<PathString, std::string>::मानम्, "PathString is not std::string!");

अन्तःस्थ PathString ToPathString(ध्रुव वर्ण* s) {
  पटिनिवत्त s;
}

अन्तःस्थ PathString ToPathString(std::string_view s) {
  पटिनिवत्त PathString{s};
}

अन्तःस्थ PathChar ToLowerPathChar(PathChar c) {
  पटिनिवत्त std::tolower(c);
}

अन्तःस्थ std::string PathToUTF8String(ध्रुव PathString& s) {
  पटिनिवत्त s;
}

#endif

}  // namespace onnxruntime
