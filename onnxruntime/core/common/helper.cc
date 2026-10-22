// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ଅନ୍ତର୍ଭାଵ "core/common/common.h"

#ifdef _WIN32
#ଅନ୍ତର୍ଭାଵ <Windows.h>
#ଅନ୍ତର୍ଭାଵ <assert.h>
#endif

#ifdef ORT_NO_EXCEPTIONS
#ଯଦି defined(__ANDROID__)
#ଅନ୍ତର୍ଭାଵ <android/log.h>
#ଅନ୍ୟଥା
#ଅନ୍ତର୍ଭାଵ <iostream>
#endif
#endif

ନାମାକାଶ onnxruntime {
#ifdef _WIN32
std::string ToUTF8String(std::wstring_view s) {
  ଯଦି (s.size() >= static_cast<size_t>(std::numeric_limits<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>::max()))
    ORT_THROW("length overflow");

  ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା src_len = static_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(s.size() + 1);
  ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା len = WideCharToMultiByte(CP_UTF8, 0, s.data(), src_len, nullptr, 0, nullptr, nullptr);
  ପ୍ରତିଜ୍ଞା(len > 0);
  std::string ret(static_cast<size_t>(len) - 1, '\0');
#pragma warning(disable : 4189)
  ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା r = WideCharToMultiByte(CP_UTF8, 0, s.data(), src_len, (ଅକ୍ଷର*)ret.data(), len, nullptr, nullptr);
  ପ୍ରତିଜ୍ଞା(len == r);
#pragma warning(ପୂର୍ବନିର୍ଦ୍ଧାରିତ : 4189)
  ଫେରନ୍ତୁ ret;
}

std::wstring ToWideString(std::string_view s) {
  ଯଦି (s.size() >= static_cast<size_t>(std::numeric_limits<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>::max()))
    ORT_THROW("length overflow");

  ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା src_len = static_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(s.size() + 1);
  ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା len = MultiByteToWideChar(CP_UTF8, 0, s.data(), src_len, nullptr, 0);
  ପ୍ରତିଜ୍ଞା(len > 0);
  std::wstring ret(static_cast<size_t>(len) - 1, '\0');
#pragma warning(disable : 4189)
  ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା r = MultiByteToWideChar(CP_UTF8, 0, s.data(), src_len, (wchar_t*)ret.data(), len);
  ପ୍ରତିଜ୍ଞା(len == r);
#pragma warning(ପୂର୍ବନିର୍ଦ୍ଧାରିତ : 4189)
  ଫେରନ୍ତୁ ret;
}
#endif  // #ifdef _WIN32

#ifdef ORT_NO_EXCEPTIONS
ଶୂନ୍ୟ PrintFinalMessage(ଅଚଳ ଅକ୍ଷର* ସନ୍ଦେଶ) {
#ଯଦି defined(__ANDROID__)
  __android_log_print(ANDROID_LOG_ERROR, "onnxruntime", "%s", ସନ୍ଦେଶ);
#ଅନ୍ୟଥା
  // TODO, consider changing the output of the error message from std::cerr to logging when the
  // exceptions are disabled, since using std::cerr might increase binary size, and std::cerr output
  // might not be easily accessible on some systems such as mobile
  // TODO, see if we need to change the output of the error message from std::cerr to NSLog for iOS
  std::cerr << ସନ୍ଦେଶ << std::endl;
#endif
}
#endif  // #ifdef ORT_NO_EXCEPTIONS

}  // namespace onnxruntime
