// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#অন্তর্ভাৱ "core/common/common.h"

#ifdef _WIN32
#অন্তর্ভাৱ <Windows.h>
#অন্তর্ভাৱ <assert.h>
#endif

#ifdef ORT_NO_EXCEPTIONS
#যদি defined(__ANDROID__)
#অন্তর্ভাৱ <android/log.h>
#অন্যথা
#অন্তর্ভাৱ <iostream>
#endif
#endif

নামাকাশ onnxruntime {
#ifdef _WIN32
std::string ToUTF8String(std::wstring_view s) {
  যদি (s.size() >= static_cast<size_t>(std::numeric_limits<পূর্ণাঙ্ক>::max()))
    ORT_THROW("length overflow");

  ধ্রুৱ পূর্ণাঙ্ক src_len = static_cast<পূর্ণাঙ্ক>(s.size() + 1);
  ধ্রুৱ পূর্ণাঙ্ক len = WideCharToMultiByte(CP_UTF8, 0, s.data(), src_len, nullptr, 0, nullptr, nullptr);
  প্রতিজ্ঞা(len > 0);
  std::string ret(static_cast<size_t>(len) - 1, '\0');
#pragma warning(disable : 4189)
  ধ্রুৱ পূর্ণাঙ্ক r = WideCharToMultiByte(CP_UTF8, 0, s.data(), src_len, (ৱর্ণ*)ret.data(), len, nullptr, nullptr);
  প্রতিজ্ঞা(len == r);
#pragma warning(স্ৱভাৱ : 4189)
  ঘূৰাই ret;
}

std::wstring ToWideString(std::string_view s) {
  যদি (s.size() >= static_cast<size_t>(std::numeric_limits<পূর্ণাঙ্ক>::max()))
    ORT_THROW("length overflow");

  ধ্রুৱ পূর্ণাঙ্ক src_len = static_cast<পূর্ণাঙ্ক>(s.size() + 1);
  ধ্রুৱ পূর্ণাঙ্ক len = MultiByteToWideChar(CP_UTF8, 0, s.data(), src_len, nullptr, 0);
  প্রতিজ্ঞা(len > 0);
  std::wstring ret(static_cast<size_t>(len) - 1, '\0');
#pragma warning(disable : 4189)
  ধ্রুৱ পূর্ণাঙ্ক r = MultiByteToWideChar(CP_UTF8, 0, s.data(), src_len, (wchar_t*)ret.data(), len);
  প্রতিজ্ঞা(len == r);
#pragma warning(স্ৱভাৱ : 4189)
  ঘূৰাই ret;
}
#endif  // #ifdef _WIN32

#ifdef ORT_NO_EXCEPTIONS
শূন্য PrintFinalMessage(ধ্রুৱ ৱর্ণ* বাৰ্তা) {
#যদি defined(__ANDROID__)
  __android_log_print(ANDROID_LOG_ERROR, "onnxruntime", "%s", বাৰ্তা);
#অন্যথা
  // TODO, consider changing the output of the error message from std::cerr to logging when the
  // exceptions are disabled, since using std::cerr might increase binary size, and std::cerr output
  // might not be easily accessible on some systems such as mobile
  // TODO, see if we need to change the output of the error message from std::cerr to NSLog for iOS
  std::cerr << বাৰ্তা << std::endl;
#endif
}
#endif  // #ifdef ORT_NO_EXCEPTIONS

}  // namespace onnxruntime
