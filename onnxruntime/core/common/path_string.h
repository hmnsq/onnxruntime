// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#సూచన once

#అన్తర్భావ <string>
#అన్తర్భావ <type_traits>

// for std::tolower or std::towlower
#నిర్వచితమైతే _WIN32
#అన్తర్భావ <cwctype>
#లేకపోతే
#అన్తర్భావ <cctype>
#అయితే_ముగింపు

// for converting / printing ORT_TSTR path strings to std::string
#నిర్వచితమైతే _WIN32
#నిర్వచించు ORT_TSTR_CONVERT_గమ్యం_PRINTABLE_STRING(X) std::wstring_convert<std::codecvt_utf8<wchar_t>>().గమ్యం_బైట్లు(X)
#నిర్వచించు ORT_TSTR_CONVERT_FROM_STRING(X) std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_బైట్లు(X);
#లేకపోతే
#నిర్వచించు ORT_TSTR_CONVERT_గమ్యం_PRINTABLE_STRING(X) X
#నిర్వచించు ORT_TSTR_CONVERT_FROM_STRING(X) X
#అయితే_ముగింపు

#అన్తర్భావ "core/common/common.h"
#అన్తర్భావ "core/session/onnxruntime_c_api.h"

namespace onnxruntime {

// char type for filesystem paths
using మార్గం_Char = ORTCHAR_T;
// string type for filesystem paths
using మార్గం_String = std::basic_string<మార్గం_Char>;

అంతఃవరుస మార్గం_String గమ్యం_మార్గం_String(స్థిరరాశి మార్గం_String& s) {
  తిరిగి s;
}

#నిర్వచితమైతే _WIN32

static_assert(std::is_same<మార్గం_String, std::wstring>::విలువ, "PathString is not std::wstring!");

అంతఃవరుస మార్గం_String గమ్యం_మార్గం_String(std::string_view s) {
  తిరిగి గమ్యం_Wide_String(s);
}
అంతఃవరుస మార్గం_String గమ్యం_మార్గం_String(స్థిరరాశి అక్షరం* s) {
  తిరిగి గమ్యం_Wide_String(s);
}
అంతఃవరుస మార్గం_String గమ్యం_మార్గం_String(స్థిరరాశి std::string& s) {
  తిరిగి గమ్యం_Wide_String(s);
}

అంతఃవరుస మార్గం_Char గమ్యం_క్రింది_మార్గం_Char(మార్గం_Char c) {
  తిరిగి std::towlower(c);
}

అంతఃవరుస std::string మార్గం_గమ్యం_UTF8_String(స్థిరరాశి మార్గం_String& s) {
  తిరిగి గమ్యం_UTF8_String(s);
}

#లేకపోతే

static_assert(std::is_same<మార్గం_String, std::string>::విలువ, "PathString is not std::string!");

అంతఃవరుస మార్గం_String గమ్యం_మార్గం_String(స్థిరరాశి అక్షరం* s) {
  తిరిగి s;
}

అంతఃవరుస మార్గం_String గమ్యం_మార్గం_String(std::string_view s) {
  తిరిగి మార్గం_String{s};
}

అంతఃవరుస మార్గం_Char గమ్యం_క్రింది_మార్గం_Char(మార్గం_Char c) {
  తిరిగి std::tolower(c);
}

అంతఃవరుస std::string మార్గం_గమ్యం_UTF8_String(స్థిరరాశి మార్గం_String& s) {
  తిరిగి s;
}

#అయితే_ముగింపు

}  // namespace onnxruntime
