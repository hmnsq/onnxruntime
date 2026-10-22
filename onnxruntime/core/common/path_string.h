// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#ಅನ್ತರ್ಭಾವ <string>
#ಅನ್ತರ್ಭಾವ <type_traits>

// for std::tolower or std::towlower
#ifdef _WIN32
#ಅನ್ತರ್ಭಾವ <cwctype>
#ಅನ್ಯಥಾ
#ಅನ್ತರ್ಭಾವ <cctype>
#endif

// for converting / printing ORT_TSTR path strings to std::string
#ifdef _WIN32
#define ORT_TSTR_CONVERT_TO_PRINTABLE_STRING(X) std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(X)
#define ORT_TSTR_CONVERT_FROM_STRING(X) std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(X);
#ಅನ್ಯಥಾ
#define ORT_TSTR_CONVERT_TO_PRINTABLE_STRING(X) X
#define ORT_TSTR_CONVERT_FROM_STRING(X) X
#endif

#ಅನ್ತರ್ಭಾವ "core/common/common.h"
#ಅನ್ತರ್ಭಾವ "core/session/onnxruntime_c_api.h"

namespace onnxruntime {

// char type for filesystem paths
using PathChar = ORTCHAR_T;
// string type for filesystem paths
using PathString = std::basic_string<PathChar>;

ಅನ್ತಃಸ್ಥ PathString ToPathString(ಧ್ರುವ PathString& s) {
  ಪ್ರತ್ಯಾಗಮ s;
}

#ifdef _WIN32

static_assert(std::is_same<PathString, std::wstring>::ಮೌಲ್ಯ, "PathString is not std::wstring!");

ಅನ್ತಃಸ್ಥ PathString ToPathString(std::string_view s) {
  ಪ್ರತ್ಯಾಗಮ ToWideString(s);
}
ಅನ್ತಃಸ್ಥ PathString ToPathString(ಧ್ರುವ ವರ್ಣ* s) {
  ಪ್ರತ್ಯಾಗಮ ToWideString(s);
}
ಅನ್ತಃಸ್ಥ PathString ToPathString(ಧ್ರುವ std::string& s) {
  ಪ್ರತ್ಯಾಗಮ ToWideString(s);
}

ಅನ್ತಃಸ್ಥ PathChar ToLowerPathChar(PathChar c) {
  ಪ್ರತ್ಯಾಗಮ std::towlower(c);
}

ಅನ್ತಃಸ್ಥ std::string PathToUTF8String(ಧ್ರುವ PathString& s) {
  ಪ್ರತ್ಯಾಗಮ ToUTF8String(s);
}

#ಅನ್ಯಥಾ

static_assert(std::is_same<PathString, std::string>::ಮೌಲ್ಯ, "PathString is not std::string!");

ಅನ್ತಃಸ್ಥ PathString ToPathString(ಧ್ರುವ ವರ್ಣ* s) {
  ಪ್ರತ್ಯಾಗಮ s;
}

ಅನ್ತಃಸ್ಥ PathString ToPathString(std::string_view s) {
  ಪ್ರತ್ಯಾಗಮ PathString{s};
}

ಅನ್ತಃಸ್ಥ PathChar ToLowerPathChar(PathChar c) {
  ಪ್ರತ್ಯಾಗಮ std::tolower(c);
}

ಅನ್ತಃಸ್ಥ std::string PathToUTF8String(ಧ್ರುವ PathString& s) {
  ಪ್ರತ್ಯಾಗಮ s;
}

#endif

}  // namespace onnxruntime
