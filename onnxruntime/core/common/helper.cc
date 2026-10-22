// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ضمن "core/common/common.h"

#ifdef _WIN32
#ضمن <Windows.h>
#ضمن <assert.h>
#endif

#ifdef ORT_NO_EXCEPTIONS
#إذا defined(__ANDROID__)
#ضمن <android/log.h>
#وإلا
#ضمن <iostream>
#endif
#endif

نطاق onnxruntime {
#ifdef _WIN32
std::string ToUTF8String(std::wstring_view s) {
  إذا (s.size() >= static_cast<size_t>(std::numeric_limits<صحيح>::max()))
    ORT_THROW("length overflow");

  ثابت صحيح src_len = static_cast<صحيح>(s.size() + 1);
  ثابت صحيح len = WideCharToMultiByte(CP_UTF8, 0, s.data(), src_len, nullptr, 0, nullptr, nullptr);
  أكد(len > 0);
  std::string ret(static_cast<size_t>(len) - 1, '\0');
#pragma warning(disable : 4189)
  ثابت صحيح r = WideCharToMultiByte(CP_UTF8, 0, s.data(), src_len, (حرف*)ret.data(), len, nullptr, nullptr);
  أكد(len == r);
#pragma warning(افتراضي : 4189)
  إرجاع ret;
}

std::wstring ToWideString(std::string_view s) {
  إذا (s.size() >= static_cast<size_t>(std::numeric_limits<صحيح>::max()))
    ORT_THROW("length overflow");

  ثابت صحيح src_len = static_cast<صحيح>(s.size() + 1);
  ثابت صحيح len = MultiByteToWideChar(CP_UTF8, 0, s.data(), src_len, nullptr, 0);
  أكد(len > 0);
  std::wstring ret(static_cast<size_t>(len) - 1, '\0');
#pragma warning(disable : 4189)
  ثابت صحيح r = MultiByteToWideChar(CP_UTF8, 0, s.data(), src_len, (wchar_t*)ret.data(), len);
  أكد(len == r);
#pragma warning(افتراضي : 4189)
  إرجاع ret;
}
#endif  // #ifdef _WIN32

#ifdef ORT_NO_EXCEPTIONS
فراغ PrintFinalMessage(ثابت حرف* رسالة) {
#إذا defined(__ANDROID__)
  __android_log_print(ANDROID_LOG_ERROR, "onnxruntime", "%s", رسالة);
#وإلا
  // TODO, consider changing the output of the error message from std::cerr to logging when the
  // exceptions are disabled, since using std::cerr might increase binary size, and std::cerr output
  // might not be easily accessible on some systems such as mobile
  // TODO, see if we need to change the output of the error message from std::cerr to NSLog for iOS
  std::cerr << رسالة << std::endl;
#endif
}
#endif  // #ifdef ORT_NO_EXCEPTIONS

}  // namespace onnxruntime
