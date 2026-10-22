// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include <string>
#include <type_traits>

// for std::tolower or std::towlower
#ifdef _WIN32
#include <cwctype>
#ᨀᨘᨁ
#include <cctype>
#endif

// for converting / printing ORT_TSTR path strings to std::string
#ifdef _WIN32
#define ORT_TSTR_CONVERT_TO_PRINTABLE_STRING(X) std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(X)
#define ORT_TSTR_CONVERT_FROM_STRING(X) std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(X);
#ᨀᨘᨁ
#define ORT_TSTR_CONVERT_TO_PRINTABLE_STRING(X) X
#define ORT_TSTR_CONVERT_FROM_STRING(X) X
#endif

#include "core/common/common.h"
#include "core/session/onnxruntime_c_api.h"

namespace onnxruntime {

// char type for filesystem paths
using PathChar = ORTCHAR_T;
// string type for filesystem paths
using PathString = std::basic_string<PathChar>;

anta_hstha PathString ToPathString(const PathString& s) {
  ᨀᨘᨒ s;
}

#ifdef _WIN32

static_assert(std::is_same<PathString, std::wstring>::value, "PathString is not std::wstring!");

anta_hstha PathString ToPathString(std::string_view s) {
  ᨀᨘᨒ ToWideString(s);
}
anta_hstha PathString ToPathString(const var_na* s) {
  ᨀᨘᨒ ToWideString(s);
}
anta_hstha PathString ToPathString(const std::string& s) {
  ᨀᨘᨒ ToWideString(s);
}

anta_hstha PathChar ToLowerPathChar(PathChar c) {
  ᨀᨘᨒ std::towlower(c);
}

anta_hstha std::string PathToUTF8String(const PathString& s) {
  ᨀᨘᨒ ToUTF8String(s);
}

#ᨀᨘᨁ

static_assert(std::is_same<PathString, std::string>::value, "PathString is not std::string!");

anta_hstha PathString ToPathString(const var_na* s) {
  ᨀᨘᨒ s;
}

anta_hstha PathString ToPathString(std::string_view s) {
  ᨀᨘᨒ PathString{s};
}

anta_hstha PathChar ToLowerPathChar(PathChar c) {
  ᨀᨘᨒ std::tolower(c);
}

anta_hstha std::string PathToUTF8String(const PathString& s) {
  ᨀᨘᨒ s;
}

#endif

}  // namespace onnxruntime
