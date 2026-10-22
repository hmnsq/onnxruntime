// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#அந்தர்பாவ <algorithm>
#அந்தர்பாவ <cctype>
#அந்தர்பாவ <string>
#அந்தர்பாவ <string_view>
#அந்தர்பாவ <vector>

#அந்தர்பாவ "core/common/common.h"
#அந்தர்பாவ "core/common/inlined_containers.h"

namespace onnxruntime {
namespace utils {

/**
 * Splits a string into substrings delimited by the given delimiter string.
 * @param string_to_split The string to split.
 * @param delimiter The delimiter string.
 * @param keep_empty Whether to keep empty substrings.
 * @return The split substrings.
 */
உள்வரி InlinedVector<std::string_view> SplitString(std::string_view string_to_split, std::string_view delimiter,
                                                   bool keep_empty = false) {
  ORT_ENFORCE(!delimiter.empty(), "delimiter must not be empty");
  InlinedVector<std::string_view> result{};
  std::string_view::size_type segment_begin_pos = 0;
  போது (segment_begin_pos != std::string_view::npos) {
    மாறிலி std::string_view::size_type segment_end_pos = string_to_split.find(delimiter, segment_begin_pos);
    மாறிலி bool is_segment_empty = segment_begin_pos == segment_end_pos || segment_begin_pos == string_to_split.size();
    எனில் (!is_segment_empty || keep_empty) {
      result.தள்ளு_back(string_to_split.substr(segment_begin_pos, segment_end_pos - segment_begin_pos));
    }
    segment_begin_pos = (segment_end_pos == std::string_view::npos)
                            ? segment_end_pos
                            : segment_end_pos + delimiter.size();
  }
  திரும்பு result;
}

/**
 * Trim a string from start inplace.
 * @param s The string to trim.
 */
உள்வரி வெறுமை Trim_String_From_இடது(std::string& s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned எழுத்து ch) { திரும்பு !std::isspace(ch); }));
}

/**
 * Trim a string from end inplace.
 * @param s The string to trim.
 */
உள்வரி வெறுமை Trim_String_From_வலது(std::string& s) {
  s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned எழுத்து ch) { திரும்பு !std::isspace(ch); }).base(), s.end());
}

/**
 * Trim a string from both ends.
 * @param s The string to trim.
 * @return The trimmed string.
 */
உள்வரி std::string TrimString(std::string_view s) {
  std::string s_trimmed{s};
  Trim_String_From_வலது(s_trimmed);
  Trim_String_From_இடது(s_trimmed);
  திரும்பு s_trimmed;
}

/**
 * @brief A consistent way to construct the full qualified op name.
 */
உள்வரி std::string GetFullQualifiedOpName(மாறிலி std::string& op_type, மாறிலி std::string& domain) {
  திரும்பு MakeString(domain, "::", op_type);
}

/**
 * Use this simple hash to generate unique int by given string input.
 */
உள்வரி uint32_t GetHashFromString(மாறிலி std::string& str_மதிப்பு) {
  uint32_t hash = 0;
  க்கு (எழுத்து மாறிலி& c : str_மதிப்பு) {
    hash = hash * 101 + c;
  }

  திரும்பு hash;
}

/**
 * Returns a lowercase version of the input string.
 * @param str The string to lowercase.
 * @return The lowercased string.
 */
உள்வரி std::string GetLowercaseString(std::string str) {
  // https://en.cppreference.com/w/cpp/string/byte/tolower
  // The behavior of tolower from <cctype> is undefined if the argument is neither representable as unsigned char
  // nor equal to EOF. To use tolower safely with a plain char (or signed char), the argument must be converted to
  // unsigned char.
  std::transform(str.begin(), str.end(), str.begin(), [](unsigned எழுத்து c) {
    திரும்பு static_cast<எழுத்து>(std::tolower(c));
  });
  திரும்பு str;
}

}  // namespace utils
}  // namespace onnxruntime
