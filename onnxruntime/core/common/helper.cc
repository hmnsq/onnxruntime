// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ضمن "core/common/common.h"

#جيڪڏهن_وصف_ٿيل _WIN32
#ضمن <Windows.h>
#ضمن <assert.h>
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل ORT_NO_EXCEPTIONS
#إذا defined(__ANDROID__)
#ضمن <android/log.h>
#وإلا
#ضمن <iostream>
#جيڪڏهن_پورو
#جيڪڏهن_پورو

نطاق onnxruntime {
#جيڪڏهن_وصف_ٿيل _WIN32
std::string منزل_UTF8_String(std::wstring_view s) {
  إذا (s.size() >= static_cast<size_t>(std::numeric_limits<صحيح>::max()))
    ORT_THROW("length overflow");

  ثابت صحيح src_len = static_cast<صحيح>(s.size() + 1);
  ثابت صحيح len = Wide_Char_منزل_Multi_Byte(CP_UTF8, 0, s.معلومات(), src_len, خالي_اشارو, 0, خالي_اشارو, خالي_اشارو);
  أكد(len > 0);
  std::string ret(static_cast<size_t>(len) - 1, '\0');
#هدايت warning(disable : 4189)
  ثابت صحيح r = Wide_Char_منزل_Multi_Byte(CP_UTF8, 0, s.معلومات(), src_len, (حرف*)ret.معلومات(), len, خالي_اشارو, خالي_اشارو);
  أكد(len == r);
#هدايت warning(افتراضي : 4189)
  أرجع ret;
}

std::wstring منزل_Wide_String(std::string_view s) {
  إذا (s.size() >= static_cast<size_t>(std::numeric_limits<صحيح>::max()))
    ORT_THROW("length overflow");

  ثابت صحيح src_len = static_cast<صحيح>(s.size() + 1);
  ثابت صحيح len = Multi_Byte_منزل_Wide_Char(CP_UTF8, 0, s.معلومات(), src_len, خالي_اشارو, 0);
  أكد(len > 0);
  std::wstring ret(static_cast<size_t>(len) - 1, '\0');
#هدايت warning(disable : 4189)
  ثابت صحيح r = Multi_Byte_منزل_Wide_Char(CP_UTF8, 0, s.معلومات(), src_len, (wchar_t*)ret.معلومات(), len);
  أكد(len == r);
#هدايت warning(افتراضي : 4189)
  أرجع ret;
}
#جيڪڏهن_پورو  // #ifdef _WIN32

#جيڪڏهن_وصف_ٿيل ORT_NO_EXCEPTIONS
فراغ Print_Final_پيغام(ثابت حرف* نياپو) {
#إذا defined(__ANDROID__)
  __android_log_print(ANDROID_LOG_ERROR, "onnxruntime", "%s", نياپو);
#وإلا
  // TODO, consider changing the output of the error message from std::cerr to logging when the
  // exceptions are disabled, since using std::cerr might increase binary size, and std::cerr output
  // might not be easily accessible on some systems such as mobile
  // TODO, see if we need to change the output of the error message from std::cerr to NSLog for iOS
  std::cerr << نياپو << std::سٽ_پڄاڻي;
#جيڪڏهن_پورو
}
#جيڪڏهن_پورو  // #ifdef ORT_NO_EXCEPTIONS

}  // namespace onnxruntime
