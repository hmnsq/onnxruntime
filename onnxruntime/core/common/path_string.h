// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#आशय once

#समावेश <string>
#समावेश <type_traits>

// for std::tolower or std::towlower
#अगर_घोषित _WIN32
#समावेश <cwctype>
#अन्यथा
#समावेश <cctype>
#पूर्ण_अगर

// for converting / printing ORT_TSTR path strings to std::string
#अगर_घोषित _WIN32
#घोषणा ORT_TSTR_CONVERT_TO_PRINTABLE_STRING(X) std::wstring_convert<std::codecvt_utf8<ब_अक्षर_प्रकार>>().to_bytes(X)
#घोषणा ORT_TSTR_CONVERT_FROM_STRING(X) std::wstring_convert<std::codecvt_utf8<ब_अक्षर_प्रकार>>().from_bytes(X);
#अन्यथा
#घोषणा ORT_TSTR_CONVERT_TO_PRINTABLE_STRING(X) X
#घोषणा ORT_TSTR_CONVERT_FROM_STRING(X) X
#पूर्ण_अगर

#समावेश "core/common/common.h"
#समावेश "core/session/onnxruntime_c_api.h"

namespace onnxruntime {

// char type for filesystem paths
using PathChar = ORTCHAR_T;
// string type for filesystem paths
using PathString = std::basic_string<PathChar>;

अंतरभूत PathString ToPathString(स्थिर PathString& s) {
  वापस s;
}

#अगर_घोषित _WIN32

static_assert(std::is_same<PathString, std::wstring>::मूल्य, "PathString is not std::wstring!");

अंतरभूत PathString ToPathString(std::string_view s) {
  वापस ToWideString(s);
}
अंतरभूत PathString ToPathString(स्थिर अक्षर* s) {
  वापस ToWideString(s);
}
अंतरभूत PathString ToPathString(स्थिर std::string& s) {
  वापस ToWideString(s);
}

अंतरभूत PathChar ToLowerPathChar(PathChar c) {
  वापस std::towlower(c);
}

अंतरभूत std::string PathToUTF8String(स्थिर PathString& s) {
  वापस ToUTF8String(s);
}

#अन्यथा

static_assert(std::is_same<PathString, std::string>::मूल्य, "PathString is not std::string!");

अंतरभूत PathString ToPathString(स्थिर अक्षर* s) {
  वापस s;
}

अंतरभूत PathString ToPathString(std::string_view s) {
  वापस PathString{s};
}

अंतरभूत PathChar ToLowerPathChar(PathChar c) {
  वापस std::छोटे(c);
}

अंतरभूत std::string PathToUTF8String(स्थिर PathString& s) {
  वापस s;
}

#पूर्ण_अगर

}  // namespace onnxruntime
