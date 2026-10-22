// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#شامل "core/common/common.h"

#ifdef _WIN32
#شامل <Windows.h>
#شامل <assert.h>
#endif

#ifdef ORT_NO_EXCEPTIONS
#اگر defined(__ANDROID__)
#شامل <android/log.h>
#وگرنه
#شامل <iostream>
#endif
#endif

فضاینام onnxruntime {
#ifdef _WIN32
std::string ToUTF8String(std::wstring_view s) {
  اگر (s.size() >= static_cast<size_t>(std::numeric_limits<صحیح>::max()))
    ORT_THROW("length overflow");

  ثابت صحیح src_len = static_cast<صحیح>(s.size() + 1);
  ثابت صحیح len = WideCharToMultiByte(CP_UTF8, 0, s.data(), src_len, nullptr, 0, nullptr, nullptr);
  أكد(len > 0);
  std::string ret(static_cast<size_t>(len) - 1, '\0');
#pragma warning(disable : 4189)
  ثابت صحیح r = WideCharToMultiByte(CP_UTF8, 0, s.data(), src_len, (نویسه*)ret.data(), len, nullptr, nullptr);
  أكد(len == r);
#pragma warning(پیشفرض : 4189)
  بازگشت ret;
}

std::wstring ToWideString(std::string_view s) {
  اگر (s.size() >= static_cast<size_t>(std::numeric_limits<صحیح>::max()))
    ORT_THROW("length overflow");

  ثابت صحیح src_len = static_cast<صحیح>(s.size() + 1);
  ثابت صحیح len = MultiByteToWideChar(CP_UTF8, 0, s.data(), src_len, nullptr, 0);
  أكد(len > 0);
  std::wstring ret(static_cast<size_t>(len) - 1, '\0');
#pragma warning(disable : 4189)
  ثابت صحیح r = MultiByteToWideChar(CP_UTF8, 0, s.data(), src_len, (wchar_t*)ret.data(), len);
  أكد(len == r);
#pragma warning(پیشفرض : 4189)
  بازگشت ret;
}
#endif  // #ifdef _WIN32

#ifdef ORT_NO_EXCEPTIONS
تهی PrintFinalMessage(ثابت نویسه* پیام) {
#اگر defined(__ANDROID__)
  __android_log_print(ANDROID_LOG_ERROR, "onnxruntime", "%s", پیام);
#وگرنه
  // TODO, consider changing the output of the error message from std::cerr to logging when the
  // exceptions are disabled, since using std::cerr might increase binary size, and std::cerr output
  // might not be easily accessible on some systems such as mobile
  // TODO, see if we need to change the output of the error message from std::cerr to NSLog for iOS
  std::cerr << پیام << std::endl;
#endif
}
#endif  // #ifdef ORT_NO_EXCEPTIONS

}  // namespace onnxruntime
