// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ಅನ್ತರ್ಭಾವ "core/common/common.h"

#ifdef _WIN32
#ಅನ್ತರ್ಭಾವ <Windows.h>
#ಅನ್ತರ್ಭಾವ <assert.h>
#endif

#ifdef ORT_NO_EXCEPTIONS
#ಯದಿ defined(__ANDROID__)
#ಅನ್ತರ್ಭಾವ <android/log.h>
#ಅನ್ಯಥಾ
#ಅನ್ತರ್ಭಾವ <iostream>
#endif
#endif

ನಾಮಾಕಾಶ onnxruntime {
#ifdef _WIN32
std::string ToUTF8String(std::wstring_view s) {
  ಯದಿ (s.size() >= static_cast<size_t>(std::numeric_limits<ಪೂರ್ಣಾಙ್ಕ>::max()))
    ORT_THROW("length overflow");

  ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ src_len = static_cast<ಪೂರ್ಣಾಙ್ಕ>(s.size() + 1);
  ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ len = WideCharToMultiByte(CP_UTF8, 0, s.data(), src_len, nullptr, 0, nullptr, nullptr);
  ಪ್ರತಿಜ್ಞಾ(len > 0);
  std::string ret(static_cast<size_t>(len) - 1, '\0');
#pragma warning(disable : 4189)
  ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ r = WideCharToMultiByte(CP_UTF8, 0, s.data(), src_len, (ವರ್ಣ*)ret.data(), len, nullptr, nullptr);
  ಪ್ರತಿಜ್ಞಾ(len == r);
#pragma warning(ಸ್ವಭಾವ : 4189)
  ಪ್ರತ್ಯಾಗಮ ret;
}

std::wstring ToWideString(std::string_view s) {
  ಯದಿ (s.size() >= static_cast<size_t>(std::numeric_limits<ಪೂರ್ಣಾಙ್ಕ>::max()))
    ORT_THROW("length overflow");

  ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ src_len = static_cast<ಪೂರ್ಣಾಙ್ಕ>(s.size() + 1);
  ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ len = MultiByteToWideChar(CP_UTF8, 0, s.data(), src_len, nullptr, 0);
  ಪ್ರತಿಜ್ಞಾ(len > 0);
  std::wstring ret(static_cast<size_t>(len) - 1, '\0');
#pragma warning(disable : 4189)
  ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ r = MultiByteToWideChar(CP_UTF8, 0, s.data(), src_len, (wchar_t*)ret.data(), len);
  ಪ್ರತಿಜ್ಞಾ(len == r);
#pragma warning(ಸ್ವಭಾವ : 4189)
  ಪ್ರತ್ಯಾಗಮ ret;
}
#endif  // #ifdef _WIN32

#ifdef ORT_NO_EXCEPTIONS
ಶೂನ್ಯ PrintFinalMessage(ಧ್ರುವ ವರ್ಣ* ಸಂದೇಶ) {
#ಯದಿ defined(__ANDROID__)
  __android_log_print(ANDROID_LOG_ERROR, "onnxruntime", "%s", ಸಂದೇಶ);
#ಅನ್ಯಥಾ
  // TODO, consider changing the output of the error message from std::cerr to logging when the
  // exceptions are disabled, since using std::cerr might increase binary size, and std::cerr output
  // might not be easily accessible on some systems such as mobile
  // TODO, see if we need to change the output of the error message from std::cerr to NSLog for iOS
  std::cerr << ಸಂದೇಶ << std::endl;
#endif
}
#endif  // #ifdef ORT_NO_EXCEPTIONS

}  // namespace onnxruntime
