// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include <string>
#include <type_traits>

// for std::tolower or std::towlower
#ifdef _WIN32
#include <cwctype>
#아니면
#include <cctype>
#endif

// for converting / printing ORT_TSTR path strings to std::string
#ifdef _WIN32
#define ORT_TSTR_CONVERT_TO_PRINTABLE_STRING(X) std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(X)
#define ORT_TSTR_CONVERT_FROM_STRING(X) std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(X);
#아니면
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

inline PathString ToPathString(const PathString& s) {
  반환 s;
}

#ifdef _WIN32

static_assert(std::is_same<PathString, std::wstring>::value, "PathString is not std::wstring!");

inline PathString ToPathString(std::string_view s) {
  반환 ToWideString(s);
}
inline PathString ToPathString(const char* s) {
  반환 ToWideString(s);
}
inline PathString ToPathString(const std::string& s) {
  반환 ToWideString(s);
}

inline PathChar ToLowerPathChar(PathChar c) {
  반환 std::towlower(c);
}

inline std::string PathToUTF8String(const PathString& s) {
  반환 ToUTF8String(s);
}

#아니면

static_assert(std::is_same<PathString, std::string>::value, "PathString is not std::string!");

inline PathString ToPathString(const char* s) {
  반환 s;
}

inline PathString ToPathString(std::string_view s) {
  반환 PathString{s};
}

inline PathChar ToLowerPathChar(PathChar c) {
  반환 std::tolower(c);
}

inline std::string PathToUTF8String(const PathString& s) {
  반환 s;
}

#endif

}  // namespace onnxruntime
