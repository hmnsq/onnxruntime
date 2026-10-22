// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

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
ಅನ್ತಃಸ್ಥ InlinedVector<std::string_view> SplitString(std::string_view string_to_split, std::string_view delimiter,
                                                   bool keep_empty = false) {
  ORT_ENFORCE(!delimiter.empty(), "delimiter must not be empty");
  InlinedVector<std::string_view> result{};
  std::string_view::size_type segment_begin_pos = 0;
  ಯಾವತ್ (segment_begin_pos != std::string_view::npos) {
    ಧ್ರುವ std::string_view::size_type segment_end_pos = string_to_split.find(delimiter, segment_begin_pos);
    ಧ್ರುವ bool is_segment_empty = segment_begin_pos == segment_end_pos || segment_begin_pos == string_to_split.size();
    ಯದಿ (!is_segment_empty || keep_empty) {
      result.ತಳ್ಳು_back(string_to_split.substr(segment_begin_pos, segment_end_pos - segment_begin_pos));
    }
    segment_begin_pos = (segment_end_pos == std::string_view::npos)
                            ? segment_end_pos
                            : segment_end_pos + delimiter.size();
  }
  ಪ್ರತ್ಯಾಗಮ result;
}

/**
 * Trim a string from start inplace.
 * @param s The string to trim.
 */
ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ Trim_String_From_ಎಡ(std::string& s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned ವರ್ಣ ch) { ಪ್ರತ್ಯಾಗಮ !std::isspace(ch); }));
}

/**
 * Trim a string from end inplace.
 * @param s The string to trim.
 */
ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ Trim_String_From_ಬಲ(std::string& s) {
  s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned ವರ್ಣ ch) { ಪ್ರತ್ಯಾಗಮ !std::isspace(ch); }).base(), s.end());
}

/**
 * Trim a string from both ends.
 * @param s The string to trim.
 * @return The trimmed string.
 */
ಅನ್ತಃಸ್ಥ std::string TrimString(std::string_view s) {
  std::string s_trimmed{s};
  Trim_String_From_ಬಲ(s_trimmed);
  Trim_String_From_ಎಡ(s_trimmed);
  ಪ್ರತ್ಯಾಗಮ s_trimmed;
}

/**
 * @brief A consistent way to construct the full qualified op name.
 */
ಅನ್ತಃಸ್ಥ std::string GetFullQualifiedOpName(ಧ್ರುವ std::string& op_type, ಧ್ರುವ std::string& domain) {
  ಪ್ರತ್ಯಾಗಮ MakeString(domain, "::", op_type);
}

/**
 * Use this simple hash to generate unique int by given string input.
 */
ಅನ್ತಃಸ್ಥ uint32_t GetHashFromString(ಧ್ರುವ std::string& str_ಮೌಲ್ಯ) {
  uint32_t hash = 0;
  ಪ್ರತಿ (ವರ್ಣ ಧ್ರುವ& c : str_ಮೌಲ್ಯ) {
    hash = hash * 101 + c;
  }

  ಪ್ರತ್ಯಾಗಮ hash;
}

/**
 * Returns a lowercase version of the input string.
 * @param str The string to lowercase.
 * @return The lowercased string.
 */
ಅನ್ತಃಸ್ಥ std::string GetLowercaseString(std::string str) {
  // https://en.cppreference.com/w/cpp/string/byte/tolower
  // The behavior of tolower from <cctype> is undefined if the argument is neither representable as unsigned char
  // nor equal to EOF. To use tolower safely with a plain char (or signed char), the argument must be converted to
  // unsigned char.
  std::transform(str.begin(), str.end(), str.begin(), [](unsigned ವರ್ಣ c) {
    ಪ್ರತ್ಯಾಗಮ static_cast<ವರ್ಣ>(std::tolower(c));
  });
  ಪ್ರತ್ಯಾಗಮ str;
}

}  // namespace utils
}  // namespace onnxruntime
