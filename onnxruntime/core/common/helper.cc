// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ضمن "core/common/common.h"

#جے_تعریف_شدہ _WIN32
#ضمن <Windows.h>
#ضمن <assert.h>
#جے_انت

#جے_تعریف_شدہ ORT_NO_EXCEPTIONS
#إذا defined(__ANDROID__)
#ضمن <android/log.h>
#وإلا
#ضمن <iostream>
#جے_انت
#جے_انت

نطاق onnxruntime {
#جے_تعریف_شدہ _WIN32
std::string منزل_UTF8_String(std::wstring_view s) {
  إذا (s.size() >= static_cast<size_t>(std::numeric_limits<صحيح>::max()))
    ORT_THROW("length overflow");

  ثابت صحيح src_len = static_cast<صحيح>(s.size() + 1);
  ثابت صحيح len = Wide_Char_منزل_Multi_Byte(CP_UTF8, 0, s.انکڑے(), src_len, خالی_سوچک, 0, خالی_سوچک, خالی_سوچک);
  أكد(len > 0);
  std::string ret(static_cast<size_t>(len) - 1, '\0');
#ہدایت warning(disable : 4189)
  ثابت صحيح r = Wide_Char_منزل_Multi_Byte(CP_UTF8, 0, s.انکڑے(), src_len, (حرف*)ret.انکڑے(), len, خالی_سوچک, خالی_سوچک);
  أكد(len == r);
#ہدایت warning(افتراضي : 4189)
  أرجع ret;
}

std::wstring منزل_Wide_String(std::string_view s) {
  إذا (s.size() >= static_cast<size_t>(std::numeric_limits<صحيح>::max()))
    ORT_THROW("length overflow");

  ثابت صحيح src_len = static_cast<صحيح>(s.size() + 1);
  ثابت صحيح len = Multi_Byte_منزل_Wide_Char(CP_UTF8, 0, s.انکڑے(), src_len, خالی_سوچک, 0);
  أكد(len > 0);
  std::wstring ret(static_cast<size_t>(len) - 1, '\0');
#ہدایت warning(disable : 4189)
  ثابت صحيح r = Multi_Byte_منزل_Wide_Char(CP_UTF8, 0, s.انکڑے(), src_len, (wchar_t*)ret.انکڑے(), len);
  أكد(len == r);
#ہدایت warning(افتراضي : 4189)
  أرجع ret;
}
#جے_انت  // #ifdef _WIN32

#جے_تعریف_شدہ ORT_NO_EXCEPTIONS
فراغ Print_Final_سنیہا(ثابت حرف* پیغام) {
#إذا defined(__ANDROID__)
  __android_log_print(ANDROID_LOG_ERROR, "onnxruntime", "%s", پیغام);
#وإلا
  // TODO, consider changing the output of the error message from std::cerr to logging when the
  // exceptions are disabled, since using std::cerr might increase binary size, and std::cerr output
  // might not be easily accessible on some systems such as mobile
  // TODO, see if we need to change the output of the error message from std::cerr to NSLog for iOS
  std::cerr << پیغام << std::سطر_انت;
#جے_انت
}
#جے_انت  // #ifdef ORT_NO_EXCEPTIONS

}  // namespace onnxruntime
