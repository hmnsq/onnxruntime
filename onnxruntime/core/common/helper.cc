// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#अन्तर्भाव "core/common/common.h"

#यदि_परिभाषितम् _WIN32
#अन्तर्भाव <Windows.h>
#अन्तर्भाव <assert.h>
#यदि_अन्तः

#यदि_परिभाषितम् ORT_NO_EXCEPTIONS
#यदि defined(__ANDROID__)
#अन्तर्भाव <android/log.h>
#अन्यथा
#अन्तर्भाव <iostream>
#यदि_अन्तः
#यदि_अन्तः

नामाकाश onnxruntime {
#यदि_परिभाषितम् _WIN32
std::string गन्तव्यम्_UTF8_String(std::wstring_view s) {
  यदि (s.size() >= static_cast<size_t>(std::numeric_limits<पूर्णाङ्क>::max()))
    ORT_THROW("length overflow");

  ध्रुव पूर्णाङ्क src_len = static_cast<पूर्णाङ्क>(s.size() + 1);
  ध्रुव पूर्णाङ्क len = Wide_Char_गन्तव्यम्_Multi_Byte(CP_UTF8, 0, s.दत्त(), src_len, शून्यसूचकः, 0, शून्यसूचकः, शून्यसूचकः);
  प्रतिज्ञा(len > 0);
  std::string ret(static_cast<size_t>(len) - 1, '\0');
#निर्देशः warning(disable : 4189)
  ध्रुव पूर्णाङ्क r = Wide_Char_गन्तव्यम्_Multi_Byte(CP_UTF8, 0, s.दत्त(), src_len, (वर्ण*)ret.दत्त(), len, शून्यसूचकः, शून्यसूचकः);
  प्रतिज्ञा(len == r);
#निर्देशः warning(स्वभाव : 4189)
  प्रत्यागम ret;
}

std::wstring गन्तव्यम्_Wide_String(std::string_view s) {
  यदि (s.size() >= static_cast<size_t>(std::numeric_limits<पूर्णाङ्क>::max()))
    ORT_THROW("length overflow");

  ध्रुव पूर्णाङ्क src_len = static_cast<पूर्णाङ्क>(s.size() + 1);
  ध्रुव पूर्णाङ्क len = Multi_Byte_गन्तव्यम्_Wide_Char(CP_UTF8, 0, s.दत्त(), src_len, शून्यसूचकः, 0);
  प्रतिज्ञा(len > 0);
  std::wstring ret(static_cast<size_t>(len) - 1, '\0');
#निर्देशः warning(disable : 4189)
  ध्रुव पूर्णाङ्क r = Multi_Byte_गन्तव्यम्_Wide_Char(CP_UTF8, 0, s.दत्त(), src_len, (wchar_t*)ret.दत्त(), len);
  प्रतिज्ञा(len == r);
#निर्देशः warning(स्वभाव : 4189)
  प्रत्यागम ret;
}
#यदि_अन्तः  // #ifdef _WIN32

#यदि_परिभाषितम् ORT_NO_EXCEPTIONS
शून्य Print_Final_सन्देशः(ध्रुव वर्ण* सन्देशः_2) {
#यदि defined(__ANDROID__)
  __android_log_print(ANDROID_LOG_ERROR, "onnxruntime", "%s", सन्देशः_2);
#अन्यथा
  // TODO, consider changing the output of the error message from std::cerr to logging when the
  // exceptions are disabled, since using std::cerr might increase binary size, and std::cerr output
  // might not be easily accessible on some systems such as mobile
  // TODO, see if we need to change the output of the error message from std::cerr to NSLog for iOS
  std::cerr << सन्देशः_2 << std::पङ्क्त्यन्तः;
#यदि_अन्तः
}
#यदि_अन्तः  // #ifdef ORT_NO_EXCEPTIONS

}  // namespace onnxruntime
