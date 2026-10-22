// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ಸೂಚನೆ once

#ಅನ್ತರ್ಭಾವ <algorithm>
#ಅನ್ತರ್ಭಾವ <cctype>
#ಅನ್ತರ್ಭಾವ <string>
#ಅನ್ತರ್ಭಾವ <string_view>
#ಅನ್ತರ್ಭಾವ <vector>

#ಅನ್ತರ್ಭಾವ "core/common/common.h"
#ಅನ್ತರ್ಭಾವ "core/common/inlined_containers.h"

namespace onnxruntime {
namespace utils {

/**
 * Splits a string into substrings delimited by the given delimiter string.
 * @param string_to_split The string to split.
 * @param delimiter The delimiter string.
 * @param keep_empty Whether to keep empty substrings.
 * @return The split substrings.
 */
ಸಾಲೊಳಗೆ Inlined_ಸದಿಶ<std::string_view> ವಿಭಜಿಸು_String(std::string_view string_ಗಮ್ಯ_ವಿಭಜಿಸು, std::string_view delimiter,
                                                   bool keep_ಖಾಲಿಗಳು = false) {
  ORT_ENFORCE(!delimiter.ಖಾಲಿಗಳು(), "delimiter must not be empty");
  Inlined_ಸದಿಶ<std::string_view> ಫಲಿತಾಂಶ{};
  std::string_view::ಗಾತ್ರ_type segment_begin_pos = 0;
  ಆಗ (segment_begin_pos != std::string_view::npos) {
    ಸ್ಥಿರಾಂಕ std::string_view::ಗಾತ್ರ_type segment_ಕೊನೆ_pos = string_ಗಮ್ಯ_ವಿಭಜಿಸು.ಕಂಡುಹಿಡಿ(delimiter, segment_begin_pos);
    ಸ್ಥಿರಾಂಕ bool is_segment_ಖಾಲಿಗಳು = segment_begin_pos == segment_ಕೊನೆ_pos || segment_begin_pos == string_ಗಮ್ಯ_ವಿಭಜಿಸು.ಗಾತ್ರ_2();
    ಆದರೆ (!is_segment_ಖಾಲಿಗಳು || keep_ಖಾಲಿಗಳು) {
      ಫಲಿತಾಂಶ.ತಳ್ಳು_ಹಿಂಭಾಗ(string_ಗಮ್ಯ_ವಿಭಜಿಸು.substr(segment_begin_pos, segment_ಕೊನೆ_pos - segment_begin_pos));
    }
    segment_begin_pos = (segment_ಕೊನೆ_pos == std::string_view::npos)
                            ? segment_ಕೊನೆ_pos
                            : segment_ಕೊನೆ_pos + delimiter.ಗಾತ್ರ_2();
  }
  ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

/**
 * Trim a string from start inplace.
 * @param s The string to trim.
 */
ಸಾಲೊಳಗೆ ಶೂನ್ಯ Trim_String_From_ಎಡ(std::string& s) {
  s.erase(s.begin(), std::ಕಂಡುಹಿಡಿ_if(s.begin(), s.ಕೊನೆ(), [](ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ ch) { ಹಿಂತಿರುಗಿ !std::ಖಾಲಿಯೇ(ch); }));
}

/**
 * Trim a string from end inplace.
 * @param s The string to trim.
 */
ಸಾಲೊಳಗೆ ಶೂನ್ಯ Trim_String_From_ಬಲ(std::string& s) {
  s.erase(std::ಕಂಡುಹಿಡಿ_if(s.rbegin(), s.rend(), [](ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ ch) { ಹಿಂತಿರುಗಿ !std::ಖಾಲಿಯೇ(ch); }).ತಳ(), s.ಕೊನೆ());
}

/**
 * Trim a string from both ends.
 * @param s The string to trim.
 * @return The trimmed string.
 */
ಸಾಲೊಳಗೆ std::string TrimString(std::string_view s) {
  std::string s_trimmed{s};
  Trim_String_From_ಬಲ(s_trimmed);
  Trim_String_From_ಎಡ(s_trimmed);
  ಹಿಂತಿರುಗಿ s_trimmed;
}

/**
 * @brief A consistent way to construct the full qualified op name.
 */
ಸಾಲೊಳಗೆ std::string Get_ತುಂಬಿದ_Qualified_Op_Name(ಸ್ಥಿರಾಂಕ std::string& op_type, ಸ್ಥಿರಾಂಕ std::string& domain) {
  ಹಿಂತಿರುಗಿ MakeString(domain, "::", op_type);
}

/**
 * Use this simple hash to generate unique int by given string input.
 */
ಸಾಲೊಳಗೆ uint32_t GetHashFromString(ಸ್ಥಿರಾಂಕ std::string& str_ಮೌಲ್ಯ) {
  uint32_t hash = 0;
  ಗಾಗಿ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ& c : str_ಮೌಲ್ಯ) {
    hash = hash * 101 + c;
  }

  ಹಿಂತಿರುಗಿ hash;
}

/**
 * Returns a lowercase version of the input string.
 * @param str The string to lowercase.
 * @return The lowercased string.
 */
ಸಾಲೊಳಗೆ std::string GetLowercaseString(std::string str) {
  // https://en.cppreference.com/w/cpp/string/byte/tolower
  // The behavior of tolower from <cctype> is undefined if the argument is neither representable as unsigned char
  // nor equal to EOF. To use tolower safely with a plain char (or signed char), the argument must be converted to
  // unsigned char.
  std::transform(str.begin(), str.ಕೊನೆ(), str.begin(), [](ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ c) {
    ಹಿಂತಿರುಗಿ static_cast<ಅಕ್ಷರ>(std::tolower(c));
  });
  ಹಿಂತಿರುಗಿ str;
}

}  // namespace utils
}  // namespace onnxruntime
