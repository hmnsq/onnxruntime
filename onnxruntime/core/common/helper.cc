// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ਅਨ੍ਤਰ੍ਭਾਵ "core/common/common.h"

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _WIN32
#ਅਨ੍ਤਰ੍ਭਾਵ <Windows.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <assert.h>
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ORT_NO_EXCEPTIONS
#ਜੇ defined(__ANDROID__)
#ਅਨ੍ਤਰ੍ਭਾਵ <android/log.h>
#ਨਹੀਂਤਾਂ
#ਅਨ੍ਤਰ੍ਭਾਵ <iostream>
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

ਨਾਮਾਕਾਸ਼ onnxruntime {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _WIN32
std::string ਮੰਜ਼ਿਲ_UTF8_String(std::wstring_view s) {
  ਜੇ (s.size() >= static_cast<size_t>(std::numeric_limits<ਪੂਰਨ>::max()))
    ORT_THROW("length overflow");

  ਅਚਲ ਪੂਰਨ src_len = static_cast<ਪੂਰਨ>(s.size() + 1);
  ਅਚਲ ਪੂਰਨ len = Wide_Char_ਮੰਜ਼ਿਲ_Multi_Byte(CP_UTF8, 0, s.ਅੰਕੜੇ(), src_len, ਖਾਲੀ_ਸੂਚਕ, 0, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
  ਪ੍ਰਤਿਜ੍ਞਾ(len > 0);
  std::string ret(static_cast<size_t>(len) - 1, '\0');
#ਹਦਾਇਤ warning(disable : 4189)
  ਅਚਲ ਪੂਰਨ r = Wide_Char_ਮੰਜ਼ਿਲ_Multi_Byte(CP_UTF8, 0, s.ਅੰਕੜੇ(), src_len, (ਅੱਖਰ*)ret.ਅੰਕੜੇ(), len, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
  ਪ੍ਰਤਿਜ੍ਞਾ(len == r);
#ਹਦਾਇਤ warning(ਮੂਲ : 4189)
  ਵਾਪਸ ret;
}

std::wstring ਮੰਜ਼ਿਲ_Wide_String(std::string_view s) {
  ਜੇ (s.size() >= static_cast<size_t>(std::numeric_limits<ਪੂਰਨ>::max()))
    ORT_THROW("length overflow");

  ਅਚਲ ਪੂਰਨ src_len = static_cast<ਪੂਰਨ>(s.size() + 1);
  ਅਚਲ ਪੂਰਨ len = Multi_Byte_ਮੰਜ਼ਿਲ_Wide_Char(CP_UTF8, 0, s.ਅੰਕੜੇ(), src_len, ਖਾਲੀ_ਸੂਚਕ, 0);
  ਪ੍ਰਤਿਜ੍ਞਾ(len > 0);
  std::wstring ret(static_cast<size_t>(len) - 1, '\0');
#ਹਦਾਇਤ warning(disable : 4189)
  ਅਚਲ ਪੂਰਨ r = Multi_Byte_ਮੰਜ਼ਿਲ_Wide_Char(CP_UTF8, 0, s.ਅੰਕੜੇ(), src_len, (wchar_t*)ret.ਅੰਕੜੇ(), len);
  ਪ੍ਰਤਿਜ੍ਞਾ(len == r);
#ਹਦਾਇਤ warning(ਮੂਲ : 4189)
  ਵਾਪਸ ret;
}
#ਜੇ_ਅੰਤ  // #ifdef _WIN32

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ORT_NO_EXCEPTIONS
ਖਾਲੀ Print_Final_ਸੁਨੇਹਾ(ਅਚਲ ਅੱਖਰ* ਸੁਨੇਹਾ_2) {
#ਜੇ defined(__ANDROID__)
  __android_log_print(ANDROID_LOG_ERROR, "onnxruntime", "%s", ਸੁਨੇਹਾ_2);
#ਨਹੀਂਤਾਂ
  // TODO, consider changing the output of the error message from std::cerr to logging when the
  // exceptions are disabled, since using std::cerr might increase binary size, and std::cerr output
  // might not be easily accessible on some systems such as mobile
  // TODO, see if we need to change the output of the error message from std::cerr to NSLog for iOS
  std::cerr << ਸੁਨੇਹਾ_2 << std::ਸਤਰ_ਅੰਤ;
#ਜੇ_ਅੰਤ
}
#ਜੇ_ਅੰਤ  // #ifdef ORT_NO_EXCEPTIONS

}  // namespace onnxruntime
