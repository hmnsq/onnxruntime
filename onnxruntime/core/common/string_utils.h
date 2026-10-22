// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#અન્તર્ભાવ <algorithm>
#અન્તર્ભાવ <cctype>
#અન્તર્ભાવ <string>
#અન્તર્ભાવ <string_view>
#અન્તર્ભાવ <vector>

#અન્તર્ભાવ "core/common/common.h"
#અન્તર્ભાવ "core/common/inlined_containers.h"

namespace onnxruntime {
namespace utils {

/**
 * Splits a string into substrings delimited by the given delimiter string.
 * @param string_to_split The string to split.
 * @param delimiter The delimiter string.
 * @param keep_empty Whether to keep empty substrings.
 * @return The split substrings.
 */
અંતઃરેખ InlinedVector<std::string_view> SplitString(std::string_view string_to_split, std::string_view delimiter,
                                                   bool keep_empty = false) {
  ORT_ENFORCE(!delimiter.empty(), "delimiter must not be empty");
  InlinedVector<std::string_view> result{};
  std::string_view::size_type segment_begin_pos = 0;
  જ્યારે (segment_begin_pos != std::string_view::npos) {
    અચલ std::string_view::size_type segment_end_pos = string_to_split.find(delimiter, segment_begin_pos);
    અચલ bool is_segment_empty = segment_begin_pos == segment_end_pos || segment_begin_pos == string_to_split.size();
    જો (!is_segment_empty || keep_empty) {
      result.ધકેલો_back(string_to_split.substr(segment_begin_pos, segment_end_pos - segment_begin_pos));
    }
    segment_begin_pos = (segment_end_pos == std::string_view::npos)
                            ? segment_end_pos
                            : segment_end_pos + delimiter.size();
  }
  પાછા result;
}

/**
 * Trim a string from start inplace.
 * @param s The string to trim.
 */
અંતઃરેખ શૂન્ય Trim_String_From_ડાબું(std::string& s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned અક્ષર ch) { પાછા !std::isspace(ch); }));
}

/**
 * Trim a string from end inplace.
 * @param s The string to trim.
 */
અંતઃરેખ શૂન્ય Trim_String_From_જમણું(std::string& s) {
  s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned અક્ષર ch) { પાછા !std::isspace(ch); }).base(), s.end());
}

/**
 * Trim a string from both ends.
 * @param s The string to trim.
 * @return The trimmed string.
 */
અંતઃરેખ std::string TrimString(std::string_view s) {
  std::string s_trimmed{s};
  Trim_String_From_જમણું(s_trimmed);
  Trim_String_From_ડાબું(s_trimmed);
  પાછા s_trimmed;
}

/**
 * @brief A consistent way to construct the full qualified op name.
 */
અંતઃરેખ std::string GetFullQualifiedOpName(અચલ std::string& op_type, અચલ std::string& domain) {
  પાછા MakeString(domain, "::", op_type);
}

/**
 * Use this simple hash to generate unique int by given string input.
 */
અંતઃરેખ uint32_t GetHashFromString(અચલ std::string& str_કિંમત) {
  uint32_t hash = 0;
  માટે (અક્ષર અચલ& c : str_કિંમત) {
    hash = hash * 101 + c;
  }

  પાછા hash;
}

/**
 * Returns a lowercase version of the input string.
 * @param str The string to lowercase.
 * @return The lowercased string.
 */
અંતઃરેખ std::string GetLowercaseString(std::string str) {
  // https://en.cppreference.com/w/cpp/string/byte/tolower
  // The behavior of tolower from <cctype> is undefined if the argument is neither representable as unsigned char
  // nor equal to EOF. To use tolower safely with a plain char (or signed char), the argument must be converted to
  // unsigned char.
  std::transform(str.begin(), str.end(), str.begin(), [](unsigned અક્ષર c) {
    પાછા static_cast<અક્ષર>(std::tolower(c));
  });
  પાછા str;
}

}  // namespace utils
}  // namespace onnxruntime
