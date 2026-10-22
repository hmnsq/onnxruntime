// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#અન્તર્ભાવ "core/common/common.h"

#ifdef _WIN32
#અન્તર્ભાવ <Windows.h>
#અન્તર્ભાવ <assert.h>
#endif

#ifdef ORT_NO_EXCEPTIONS
#જો defined(__ANDROID__)
#અન્તર્ભાવ <android/log.h>
#નહીંતર
#અન્તર્ભાવ <iostream>
#endif
#endif

નામાકાશ onnxruntime {
#ifdef _WIN32
std::string ToUTF8String(std::wstring_view s) {
  જો (s.size() >= static_cast<size_t>(std::numeric_limits<પૂર્ણાંક>::max()))
    ORT_THROW("length overflow");

  અચલ પૂર્ણાંક src_len = static_cast<પૂર્ણાંક>(s.size() + 1);
  અચલ પૂર્ણાંક len = WideCharToMultiByte(CP_UTF8, 0, s.data(), src_len, nullptr, 0, nullptr, nullptr);
  પ્રતિજ્ઞા(len > 0);
  std::string ret(static_cast<size_t>(len) - 1, '\0');
#pragma warning(disable : 4189)
  અચલ પૂર્ણાંક r = WideCharToMultiByte(CP_UTF8, 0, s.data(), src_len, (અક્ષર*)ret.data(), len, nullptr, nullptr);
  પ્રતિજ્ઞા(len == r);
#pragma warning(મૂળભૂત : 4189)
  પાછા ret;
}

std::wstring ToWideString(std::string_view s) {
  જો (s.size() >= static_cast<size_t>(std::numeric_limits<પૂર્ણાંક>::max()))
    ORT_THROW("length overflow");

  અચલ પૂર્ણાંક src_len = static_cast<પૂર્ણાંક>(s.size() + 1);
  અચલ પૂર્ણાંક len = MultiByteToWideChar(CP_UTF8, 0, s.data(), src_len, nullptr, 0);
  પ્રતિજ્ઞા(len > 0);
  std::wstring ret(static_cast<size_t>(len) - 1, '\0');
#pragma warning(disable : 4189)
  અચલ પૂર્ણાંક r = MultiByteToWideChar(CP_UTF8, 0, s.data(), src_len, (wchar_t*)ret.data(), len);
  પ્રતિજ્ઞા(len == r);
#pragma warning(મૂળભૂત : 4189)
  પાછા ret;
}
#endif  // #ifdef _WIN32

#ifdef ORT_NO_EXCEPTIONS
શૂન્ય PrintFinalMessage(અચલ અક્ષર* સંદેશ) {
#જો defined(__ANDROID__)
  __android_log_print(ANDROID_LOG_ERROR, "onnxruntime", "%s", સંદેશ);
#નહીંતર
  // TODO, consider changing the output of the error message from std::cerr to logging when the
  // exceptions are disabled, since using std::cerr might increase binary size, and std::cerr output
  // might not be easily accessible on some systems such as mobile
  // TODO, see if we need to change the output of the error message from std::cerr to NSLog for iOS
  std::cerr << સંદેશ << std::endl;
#endif
}
#endif  // #ifdef ORT_NO_EXCEPTIONS

}  // namespace onnxruntime
