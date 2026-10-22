// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#അന്തര്ഭാവ <algorithm>
#അന്തര്ഭാവ <cctype>
#അന്തര്ഭാവ <string>
#അന്തര്ഭാവ <string_view>
#അന്തര്ഭാവ <vector>

#അന്തര്ഭാവ "core/common/common.h"
#അന്തര്ഭാവ "core/common/inlined_containers.h"

namespace onnxruntime {
namespace utils {

/**
 * Splits a string into substrings delimited by the given delimiter string.
 * @param string_to_split The string to split.
 * @param delimiter The delimiter string.
 * @param keep_empty Whether to keep empty substrings.
 * @return The split substrings.
 */
വരിയിൽ InlinedVector<std::string_view> SplitString(std::string_view string_to_split, std::string_view delimiter,
                                                   bool keep_empty = false) {
  ORT_ENFORCE(!delimiter.empty(), "delimiter must not be empty");
  InlinedVector<std::string_view> result{};
  std::string_view::size_type segment_begin_pos = 0;
  ആകുമ്പോൾ (segment_begin_pos != std::string_view::npos) {
    സ്ഥിരം std::string_view::size_type segment_end_pos = string_to_split.find(delimiter, segment_begin_pos);
    സ്ഥിരം bool is_segment_empty = segment_begin_pos == segment_end_pos || segment_begin_pos == string_to_split.size();
    എങ്കിൽ (!is_segment_empty || keep_empty) {
      result.തള്ളുക_back(string_to_split.substr(segment_begin_pos, segment_end_pos - segment_begin_pos));
    }
    segment_begin_pos = (segment_end_pos == std::string_view::npos)
                            ? segment_end_pos
                            : segment_end_pos + delimiter.size();
  }
  തിരികെ result;
}

/**
 * Trim a string from start inplace.
 * @param s The string to trim.
 */
വരിയിൽ ശൂന്യം Trim_String_From_ഇടത്(std::string& s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned അക്ഷരം ch) { തിരികെ !std::isspace(ch); }));
}

/**
 * Trim a string from end inplace.
 * @param s The string to trim.
 */
വരിയിൽ ശൂന്യം Trim_String_From_വലത്(std::string& s) {
  s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned അക്ഷരം ch) { തിരികെ !std::isspace(ch); }).base(), s.end());
}

/**
 * Trim a string from both ends.
 * @param s The string to trim.
 * @return The trimmed string.
 */
വരിയിൽ std::string TrimString(std::string_view s) {
  std::string s_trimmed{s};
  Trim_String_From_വലത്(s_trimmed);
  Trim_String_From_ഇടത്(s_trimmed);
  തിരികെ s_trimmed;
}

/**
 * @brief A consistent way to construct the full qualified op name.
 */
വരിയിൽ std::string GetFullQualifiedOpName(സ്ഥിരം std::string& op_type, സ്ഥിരം std::string& domain) {
  തിരികെ MakeString(domain, "::", op_type);
}

/**
 * Use this simple hash to generate unique int by given string input.
 */
വരിയിൽ uint32_t GetHashFromString(സ്ഥിരം std::string& str_മൂല്യം) {
  uint32_t hash = 0;
  വേണ്ടി (അക്ഷരം സ്ഥിരം& c : str_മൂല്യം) {
    hash = hash * 101 + c;
  }

  തിരികെ hash;
}

/**
 * Returns a lowercase version of the input string.
 * @param str The string to lowercase.
 * @return The lowercased string.
 */
വരിയിൽ std::string GetLowercaseString(std::string str) {
  // https://en.cppreference.com/w/cpp/string/byte/tolower
  // The behavior of tolower from <cctype> is undefined if the argument is neither representable as unsigned char
  // nor equal to EOF. To use tolower safely with a plain char (or signed char), the argument must be converted to
  // unsigned char.
  std::transform(str.begin(), str.end(), str.begin(), [](unsigned അക്ഷരം c) {
    തിരികെ static_cast<അക്ഷരം>(std::tolower(c));
  });
  തിരികെ str;
}

}  // namespace utils
}  // namespace onnxruntime
