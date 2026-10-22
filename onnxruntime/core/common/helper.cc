// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#అన్తర్భావ "core/common/common.h"

#నిర్వచితమైతే _WIN32
#అన్తర్భావ <Windows.h>
#అన్తర్భావ <assert.h>
#అయితే_ముగింపు

#నిర్వచితమైతే ORT_NO_EXCEPTIONS
#ఒకవేళ defined(__ANDROID__)
#అన్తర్భావ <android/log.h>
#లేకపోతే
#అన్తర్భావ <iostream>
#అయితే_ముగింపు
#అయితే_ముగింపు

నామాకాశ onnxruntime {
#నిర్వచితమైతే _WIN32
std::string గమ్యం_UTF8_String(std::wstring_view s) {
  ఒకవేళ (s.size() >= static_cast<size_t>(std::numeric_limits<పూర్ణాంకం>::max()))
    ORT_THROW("length overflow");

  స్థిరరాశి పూర్ణాంకం src_len = static_cast<పూర్ణాంకం>(s.size() + 1);
  స్థిరరాశి పూర్ణాంకం len = Wide_Char_గమ్యం_Multi_Byte(CP_UTF8, 0, s.దత్తాంశం(), src_len, శూన్యసూచిక, 0, శూన్యసూచిక, శూన్యసూచిక);
  ప్రతిజ్ఞా(len > 0);
  std::string ret(static_cast<size_t>(len) - 1, '\0');
#సూచన warning(disable : 4189)
  స్థిరరాశి పూర్ణాంకం r = Wide_Char_గమ్యం_Multi_Byte(CP_UTF8, 0, s.దత్తాంశం(), src_len, (అక్షరం*)ret.దత్తాంశం(), len, శూన్యసూచిక, శూన్యసూచిక);
  ప్రతిజ్ఞా(len == r);
#సూచన warning(అప్రమేయ : 4189)
  తిరిగి ret;
}

std::wstring గమ్యం_Wide_String(std::string_view s) {
  ఒకవేళ (s.size() >= static_cast<size_t>(std::numeric_limits<పూర్ణాంకం>::max()))
    ORT_THROW("length overflow");

  స్థిరరాశి పూర్ణాంకం src_len = static_cast<పూర్ణాంకం>(s.size() + 1);
  స్థిరరాశి పూర్ణాంకం len = Multi_Byte_గమ్యం_Wide_Char(CP_UTF8, 0, s.దత్తాంశం(), src_len, శూన్యసూచిక, 0);
  ప్రతిజ్ఞా(len > 0);
  std::wstring ret(static_cast<size_t>(len) - 1, '\0');
#సూచన warning(disable : 4189)
  స్థిరరాశి పూర్ణాంకం r = Multi_Byte_గమ్యం_Wide_Char(CP_UTF8, 0, s.దత్తాంశం(), src_len, (wchar_t*)ret.దత్తాంశం(), len);
  ప్రతిజ్ఞా(len == r);
#సూచన warning(అప్రమేయ : 4189)
  తిరిగి ret;
}
#అయితే_ముగింపు  // #ifdef _WIN32

#నిర్వచితమైతే ORT_NO_EXCEPTIONS
శూన్య Print_Final_సందేశం(స్థిరరాశి అక్షరం* సందేశం_2) {
#ఒకవేళ defined(__ANDROID__)
  __android_log_print(ANDROID_LOG_ERROR, "onnxruntime", "%s", సందేశం_2);
#లేకపోతే
  // TODO, consider changing the output of the error message from std::cerr to logging when the
  // exceptions are disabled, since using std::cerr might increase binary size, and std::cerr output
  // might not be easily accessible on some systems such as mobile
  // TODO, see if we need to change the output of the error message from std::cerr to NSLog for iOS
  std::cerr << సందేశం_2 << std::పంక్తి_ముగింపు;
#అయితే_ముగింపు
}
#అయితే_ముగింపు  // #ifdef ORT_NO_EXCEPTIONS

}  // namespace onnxruntime
