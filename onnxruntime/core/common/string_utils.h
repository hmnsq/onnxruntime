// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#অন্তর্ভাৱ <algorithm>
#অন্তর্ভাৱ <cctype>
#অন্তর্ভাৱ <string>
#অন্তর্ভাৱ <string_view>
#অন্তর্ভাৱ <vector>

#অন্তর্ভাৱ "core/common/common.h"
#অন্তর্ভাৱ "core/common/inlined_containers.h"

namespace onnxruntime {
namespace utils {

/**
 * Splits a string into substrings delimited by the given delimiter string.
 * @param string_to_split The string to split.
 * @param delimiter The delimiter string.
 * @param keep_empty Whether to keep empty substrings.
 * @return The split substrings.
 */
অন্তঃস্থ InlinedVector<std::string_view> SplitString(std::string_view string_to_split, std::string_view delimiter,
                                                   bool keep_empty = false) {
  ORT_ENFORCE(!delimiter.empty(), "delimiter must not be empty");
  InlinedVector<std::string_view> result{};
  std::string_view::size_type segment_begin_pos = 0;
  যেতিয়ালৈ (segment_begin_pos != std::string_view::npos) {
    ধ্রুৱ std::string_view::size_type segment_end_pos = string_to_split.find(delimiter, segment_begin_pos);
    ধ্রুৱ bool is_segment_empty = segment_begin_pos == segment_end_pos || segment_begin_pos == string_to_split.size();
    যদি (!is_segment_empty || keep_empty) {
      result.ঠেলো_back(string_to_split.substr(segment_begin_pos, segment_end_pos - segment_begin_pos));
    }
    segment_begin_pos = (segment_end_pos == std::string_view::npos)
                            ? segment_end_pos
                            : segment_end_pos + delimiter.size();
  }
  ঘূৰাই result;
}

/**
 * Trim a string from start inplace.
 * @param s The string to trim.
 */
অন্তঃস্থ শূন্য Trim_String_From_বাম(std::string& s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned ৱর্ণ ch) { ঘূৰাই !std::isspace(ch); }));
}

/**
 * Trim a string from end inplace.
 * @param s The string to trim.
 */
অন্তঃস্থ শূন্য Trim_String_From_ডান(std::string& s) {
  s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned ৱর্ণ ch) { ঘূৰাই !std::isspace(ch); }).base(), s.end());
}

/**
 * Trim a string from both ends.
 * @param s The string to trim.
 * @return The trimmed string.
 */
অন্তঃস্থ std::string TrimString(std::string_view s) {
  std::string s_trimmed{s};
  Trim_String_From_ডান(s_trimmed);
  Trim_String_From_বাম(s_trimmed);
  ঘূৰাই s_trimmed;
}

/**
 * @brief A consistent way to construct the full qualified op name.
 */
অন্তঃস্থ std::string GetFullQualifiedOpName(ধ্রুৱ std::string& op_type, ধ্রুৱ std::string& domain) {
  ঘূৰাই MakeString(domain, "::", op_type);
}

/**
 * Use this simple hash to generate unique int by given string input.
 */
অন্তঃস্থ uint32_t GetHashFromString(ধ্রুৱ std::string& str_মান) {
  uint32_t hash = 0;
  বাবে (ৱর্ণ ধ্রুৱ& c : str_মান) {
    hash = hash * 101 + c;
  }

  ঘূৰাই hash;
}

/**
 * Returns a lowercase version of the input string.
 * @param str The string to lowercase.
 * @return The lowercased string.
 */
অন্তঃস্থ std::string GetLowercaseString(std::string str) {
  // https://en.cppreference.com/w/cpp/string/byte/tolower
  // The behavior of tolower from <cctype> is undefined if the argument is neither representable as unsigned char
  // nor equal to EOF. To use tolower safely with a plain char (or signed char), the argument must be converted to
  // unsigned char.
  std::transform(str.begin(), str.end(), str.begin(), [](unsigned ৱর্ণ c) {
    ঘূৰাই static_cast<ৱর্ণ>(std::tolower(c));
  });
  ঘূৰাই str;
}

}  // namespace utils
}  // namespace onnxruntime
