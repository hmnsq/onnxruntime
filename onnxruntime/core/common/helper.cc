// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ضمن "core/common/common.h"

#اگر_تعریف_شده _WIN32
#ضمن <Windows.h>
#ضمن <assert.h>
#پایان_اگر

#اگر_تعریف_شده ORT_NO_EXCEPTIONS
#إذا defined(__ANDROID__)
#ضمن <android/log.h>
#وإلا
#ضمن <iostream>
#پایان_اگر
#پایان_اگر

نطاق onnxruntime {
#اگر_تعریف_شده _WIN32
std::string مقصد_UTF8_String(std::wstring_view s) {
  إذا (s.size() >= static_cast<size_t>(std::numeric_limits<صحيح>::max()))
    ORT_THROW("length overflow");

  ثابت صحيح src_len = static_cast<صحيح>(s.size() + 1);
  ثابت صحيح len = Wide_Char_مقصد_Multi_Byte(CP_UTF8, 0, s.داده(), src_len, اشاره_تهی, 0, اشاره_تهی, اشاره_تهی);
  أكد(len > 0);
  std::string ret(static_cast<size_t>(len) - 1, '\0');
#رهنمود warning(disable : 4189)
  ثابت صحيح r = Wide_Char_مقصد_Multi_Byte(CP_UTF8, 0, s.داده(), src_len, (حرف*)ret.داده(), len, اشاره_تهی, اشاره_تهی);
  أكد(len == r);
#رهنمود warning(افتراضي : 4189)
  أرجع ret;
}

std::wstring مقصد_Wide_String(std::string_view s) {
  إذا (s.size() >= static_cast<size_t>(std::numeric_limits<صحيح>::max()))
    ORT_THROW("length overflow");

  ثابت صحيح src_len = static_cast<صحيح>(s.size() + 1);
  ثابت صحيح len = Multi_Byte_مقصد_Wide_Char(CP_UTF8, 0, s.داده(), src_len, اشاره_تهی, 0);
  أكد(len > 0);
  std::wstring ret(static_cast<size_t>(len) - 1, '\0');
#رهنمود warning(disable : 4189)
  ثابت صحيح r = Multi_Byte_مقصد_Wide_Char(CP_UTF8, 0, s.داده(), src_len, (wchar_t*)ret.داده(), len);
  أكد(len == r);
#رهنمود warning(افتراضي : 4189)
  أرجع ret;
}
#پایان_اگر  // #ifdef _WIN32

#اگر_تعریف_شده ORT_NO_EXCEPTIONS
فراغ Print_Final_پیام(ثابت حرف* پیام_2) {
#إذا defined(__ANDROID__)
  __android_log_print(ANDROID_LOG_ERROR, "onnxruntime", "%s", پیام_2);
#وإلا
  // TODO, consider changing the output of the error message from std::cerr to logging when the
  // exceptions are disabled, since using std::cerr might increase binary size, and std::cerr output
  // might not be easily accessible on some systems such as mobile
  // TODO, see if we need to change the output of the error message from std::cerr to NSLog for iOS
  std::cerr << پیام_2 << std::پایان_سطر;
#پایان_اگر
}
#پایان_اگر  // #ifdef ORT_NO_EXCEPTIONS

}  // namespace onnxruntime
