// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ಸೂಚನೆ once

#ಅನ್ತರ್ಭಾವ <string>
#ಅನ್ತರ್ಭಾವ <type_traits>

// for std::tolower or std::towlower
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _WIN32
#ಅನ್ತರ್ಭಾವ <cwctype>
#ಇಲ್ಲದಿದ್ದರೆ
#ಅನ್ತರ್ಭಾವ <cctype>
#ಆದರೆ_ಅಂತ್ಯ

// for converting / printing ORT_TSTR path strings to std::string
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _WIN32
#ವ್ಯಾಖ್ಯಾನಿಸು ORT_TSTR_CONVERT_ಗಮ್ಯ_PRINTABLE_STRING(X) std::wstring_convert<std::codecvt_utf8<wchar_t>>().ಗಮ್ಯ_ಬೈಟುಗಳು(X)
#ವ್ಯಾಖ್ಯಾನಿಸು ORT_TSTR_CONVERT_FROM_STRING(X) std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_ಬೈಟುಗಳು(X);
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು ORT_TSTR_CONVERT_ಗಮ್ಯ_PRINTABLE_STRING(X) X
#ವ್ಯಾಖ್ಯಾನಿಸು ORT_TSTR_CONVERT_FROM_STRING(X) X
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "core/common/common.h"
#ಅನ್ತರ್ಭಾವ "core/session/onnxruntime_c_api.h"

namespace onnxruntime {

// char type for filesystem paths
using ಮಾರ್ಗ_Char = ORTCHAR_T;
// string type for filesystem paths
using ಮಾರ್ಗ_String = std::basic_string<ಮಾರ್ಗ_Char>;

ಸಾಲೊಳಗೆ ಮಾರ್ಗ_String ಗಮ್ಯ_ಮಾರ್ಗ_String(ಸ್ಥಿರಾಂಕ ಮಾರ್ಗ_String& s) {
  ಹಿಂತಿರುಗಿ s;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _WIN32

static_assert(std::is_same<ಮಾರ್ಗ_String, std::wstring>::ಮೌಲ್ಯ, "PathString is not std::wstring!");

ಸಾಲೊಳಗೆ ಮಾರ್ಗ_String ಗಮ್ಯ_ಮಾರ್ಗ_String(std::string_view s) {
  ಹಿಂತಿರುಗಿ ಗಮ್ಯ_Wide_String(s);
}
ಸಾಲೊಳಗೆ ಮಾರ್ಗ_String ಗಮ್ಯ_ಮಾರ್ಗ_String(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* s) {
  ಹಿಂತಿರುಗಿ ಗಮ್ಯ_Wide_String(s);
}
ಸಾಲೊಳಗೆ ಮಾರ್ಗ_String ಗಮ್ಯ_ಮಾರ್ಗ_String(ಸ್ಥಿರಾಂಕ std::string& s) {
  ಹಿಂತಿರುಗಿ ಗಮ್ಯ_Wide_String(s);
}

ಸಾಲೊಳಗೆ ಮಾರ್ಗ_Char ಗಮ್ಯ_ಅಧೋ_ಮಾರ್ಗ_Char(ಮಾರ್ಗ_Char c) {
  ಹಿಂತಿರುಗಿ std::towlower(c);
}

ಸಾಲೊಳಗೆ std::string ಮಾರ್ಗ_ಗಮ್ಯ_UTF8_String(ಸ್ಥಿರಾಂಕ ಮಾರ್ಗ_String& s) {
  ಹಿಂತಿರುಗಿ ಗಮ್ಯ_UTF8_String(s);
}

#ಇಲ್ಲದಿದ್ದರೆ

static_assert(std::is_same<ಮಾರ್ಗ_String, std::string>::ಮೌಲ್ಯ, "PathString is not std::string!");

ಸಾಲೊಳಗೆ ಮಾರ್ಗ_String ಗಮ್ಯ_ಮಾರ್ಗ_String(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* s) {
  ಹಿಂತಿರುಗಿ s;
}

ಸಾಲೊಳಗೆ ಮಾರ್ಗ_String ಗಮ್ಯ_ಮಾರ್ಗ_String(std::string_view s) {
  ಹಿಂತಿರುಗಿ ಮಾರ್ಗ_String{s};
}

ಸಾಲೊಳಗೆ ಮಾರ್ಗ_Char ಗಮ್ಯ_ಅಧೋ_ಮಾರ್ಗ_Char(ಮಾರ್ಗ_Char c) {
  ಹಿಂತಿರುಗಿ std::tolower(c);
}

ಸಾಲೊಳಗೆ std::string ಮಾರ್ಗ_ಗಮ್ಯ_UTF8_String(ಸ್ಥಿರಾಂಕ ಮಾರ್ಗ_String& s) {
  ಹಿಂತಿರುಗಿ s;
}

#ಆದರೆ_ಅಂತ್ಯ

}  // namespace onnxruntime
