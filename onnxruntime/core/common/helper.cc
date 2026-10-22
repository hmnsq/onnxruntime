// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#অন্তর্ভাব "core/common/common.h"

#ifdef _WIN32
#অন্তর্ভাব <Windows.h>
#অন্তর্ভাব <assert.h>
#endif

#ifdef ORT_NO_EXCEPTIONS
#যদি defined(__ANDROID__)
#অন্তর্ভাব <android/log.h>
#অন্যথা
#অন্তর্ভাব <iostream>
#endif
#endif

নামাকাশ onnxruntime {
#ifdef _WIN32
std::string ToUTF8String(std::wstring_view s) {
  যদি (s.size() >= static_cast<size_t>(std::numeric_limits<পূর্ণসংখ্যা>::max()))
    ORT_THROW("length overflow");

  ধ্রুবক পূর্ণসংখ্যা src_len = static_cast<পূর্ণসংখ্যা>(s.size() + 1);
  ধ্রুবক পূর্ণসংখ্যা len = WideCharToMultiByte(CP_UTF8, 0, s.তথ্য(), src_len, nullptr, 0, nullptr, nullptr);
  প্রতিজ্ঞা(len > 0);
  std::string ret(static_cast<size_t>(len) - 1, '\0');
#pragma warning(disable : 4189)
  ধ্রুবক পূর্ণসংখ্যা r = WideCharToMultiByte(CP_UTF8, 0, s.তথ্য(), src_len, (অক্ষর*)ret.তথ্য(), len, nullptr, nullptr);
  প্রতিজ্ঞা(len == r);
#pragma warning(পূর্বনির্ধারিত : 4189)
  ফেরত ret;
}

std::wstring ToWideString(std::string_view s) {
  যদি (s.size() >= static_cast<size_t>(std::numeric_limits<পূর্ণসংখ্যা>::max()))
    ORT_THROW("length overflow");

  ধ্রুবক পূর্ণসংখ্যা src_len = static_cast<পূর্ণসংখ্যা>(s.size() + 1);
  ধ্রুবক পূর্ণসংখ্যা len = MultiByteToWideChar(CP_UTF8, 0, s.তথ্য(), src_len, nullptr, 0);
  প্রতিজ্ঞা(len > 0);
  std::wstring ret(static_cast<size_t>(len) - 1, '\0');
#pragma warning(disable : 4189)
  ধ্রুবক পূর্ণসংখ্যা r = MultiByteToWideChar(CP_UTF8, 0, s.তথ্য(), src_len, (wchar_t*)ret.তথ্য(), len);
  প্রতিজ্ঞা(len == r);
#pragma warning(পূর্বনির্ধারিত : 4189)
  ফেরত ret;
}
#endif  // #ifdef _WIN32

#ifdef ORT_NO_EXCEPTIONS
শূন্য PrintFinalMessage(ধ্রুবক অক্ষর* বার্তা) {
#যদি defined(__ANDROID__)
  __android_log_print(ANDROID_LOG_ERROR, "onnxruntime", "%s", বার্তা);
#অন্যথা
  // TODO, consider changing the output of the error message from std::cerr to logging when the
  // exceptions are disabled, since using std::cerr might increase binary size, and std::cerr output
  // might not be easily accessible on some systems such as mobile
  // TODO, see if we need to change the output of the error message from std::cerr to NSLog for iOS
  std::cerr << বার্তা << std::endl;
#endif
}
#endif  // #ifdef ORT_NO_EXCEPTIONS

}  // namespace onnxruntime
