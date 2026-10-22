// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#شامل "core/common/common.h"

#اگر_تعریف_شدہ _WIN32
#شامل <Windows.h>
#شامل <assert.h>
#اگر_ختم

#اگر_تعریف_شدہ ORT_NO_EXCEPTIONS
#اگر defined(__ANDROID__)
#شامل <android/log.h>
#ورنہ
#شامل <iostream>
#اگر_ختم
#اگر_ختم

نامخلا onnxruntime {
#اگر_تعریف_شدہ _WIN32
std::string مقصد_UTF8_String(std::wstring_view s) {
  اگر (s.size() >= static_cast<size_t>(std::numeric_limits<صحیح>::max()))
    ORT_THROW("length overflow");

  ثابت صحیح src_len = static_cast<صحیح>(s.size() + 1);
  ثابت صحیح len = Wide_Char_مقصد_Multi_Byte(CP_UTF8, 0, s.ڈیٹا(), src_len, خالی_اشاریہ, 0, خالی_اشاریہ, خالی_اشاریہ);
  أكد(len > 0);
  std::string ret(static_cast<size_t>(len) - 1, '\0');
#ہدایت warning(disable : 4189)
  ثابت صحیح r = Wide_Char_مقصد_Multi_Byte(CP_UTF8, 0, s.ڈیٹا(), src_len, (حرف*)ret.ڈیٹا(), len, خالی_اشاریہ, خالی_اشاریہ);
  أكد(len == r);
#ہدایت warning(طےشدہ : 4189)
  واپس ret;
}

std::wstring مقصد_Wide_String(std::string_view s) {
  اگر (s.size() >= static_cast<size_t>(std::numeric_limits<صحیح>::max()))
    ORT_THROW("length overflow");

  ثابت صحیح src_len = static_cast<صحیح>(s.size() + 1);
  ثابت صحیح len = Multi_Byte_مقصد_Wide_Char(CP_UTF8, 0, s.ڈیٹا(), src_len, خالی_اشاریہ, 0);
  أكد(len > 0);
  std::wstring ret(static_cast<size_t>(len) - 1, '\0');
#ہدایت warning(disable : 4189)
  ثابت صحیح r = Multi_Byte_مقصد_Wide_Char(CP_UTF8, 0, s.ڈیٹا(), src_len, (wchar_t*)ret.ڈیٹا(), len);
  أكد(len == r);
#ہدایت warning(طےشدہ : 4189)
  واپس ret;
}
#اگر_ختم  // #ifdef _WIN32

#اگر_تعریف_شدہ ORT_NO_EXCEPTIONS
خالی Print_Final_پیغام(ثابت حرف* پیغام_2) {
#اگر defined(__ANDROID__)
  __android_log_print(ANDROID_LOG_ERROR, "onnxruntime", "%s", پیغام_2);
#ورنہ
  // TODO, consider changing the output of the error message from std::cerr to logging when the
  // exceptions are disabled, since using std::cerr might increase binary size, and std::cerr output
  // might not be easily accessible on some systems such as mobile
  // TODO, see if we need to change the output of the error message from std::cerr to NSLog for iOS
  std::cerr << پیغام_2 << std::سطر_ختم;
#اگر_ختم
}
#اگر_ختم  // #ifdef ORT_NO_EXCEPTIONS

}  // namespace onnxruntime
