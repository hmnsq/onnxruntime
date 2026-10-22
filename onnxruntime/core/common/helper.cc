// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#समावेश "core/common/common.h"

#अगर_घोषित _WIN32
#समावेश <Windows.h>
#समावेश <assert.h>
#पूर्ण_अगर

#अगर_घोषित ORT_NO_EXCEPTIONS
#अगर defined(__ANDROID__)
#समावेश <android/log.h>
#अन्यथा
#समावेश <iostream>
#पूर्ण_अगर
#पूर्ण_अगर

namespace onnxruntime {
#अगर_घोषित _WIN32
std::string ToUTF8String(std::wstring_view s) {
  अगर (s.size() >= static_cast<माप_प्रकार>(std::numeric_limits<पूर्णांक>::max()))
    ORT_THROW("length overflow");

  स्थिर पूर्णांक src_len = static_cast<पूर्णांक>(s.size() + 1);
  स्थिर पूर्णांक len = WideCharToMultiByte(CP_UTF8, 0, s.तथ्य(), src_len, nullptr, 0, nullptr, nullptr);
  निश्चित(len > 0);
  std::string ret(static_cast<माप_प्रकार>(len) - 1, '\0');
#आशय warning(disable : 4189)
  स्थिर पूर्णांक r = WideCharToMultiByte(CP_UTF8, 0, s.तथ्य(), src_len, (अक्षर*)ret.तथ्य(), len, nullptr, nullptr);
  निश्चित(len == r);
#आशय warning(शेष : 4189)
  वापस ret;
}

std::wstring ToWideString(std::string_view s) {
  अगर (s.size() >= static_cast<माप_प्रकार>(std::numeric_limits<पूर्णांक>::max()))
    ORT_THROW("length overflow");

  स्थिर पूर्णांक src_len = static_cast<पूर्णांक>(s.size() + 1);
  स्थिर पूर्णांक len = MultiByteToWideChar(CP_UTF8, 0, s.तथ्य(), src_len, nullptr, 0);
  निश्चित(len > 0);
  std::wstring ret(static_cast<माप_प्रकार>(len) - 1, '\0');
#आशय warning(disable : 4189)
  स्थिर पूर्णांक r = MultiByteToWideChar(CP_UTF8, 0, s.तथ्य(), src_len, (ब_अक्षर_प्रकार*)ret.तथ्य(), len);
  निश्चित(len == r);
#आशय warning(शेष : 4189)
  वापस ret;
}
#पूर्ण_अगर  // #ifdef _WIN32

#अगर_घोषित ORT_NO_EXCEPTIONS
व्योम PrintFinalMessage(स्थिर अक्षर* संदेश) {
#अगर defined(__ANDROID__)
  __android_log_print(ANDROID_LOG_ERROR, "onnxruntime", "%s", संदेश);
#अन्यथा
  // TODO, consider changing the output of the error message from std::cerr to logging when the
  // exceptions are disabled, since using std::cerr might increase binary size, and std::cerr output
  // might not be easily accessible on some systems such as mobile
  // TODO, see if we need to change the output of the error message from std::cerr to NSLog for iOS
  std::अ_त्रुटि << संदेश << std::प_पूर्ण;
#पूर्ण_अगर
}
#पूर्ण_अगर  // #ifdef ORT_NO_EXCEPTIONS

}  // namespace onnxruntime
