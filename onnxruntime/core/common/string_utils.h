// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#ضمن <algorithm>
#ضمن <cctype>
#ضمن <string>
#ضمن <string_view>
#ضمن <vector>

#ضمن "core/common/common.h"
#ضمن "core/common/inlined_containers.h"

namespace onnxruntime {
namespace utils {

/**
 * Splits a string into substrings delimited by the given delimiter string.
 * @param string_to_split The string to split.
 * @param delimiter The delimiter string.
 * @param keep_empty Whether to keep empty substrings.
 * @return The split substrings.
 */
مضمن InlinedVector<std::string_view> SplitString(std::string_view string_to_split, std::string_view delimiter,
                                                   bool keep_empty = false) {
  ORT_ENFORCE(!delimiter.empty(), "delimiter must not be empty");
  InlinedVector<std::string_view> result{};
  std::string_view::size_type segment_begin_pos = 0;
  طالما (segment_begin_pos != std::string_view::npos) {
    ثابت std::string_view::size_type segment_end_pos = string_to_split.find(delimiter, segment_begin_pos);
    ثابت bool is_segment_empty = segment_begin_pos == segment_end_pos || segment_begin_pos == string_to_split.size();
    إذا (!is_segment_empty || keep_empty) {
      result.تولق_back(string_to_split.substr(segment_begin_pos, segment_end_pos - segment_begin_pos));
    }
    segment_begin_pos = (segment_end_pos == std::string_view::npos)
                            ? segment_end_pos
                            : segment_end_pos + delimiter.size();
  }
  أرجع result;
}

/**
 * Trim a string from start inplace.
 * @param s The string to trim.
 */
مضمن فراغ Trim_String_From_کيري(std::string& s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned حرف ch) { أرجع !std::isspace(ch); }));
}

/**
 * Trim a string from end inplace.
 * @param s The string to trim.
 */
مضمن فراغ Trim_String_From_کانن(std::string& s) {
  s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned حرف ch) { أرجع !std::isspace(ch); }).base(), s.end());
}

/**
 * Trim a string from both ends.
 * @param s The string to trim.
 * @return The trimmed string.
 */
مضمن std::string TrimString(std::string_view s) {
  std::string s_trimmed{s};
  Trim_String_From_کانن(s_trimmed);
  Trim_String_From_کيري(s_trimmed);
  أرجع s_trimmed;
}

/**
 * @brief A consistent way to construct the full qualified op name.
 */
مضمن std::string GetFullQualifiedOpName(ثابت std::string& op_type, ثابت std::string& domain) {
  أرجع MakeString(domain, "::", op_type);
}

/**
 * Use this simple hash to generate unique int by given string input.
 */
مضمن uint32_t GetHashFromString(ثابت std::string& str_نيلاي) {
  uint32_t hash = 0;
  لكل (حرف ثابت& c : str_نيلاي) {
    hash = hash * 101 + c;
  }

  أرجع hash;
}

/**
 * Returns a lowercase version of the input string.
 * @param str The string to lowercase.
 * @return The lowercased string.
 */
مضمن std::string GetLowercaseString(std::string str) {
  // https://en.cppreference.com/w/cpp/string/byte/tolower
  // The behavior of tolower from <cctype> is undefined if the argument is neither representable as unsigned char
  // nor equal to EOF. To use tolower safely with a plain char (or signed char), the argument must be converted to
  // unsigned char.
  std::transform(str.begin(), str.end(), str.begin(), [](unsigned حرف c) {
    أرجع static_cast<حرف>(std::tolower(c));
  });
  أرجع str;
}

}  // namespace utils
}  // namespace onnxruntime
