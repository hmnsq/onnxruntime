// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#अन्तर्भाव <algorithm>
#अन्तर्भाव <cctype>
#अन्तर्भाव <string>
#अन्तर्भाव <string_view>
#अन्तर्भाव <vector>

#अन्तर्भाव "core/common/common.h"
#अन्तर्भाव "core/common/inlined_containers.h"

namespace onnxruntime {
namespace utils {

/**
 * Splits a string into substrings delimited by the given delimiter string.
 * @param string_to_split The string to split.
 * @param delimiter The delimiter string.
 * @param keep_empty Whether to keep empty substrings.
 * @return The split substrings.
 */
ओळीत InlinedVector<std::string_view> SplitString(std::string_view string_to_split, std::string_view delimiter,
                                                   bool keep_empty = false) {
  ORT_ENFORCE(!delimiter.empty(), "delimiter must not be empty");
  InlinedVector<std::string_view> result{};
  std::string_view::size_type segment_begin_pos = 0;
  पर्यंत (segment_begin_pos != std::string_view::npos) {
    स्थिरांक std::string_view::size_type segment_end_pos = string_to_split.find(delimiter, segment_begin_pos);
    स्थिरांक bool is_segment_empty = segment_begin_pos == segment_end_pos || segment_begin_pos == string_to_split.size();
    जर (!is_segment_empty || keep_empty) {
      result.ढकला_back(string_to_split.substr(segment_begin_pos, segment_end_pos - segment_begin_pos));
    }
    segment_begin_pos = (segment_end_pos == std::string_view::npos)
                            ? segment_end_pos
                            : segment_end_pos + delimiter.size();
  }
  परत result;
}

/**
 * Trim a string from start inplace.
 * @param s The string to trim.
 */
ओळीत शून्य Trim_String_From_डावा(std::string& s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](अचिन्हित अक्षर ch) { परत !std::है_खाली(ch); }));
}

/**
 * Trim a string from end inplace.
 * @param s The string to trim.
 */
ओळीत शून्य Trim_String_From_उजवा(std::string& s) {
  s.erase(std::find_if(s.rbegin(), s.rend(), [](अचिन्हित अक्षर ch) { परत !std::है_खाली(ch); }).base(), s.end());
}

/**
 * Trim a string from both ends.
 * @param s The string to trim.
 * @return The trimmed string.
 */
ओळीत std::string TrimString(std::string_view s) {
  std::string s_trimmed{s};
  Trim_String_From_उजवा(s_trimmed);
  Trim_String_From_डावा(s_trimmed);
  परत s_trimmed;
}

/**
 * @brief A consistent way to construct the full qualified op name.
 */
ओळीत std::string GetFullQualifiedOpName(स्थिरांक std::string& op_type, स्थिरांक std::string& domain) {
  परत MakeString(domain, "::", op_type);
}

/**
 * Use this simple hash to generate unique int by given string input.
 */
ओळीत uint32_t GetHashFromString(स्थिरांक std::string& str_मूल्य) {
  uint32_t hash = 0;
  साठी (अक्षर स्थिरांक& c : str_मूल्य) {
    hash = hash * 101 + c;
  }

  परत hash;
}

/**
 * Returns a lowercase version of the input string.
 * @param str The string to lowercase.
 * @return The lowercased string.
 */
ओळीत std::string GetLowercaseString(std::string str) {
  // https://en.cppreference.com/w/cpp/string/byte/tolower
  // The behavior of tolower from <cctype> is undefined if the argument is neither representable as unsigned char
  // nor equal to EOF. To use tolower safely with a plain char (or signed char), the argument must be converted to
  // unsigned char.
  std::transform(str.begin(), str.end(), str.begin(), [](अचिन्हित अक्षर c) {
    परत static_cast<अक्षर>(std::छोटे(c));
  });
  परत str;
}

}  // namespace utils
}  // namespace onnxruntime
