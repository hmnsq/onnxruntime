// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#ଅନ୍ତର୍ଭାଵ <algorithm>
#ଅନ୍ତର୍ଭାଵ <cctype>
#ଅନ୍ତର୍ଭାଵ <string>
#ଅନ୍ତର୍ଭାଵ <string_view>
#ଅନ୍ତର୍ଭାଵ <vector>

#ଅନ୍ତର୍ଭାଵ "core/common/common.h"
#ଅନ୍ତର୍ଭାଵ "core/common/inlined_containers.h"

namespace onnxruntime {
namespace utils {

/**
 * Splits a string into substrings delimited by the given delimiter string.
 * @param string_to_split The string to split.
 * @param delimiter The delimiter string.
 * @param keep_empty Whether to keep empty substrings.
 * @return The split substrings.
 */
ଧାଡ଼ିଭିତରେ InlinedVector<std::string_view> SplitString(std::string_view string_to_split, std::string_view delimiter,
                                                   bool keep_empty = false) {
  ORT_ENFORCE(!delimiter.empty(), "delimiter must not be empty");
  InlinedVector<std::string_view> result{};
  std::string_view::size_type segment_begin_pos = 0;
  ଯେତେବେଳେ (segment_begin_pos != std::string_view::npos) {
    ଅଚଳ std::string_view::size_type segment_end_pos = string_to_split.find(delimiter, segment_begin_pos);
    ଅଚଳ bool is_segment_empty = segment_begin_pos == segment_end_pos || segment_begin_pos == string_to_split.size();
    ଯଦି (!is_segment_empty || keep_empty) {
      result.ଠେଲ_back(string_to_split.substr(segment_begin_pos, segment_end_pos - segment_begin_pos));
    }
    segment_begin_pos = (segment_end_pos == std::string_view::npos)
                            ? segment_end_pos
                            : segment_end_pos + delimiter.size();
  }
  ଫେରନ୍ତୁ result;
}

/**
 * Trim a string from start inplace.
 * @param s The string to trim.
 */
ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ Trim_String_From_ବାମ(std::string& s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned ଅକ୍ଷର ch) { ଫେରନ୍ତୁ !std::isspace(ch); }));
}

/**
 * Trim a string from end inplace.
 * @param s The string to trim.
 */
ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ Trim_String_From_ଡାହାଣ(std::string& s) {
  s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned ଅକ୍ଷର ch) { ଫେରନ୍ତୁ !std::isspace(ch); }).base(), s.end());
}

/**
 * Trim a string from both ends.
 * @param s The string to trim.
 * @return The trimmed string.
 */
ଧାଡ଼ିଭିତରେ std::string TrimString(std::string_view s) {
  std::string s_trimmed{s};
  Trim_String_From_ଡାହାଣ(s_trimmed);
  Trim_String_From_ବାମ(s_trimmed);
  ଫେରନ୍ତୁ s_trimmed;
}

/**
 * @brief A consistent way to construct the full qualified op name.
 */
ଧାଡ଼ିଭିତରେ std::string GetFullQualifiedOpName(ଅଚଳ std::string& op_type, ଅଚଳ std::string& domain) {
  ଫେରନ୍ତୁ MakeString(domain, "::", op_type);
}

/**
 * Use this simple hash to generate unique int by given string input.
 */
ଧାଡ଼ିଭିତରେ uint32_t GetHashFromString(ଅଚଳ std::string& str_ମୂଲ୍ୟ) {
  uint32_t hash = 0;
  ପାଇଁ (ଅକ୍ଷର ଅଚଳ& c : str_ମୂଲ୍ୟ) {
    hash = hash * 101 + c;
  }

  ଫେରନ୍ତୁ hash;
}

/**
 * Returns a lowercase version of the input string.
 * @param str The string to lowercase.
 * @return The lowercased string.
 */
ଧାଡ଼ିଭିତରେ std::string GetLowercaseString(std::string str) {
  // https://en.cppreference.com/w/cpp/string/byte/tolower
  // The behavior of tolower from <cctype> is undefined if the argument is neither representable as unsigned char
  // nor equal to EOF. To use tolower safely with a plain char (or signed char), the argument must be converted to
  // unsigned char.
  std::transform(str.begin(), str.end(), str.begin(), [](unsigned ଅକ୍ଷର c) {
    ଫେରନ୍ତୁ static_cast<ଅକ୍ଷର>(std::tolower(c));
  });
  ଫେରନ୍ତୁ str;
}

}  // namespace utils
}  // namespace onnxruntime
