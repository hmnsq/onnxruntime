// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#आशय once

#समावेश <algorithm>
#समावेश <cctype>
#समावेश <string>
#समावेश <string_view>
#समावेश <vector>

#समावेश "core/common/common.h"
#समावेश "core/common/inlined_containers.h"

namespace onnxruntime {
namespace utils {

/**
 * Splits a string into substrings delimited by the given delimiter string.
 * @param string_to_split The string to split.
 * @param delimiter The delimiter string.
 * @param keep_empty Whether to keep empty substrings.
 * @return The split substrings.
 */
अंतरभूत InlinedVector<std::string_view> SplitString(std::string_view string_to_split, std::string_view delimiter,
                                                   bool keep_empty = false) {
  ORT_ENFORCE(!delimiter.empty(), "delimiter must not be empty");
  InlinedVector<std::string_view> परिणाम{};
  std::string_view::आकार_type segment_begin_pos = 0;
  जबतक (segment_begin_pos != std::string_view::npos) {
    स्थिर std::string_view::आकार_type segment_end_pos = string_to_split.find(delimiter, segment_begin_pos);
    स्थिर bool is_segment_empty = segment_begin_pos == segment_end_pos || segment_begin_pos == string_to_split.आकार();
    अगर (!is_segment_empty || keep_empty) {
      परिणाम.धकेलो_back(string_to_split.substr(segment_begin_pos, segment_end_pos - segment_begin_pos));
    }
    segment_begin_pos = (segment_end_pos == std::string_view::npos)
                            ? segment_end_pos
                            : segment_end_pos + delimiter.आकार();
  }
  वापस परिणाम;
}

/**
 * Trim a string from start inplace.
 * @param s The string to trim.
 */
अंतरभूत व्योम Trim_String_From_बायाँ(std::string& s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](अचिन्हित अक्षर ch) { वापस !std::है_खाली(ch); }));
}

/**
 * Trim a string from end inplace.
 * @param s The string to trim.
 */
अंतरभूत व्योम Trim_String_From_दायाँ(std::string& s) {
  s.erase(std::find_if(s.rbegin(), s.rend(), [](अचिन्हित अक्षर ch) { वापस !std::है_खाली(ch); }).base(), s.end());
}

/**
 * Trim a string from both ends.
 * @param s The string to trim.
 * @return The trimmed string.
 */
अंतरभूत std::string TrimString(std::string_view s) {
  std::string s_trimmed{s};
  Trim_String_From_दायाँ(s_trimmed);
  Trim_String_From_बायाँ(s_trimmed);
  वापस s_trimmed;
}

/**
 * @brief A consistent way to construct the full qualified op name.
 */
अंतरभूत std::string GetFullQualifiedOpName(स्थिर std::string& op_type, स्थिर std::string& domain) {
  वापस MakeString(domain, "::", op_type);
}

/**
 * Use this simple hash to generate unique int by given string input.
 */
अंतरभूत uint32_t GetHashFromString(स्थिर std::string& str_मूल्य) {
  uint32_t hash = 0;
  क्रम (अक्षर स्थिर& c : str_मूल्य) {
    hash = hash * 101 + c;
  }

  वापस hash;
}

/**
 * Returns a lowercase version of the input string.
 * @param str The string to lowercase.
 * @return The lowercased string.
 */
अंतरभूत std::string GetLowercaseString(std::string str) {
  // https://en.cppreference.com/w/cpp/string/byte/tolower
  // The behavior of tolower from <cctype> is undefined if the argument is neither representable as unsigned char
  // nor equal to EOF. To use tolower safely with a plain char (or signed char), the argument must be converted to
  // unsigned char.
  std::transform(str.begin(), str.end(), str.begin(), [](अचिन्हित अक्षर c) {
    वापस static_cast<अक्षर>(std::छोटे(c));
  });
  वापस str;
}

}  // namespace utils
}  // namespace onnxruntime
