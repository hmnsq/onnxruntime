// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#شامل <algorithm>
#شامل <cctype>
#شامل <string>
#شامل <string_view>
#شامل <vector>

#شامل "core/common/common.h"
#شامل "core/common/inlined_containers.h"

namespace onnxruntime {
namespace utils {

/**
 * Splits a string into substrings delimited by the given delimiter string.
 * @param string_to_split The string to split.
 * @param delimiter The delimiter string.
 * @param keep_empty Whether to keep empty substrings.
 * @return The split substrings.
 */
درونخطی InlinedVector<std::string_view> SplitString(std::string_view string_to_split, std::string_view delimiter,
                                                   bool keep_empty = false) {
  ORT_ENFORCE(!delimiter.empty(), "delimiter must not be empty");
  InlinedVector<std::string_view> result{};
  std::string_view::size_type segment_begin_pos = 0;
  وقتی (segment_begin_pos != std::string_view::npos) {
    ثابت std::string_view::size_type segment_end_pos = string_to_split.find(delimiter, segment_begin_pos);
    ثابت bool is_segment_empty = segment_begin_pos == segment_end_pos || segment_begin_pos == string_to_split.size();
    اگر (!is_segment_empty || keep_empty) {
      result.افزودن_back(string_to_split.substr(segment_begin_pos, segment_end_pos - segment_begin_pos));
    }
    segment_begin_pos = (segment_end_pos == std::string_view::npos)
                            ? segment_end_pos
                            : segment_end_pos + delimiter.size();
  }
  بازگشت result;
}

/**
 * Trim a string from start inplace.
 * @param s The string to trim.
 */
درونخطی تهی Trim_String_From_چپ(std::string& s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned نویسه ch) { بازگشت !std::isspace(ch); }));
}

/**
 * Trim a string from end inplace.
 * @param s The string to trim.
 */
درونخطی تهی Trim_String_From_راست(std::string& s) {
  s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned نویسه ch) { بازگشت !std::isspace(ch); }).base(), s.end());
}

/**
 * Trim a string from both ends.
 * @param s The string to trim.
 * @return The trimmed string.
 */
درونخطی std::string TrimString(std::string_view s) {
  std::string s_trimmed{s};
  Trim_String_From_راست(s_trimmed);
  Trim_String_From_چپ(s_trimmed);
  بازگشت s_trimmed;
}

/**
 * @brief A consistent way to construct the full qualified op name.
 */
درونخطی std::string GetFullQualifiedOpName(ثابت std::string& op_type, ثابت std::string& domain) {
  بازگشت MakeString(domain, "::", op_type);
}

/**
 * Use this simple hash to generate unique int by given string input.
 */
درونخطی uint32_t GetHashFromString(ثابت std::string& str_مقدار) {
  uint32_t hash = 0;
  برای (نویسه ثابت& c : str_مقدار) {
    hash = hash * 101 + c;
  }

  بازگشت hash;
}

/**
 * Returns a lowercase version of the input string.
 * @param str The string to lowercase.
 * @return The lowercased string.
 */
درونخطی std::string GetLowercaseString(std::string str) {
  // https://en.cppreference.com/w/cpp/string/byte/tolower
  // The behavior of tolower from <cctype> is undefined if the argument is neither representable as unsigned char
  // nor equal to EOF. To use tolower safely with a plain char (or signed char), the argument must be converted to
  // unsigned char.
  std::transform(str.begin(), str.end(), str.begin(), [](unsigned نویسه c) {
    بازگشت static_cast<نویسه>(std::tolower(c));
  });
  بازگشت str;
}

}  // namespace utils
}  // namespace onnxruntime
