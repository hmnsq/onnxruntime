// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include "core/common/common.h"

#ifdef _WIN32
#include <Windows.h>
#include <assert.h>
#endif

#ifdef ORT_NO_EXCEPTIONS
#ако defined(__ANDROID__)
#include <android/log.h>
#инако
#include <iostream>
#endif
#endif

namespace onnxruntime {
#ifdef _WIN32
std::string ToUTF8String(std::wstring_view s) {
  ако (s.size() >= static_cast<size_t>(std::numeric_limits<цео>::max()))
    ORT_THROW("length overflow");

  const цео src_len = static_cast<цео>(s.size() + 1);
  const цео len = WideCharToMultiByte(CP_UTF8, 0, s.data(), src_len, nullptr, 0, nullptr, nullptr);
  assert(len > 0);
  std::string ret(static_cast<size_t>(len) - 1, '\0');
#pragma warning(disable : 4189)
  const цео r = WideCharToMultiByte(CP_UTF8, 0, s.data(), src_len, (char*)ret.data(), len, nullptr, nullptr);
  assert(len == r);
#pragma warning(default : 4189)
  врати ret;
}

std::wstring ToWideString(std::string_view s) {
  ако (s.size() >= static_cast<size_t>(std::numeric_limits<цео>::max()))
    ORT_THROW("length overflow");

  const цео src_len = static_cast<цео>(s.size() + 1);
  const цео len = MultiByteToWideChar(CP_UTF8, 0, s.data(), src_len, nullptr, 0);
  assert(len > 0);
  std::wstring ret(static_cast<size_t>(len) - 1, '\0');
#pragma warning(disable : 4189)
  const цео r = MultiByteToWideChar(CP_UTF8, 0, s.data(), src_len, (wchar_t*)ret.data(), len);
  assert(len == r);
#pragma warning(default : 4189)
  врати ret;
}
#endif  // #ifdef _WIN32

#ifdef ORT_NO_EXCEPTIONS
void PrintFinalMessage(const char* msg) {
#ако defined(__ANDROID__)
  __android_log_print(ANDROID_LOG_ERROR, "onnxruntime", "%s", msg);
#инако
  // TODO, consider changing the output of the error message from std::cerr to logging when the
  // exceptions are disabled, since using std::cerr might increase binary size, and std::cerr output
  // might not be easily accessible on some systems such as mobile
  // TODO, see if we need to change the output of the error message from std::cerr to NSLog for iOS
  std::cerr << msg << std::endl;
#endif
}
#endif  // #ifdef ORT_NO_EXCEPTIONS

}  // namespace onnxruntime
