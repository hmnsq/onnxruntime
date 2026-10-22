// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#അന്തര്ഭാവ "core/common/common.h"

#ifdef _WIN32
#അന്തര്ഭാവ <Windows.h>
#അന്തര്ഭാവ <assert.h>
#endif

#ifdef ORT_NO_EXCEPTIONS
#എങ്കിൽ defined(__ANDROID__)
#അന്തര്ഭാവ <android/log.h>
#അല്ലെങ്കിൽ
#അന്തര്ഭാവ <iostream>
#endif
#endif

നാമാകാശ onnxruntime {
#ifdef _WIN32
std::string ToUTF8String(std::wstring_view s) {
  എങ്കിൽ (s.size() >= static_cast<size_t>(std::numeric_limits<പൂർണ്ണസംഖ്യ>::max()))
    ORT_THROW("length overflow");

  സ്ഥിരം പൂർണ്ണസംഖ്യ src_len = static_cast<പൂർണ്ണസംഖ്യ>(s.size() + 1);
  സ്ഥിരം പൂർണ്ണസംഖ്യ len = WideCharToMultiByte(CP_UTF8, 0, s.data(), src_len, nullptr, 0, nullptr, nullptr);
  പ്രതിജ്ഞാ(len > 0);
  std::string ret(static_cast<size_t>(len) - 1, '\0');
#pragma warning(disable : 4189)
  സ്ഥിരം പൂർണ്ണസംഖ്യ r = WideCharToMultiByte(CP_UTF8, 0, s.data(), src_len, (അക്ഷരം*)ret.data(), len, nullptr, nullptr);
  പ്രതിജ്ഞാ(len == r);
#pragma warning(സ്ഥിരസ്ഥിതി : 4189)
  തിരികെ ret;
}

std::wstring ToWideString(std::string_view s) {
  എങ്കിൽ (s.size() >= static_cast<size_t>(std::numeric_limits<പൂർണ്ണസംഖ്യ>::max()))
    ORT_THROW("length overflow");

  സ്ഥിരം പൂർണ്ണസംഖ്യ src_len = static_cast<പൂർണ്ണസംഖ്യ>(s.size() + 1);
  സ്ഥിരം പൂർണ്ണസംഖ്യ len = MultiByteToWideChar(CP_UTF8, 0, s.data(), src_len, nullptr, 0);
  പ്രതിജ്ഞാ(len > 0);
  std::wstring ret(static_cast<size_t>(len) - 1, '\0');
#pragma warning(disable : 4189)
  സ്ഥിരം പൂർണ്ണസംഖ്യ r = MultiByteToWideChar(CP_UTF8, 0, s.data(), src_len, (wchar_t*)ret.data(), len);
  പ്രതിജ്ഞാ(len == r);
#pragma warning(സ്ഥിരസ്ഥിതി : 4189)
  തിരികെ ret;
}
#endif  // #ifdef _WIN32

#ifdef ORT_NO_EXCEPTIONS
ശൂന്യം PrintFinalMessage(സ്ഥിരം അക്ഷരം* സന്ദേശം) {
#എങ്കിൽ defined(__ANDROID__)
  __android_log_print(ANDROID_LOG_ERROR, "onnxruntime", "%s", സന്ദേശം);
#അല്ലെങ്കിൽ
  // TODO, consider changing the output of the error message from std::cerr to logging when the
  // exceptions are disabled, since using std::cerr might increase binary size, and std::cerr output
  // might not be easily accessible on some systems such as mobile
  // TODO, see if we need to change the output of the error message from std::cerr to NSLog for iOS
  std::cerr << സന്ദേശം << std::endl;
#endif
}
#endif  // #ifdef ORT_NO_EXCEPTIONS

}  // namespace onnxruntime
