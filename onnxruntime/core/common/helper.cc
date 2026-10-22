// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#அந்தர்பாவ "core/common/common.h"

#ifdef _WIN32
#அந்தர்பாவ <Windows.h>
#அந்தர்பாவ <assert.h>
#endif

#ifdef ORT_NO_EXCEPTIONS
#எனில் defined(__ANDROID__)
#அந்தர்பாவ <android/log.h>
#இல்லையெனில்
#அந்தர்பாவ <iostream>
#endif
#endif

நாமாகாஶ onnxruntime {
#ifdef _WIN32
std::string ToUTF8String(std::wstring_view s) {
  எனில் (s.size() >= static_cast<size_t>(std::numeric_limits<முழு>::max()))
    ORT_THROW("length overflow");

  மாறிலி முழு src_len = static_cast<முழு>(s.size() + 1);
  மாறிலி முழு len = WideCharToMultiByte(CP_UTF8, 0, s.data(), src_len, nullptr, 0, nullptr, nullptr);
  ப்ரதிஜ்ஞா(len > 0);
  std::string ret(static_cast<size_t>(len) - 1, '\0');
#pragma warning(disable : 4189)
  மாறிலி முழு r = WideCharToMultiByte(CP_UTF8, 0, s.data(), src_len, (எழுத்து*)ret.data(), len, nullptr, nullptr);
  ப்ரதிஜ்ஞா(len == r);
#pragma warning(இயல்பு : 4189)
  திரும்பு ret;
}

std::wstring ToWideString(std::string_view s) {
  எனில் (s.size() >= static_cast<size_t>(std::numeric_limits<முழு>::max()))
    ORT_THROW("length overflow");

  மாறிலி முழு src_len = static_cast<முழு>(s.size() + 1);
  மாறிலி முழு len = MultiByteToWideChar(CP_UTF8, 0, s.data(), src_len, nullptr, 0);
  ப்ரதிஜ்ஞா(len > 0);
  std::wstring ret(static_cast<size_t>(len) - 1, '\0');
#pragma warning(disable : 4189)
  மாறிலி முழு r = MultiByteToWideChar(CP_UTF8, 0, s.data(), src_len, (wchar_t*)ret.data(), len);
  ப்ரதிஜ்ஞா(len == r);
#pragma warning(இயல்பு : 4189)
  திரும்பு ret;
}
#endif  // #ifdef _WIN32

#ifdef ORT_NO_EXCEPTIONS
வெறுமை PrintFinalMessage(மாறிலி எழுத்து* செய்தி) {
#எனில் defined(__ANDROID__)
  __android_log_print(ANDROID_LOG_ERROR, "onnxruntime", "%s", செய்தி);
#இல்லையெனில்
  // TODO, consider changing the output of the error message from std::cerr to logging when the
  // exceptions are disabled, since using std::cerr might increase binary size, and std::cerr output
  // might not be easily accessible on some systems such as mobile
  // TODO, see if we need to change the output of the error message from std::cerr to NSLog for iOS
  std::cerr << செய்தி << std::endl;
#endif
}
#endif  // #ifdef ORT_NO_EXCEPTIONS

}  // namespace onnxruntime
