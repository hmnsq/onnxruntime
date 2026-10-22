// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#અન્તર્ભાવ <string>
#અન્તર્ભાવ <type_traits>

// for std::tolower or std::towlower
#ifdef _WIN32
#અન્તર્ભાવ <cwctype>
#નહીંતર
#અન્તર્ભાવ <cctype>
#endif

// for converting / printing ORT_TSTR path strings to std::string
#ifdef _WIN32
#define ORT_TSTR_CONVERT_TO_PRINTABLE_STRING(X) std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(X)
#define ORT_TSTR_CONVERT_FROM_STRING(X) std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(X);
#નહીંતર
#define ORT_TSTR_CONVERT_TO_PRINTABLE_STRING(X) X
#define ORT_TSTR_CONVERT_FROM_STRING(X) X
#endif

#અન્તર્ભાવ "core/common/common.h"
#અન્તર્ભાવ "core/session/onnxruntime_c_api.h"

namespace onnxruntime {

// char type for filesystem paths
using PathChar = ORTCHAR_T;
// string type for filesystem paths
using PathString = std::basic_string<PathChar>;

અંતઃરેખ PathString ToPathString(અચલ PathString& s) {
  પાછા s;
}

#ifdef _WIN32

static_assert(std::is_same<PathString, std::wstring>::કિંમત, "PathString is not std::wstring!");

અંતઃરેખ PathString ToPathString(std::string_view s) {
  પાછા ToWideString(s);
}
અંતઃરેખ PathString ToPathString(અચલ અક્ષર* s) {
  પાછા ToWideString(s);
}
અંતઃરેખ PathString ToPathString(અચલ std::string& s) {
  પાછા ToWideString(s);
}

અંતઃરેખ PathChar ToLowerPathChar(PathChar c) {
  પાછા std::towlower(c);
}

અંતઃરેખ std::string PathToUTF8String(અચલ PathString& s) {
  પાછા ToUTF8String(s);
}

#નહીંતર

static_assert(std::is_same<PathString, std::string>::કિંમત, "PathString is not std::string!");

અંતઃરેખ PathString ToPathString(અચલ અક્ષર* s) {
  પાછા s;
}

અંતઃરેખ PathString ToPathString(std::string_view s) {
  પાછા PathString{s};
}

અંતઃરેખ PathChar ToLowerPathChar(PathChar c) {
  પાછા std::tolower(c);
}

અંતઃરેખ std::string PathToUTF8String(અચલ PathString& s) {
  પાછા s;
}

#endif

}  // namespace onnxruntime
