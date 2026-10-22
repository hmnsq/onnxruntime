// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#अन्तर्भाव "core/common/common.h"

#ifdef _WIN32
#अन्तर्भाव <Windows.h>
#अन्तर्भाव <assert.h>
#endif

#ifdef ORT_NO_EXCEPTIONS
#यदि defined(__ANDROID__)
#अन्तर्भाव <android/log.h>
#अन्यथा
#अन्तर्भाव <iostream>
#endif
#endif

नामाकाश onnxruntime {
#ifdef _WIN32
std::string ToUTF8String(std::wstring_view s) {
  यदि (s.size() >= static_cast<माप_प्रकार>(std::numeric_limits<पूर्णाङ्क>::max()))
    ORT_THROW("length overflow");

  अचर पूर्णाङ्क src_len = static_cast<पूर्णाङ्क>(s.size() + 1);
  अचर पूर्णाङ्क len = WideCharToMultiByte(CP_UTF8, 0, s.data(), src_len, nullptr, 0, nullptr, nullptr);
  प्रतिज्ञा(len > 0);
  std::string ret(static_cast<माप_प्रकार>(len) - 1, '\0');
#pragma warning(disable : 4189)
  अचर पूर्णाङ्क r = WideCharToMultiByte(CP_UTF8, 0, s.data(), src_len, (वर्ण*)ret.data(), len, nullptr, nullptr);
  प्रतिज्ञा(len == r);
#pragma warning(स्वभाव : 4189)
  फर्काऊ ret;
}

std::wstring ToWideString(std::string_view s) {
  यदि (s.size() >= static_cast<माप_प्रकार>(std::numeric_limits<पूर्णाङ्क>::max()))
    ORT_THROW("length overflow");

  अचर पूर्णाङ्क src_len = static_cast<पूर्णाङ्क>(s.size() + 1);
  अचर पूर्णाङ्क len = MultiByteToWideChar(CP_UTF8, 0, s.data(), src_len, nullptr, 0);
  प्रतिज्ञा(len > 0);
  std::wstring ret(static_cast<माप_प्रकार>(len) - 1, '\0');
#pragma warning(disable : 4189)
  अचर पूर्णाङ्क r = MultiByteToWideChar(CP_UTF8, 0, s.data(), src_len, (ब_अक्षर_प्रकार*)ret.data(), len);
  प्रतिज्ञा(len == r);
#pragma warning(स्वभाव : 4189)
  फर्काऊ ret;
}
#endif  // #ifdef _WIN32

#ifdef ORT_NO_EXCEPTIONS
शून्य PrintFinalMessage(अचर वर्ण* सन्देश) {
#यदि defined(__ANDROID__)
  __android_log_print(ANDROID_LOG_ERROR, "onnxruntime", "%s", सन्देश);
#अन्यथा
  // TODO, consider changing the output of the error message from std::cerr to logging when the
  // exceptions are disabled, since using std::cerr might increase binary size, and std::cerr output
  // might not be easily accessible on some systems such as mobile
  // TODO, see if we need to change the output of the error message from std::cerr to NSLog for iOS
  std::cerr << सन्देश << std::endl;
#endif
}
#endif  // #ifdef ORT_NO_EXCEPTIONS

}  // namespace onnxruntime
