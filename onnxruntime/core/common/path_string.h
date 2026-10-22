// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#ଅନ୍ତର୍ଭାଵ <string>
#ଅନ୍ତର୍ଭାଵ <type_traits>

// for std::tolower or std::towlower
#ifdef _WIN32
#ଅନ୍ତର୍ଭାଵ <cwctype>
#ଅନ୍ୟଥା
#ଅନ୍ତର୍ଭାଵ <cctype>
#endif

// for converting / printing ORT_TSTR path strings to std::string
#ifdef _WIN32
#define ORT_TSTR_CONVERT_TO_PRINTABLE_STRING(X) std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(X)
#define ORT_TSTR_CONVERT_FROM_STRING(X) std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(X);
#ଅନ୍ୟଥା
#define ORT_TSTR_CONVERT_TO_PRINTABLE_STRING(X) X
#define ORT_TSTR_CONVERT_FROM_STRING(X) X
#endif

#ଅନ୍ତର୍ଭାଵ "core/common/common.h"
#ଅନ୍ତର୍ଭାଵ "core/session/onnxruntime_c_api.h"

namespace onnxruntime {

// char type for filesystem paths
using PathChar = ORTCHAR_T;
// string type for filesystem paths
using PathString = std::basic_string<PathChar>;

ଧାଡ଼ିଭିତରେ PathString ToPathString(ଅଚଳ PathString& s) {
  ଫେରନ୍ତୁ s;
}

#ifdef _WIN32

static_assert(std::is_same<PathString, std::wstring>::ମୂଲ୍ୟ, "PathString is not std::wstring!");

ଧାଡ଼ିଭିତରେ PathString ToPathString(std::string_view s) {
  ଫେରନ୍ତୁ ToWideString(s);
}
ଧାଡ଼ିଭିତରେ PathString ToPathString(ଅଚଳ ଅକ୍ଷର* s) {
  ଫେରନ୍ତୁ ToWideString(s);
}
ଧାଡ଼ିଭିତରେ PathString ToPathString(ଅଚଳ std::string& s) {
  ଫେରନ୍ତୁ ToWideString(s);
}

ଧାଡ଼ିଭିତରେ PathChar ToLowerPathChar(PathChar c) {
  ଫେରନ୍ତୁ std::towlower(c);
}

ଧାଡ଼ିଭିତରେ std::string PathToUTF8String(ଅଚଳ PathString& s) {
  ଫେରନ୍ତୁ ToUTF8String(s);
}

#ଅନ୍ୟଥା

static_assert(std::is_same<PathString, std::string>::ମୂଲ୍ୟ, "PathString is not std::string!");

ଧାଡ଼ିଭିତରେ PathString ToPathString(ଅଚଳ ଅକ୍ଷର* s) {
  ଫେରନ୍ତୁ s;
}

ଧାଡ଼ିଭିତରେ PathString ToPathString(std::string_view s) {
  ଫେରନ୍ତୁ PathString{s};
}

ଧାଡ଼ିଭିତରେ PathChar ToLowerPathChar(PathChar c) {
  ଫେରନ୍ତୁ std::tolower(c);
}

ଧାଡ଼ିଭିତରେ std::string PathToUTF8String(ଅଚଳ PathString& s) {
  ଫେରନ୍ତୁ s;
}

#endif

}  // namespace onnxruntime
