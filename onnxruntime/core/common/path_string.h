// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#அந்தர்பாவ <string>
#அந்தர்பாவ <type_traits>

// for std::tolower or std::towlower
#ifdef _WIN32
#அந்தர்பாவ <cwctype>
#இல்லையெனில்
#அந்தர்பாவ <cctype>
#endif

// for converting / printing ORT_TSTR path strings to std::string
#ifdef _WIN32
#define ORT_TSTR_CONVERT_TO_PRINTABLE_STRING(X) std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(X)
#define ORT_TSTR_CONVERT_FROM_STRING(X) std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(X);
#இல்லையெனில்
#define ORT_TSTR_CONVERT_TO_PRINTABLE_STRING(X) X
#define ORT_TSTR_CONVERT_FROM_STRING(X) X
#endif

#அந்தர்பாவ "core/common/common.h"
#அந்தர்பாவ "core/session/onnxruntime_c_api.h"

namespace onnxruntime {

// char type for filesystem paths
using PathChar = ORTCHAR_T;
// string type for filesystem paths
using PathString = std::basic_string<PathChar>;

உள்வரி PathString ToPathString(மாறிலி PathString& s) {
  திரும்பு s;
}

#ifdef _WIN32

static_assert(std::is_same<PathString, std::wstring>::மதிப்பு, "PathString is not std::wstring!");

உள்வரி PathString ToPathString(std::string_view s) {
  திரும்பு ToWideString(s);
}
உள்வரி PathString ToPathString(மாறிலி எழுத்து* s) {
  திரும்பு ToWideString(s);
}
உள்வரி PathString ToPathString(மாறிலி std::string& s) {
  திரும்பு ToWideString(s);
}

உள்வரி PathChar ToLowerPathChar(PathChar c) {
  திரும்பு std::towlower(c);
}

உள்வரி std::string PathToUTF8String(மாறிலி PathString& s) {
  திரும்பு ToUTF8String(s);
}

#இல்லையெனில்

static_assert(std::is_same<PathString, std::string>::மதிப்பு, "PathString is not std::string!");

உள்வரி PathString ToPathString(மாறிலி எழுத்து* s) {
  திரும்பு s;
}

உள்வரி PathString ToPathString(std::string_view s) {
  திரும்பு PathString{s};
}

உள்வரி PathChar ToLowerPathChar(PathChar c) {
  திரும்பு std::tolower(c);
}

உள்வரி std::string PathToUTF8String(மாறிலி PathString& s) {
  திரும்பு s;
}

#endif

}  // namespace onnxruntime
