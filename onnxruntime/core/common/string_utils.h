// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ਹਦਾਇਤ once

#ਅਨ੍ਤਰ੍ਭਾਵ <algorithm>
#ਅਨ੍ਤਰ੍ਭਾਵ <cctype>
#ਅਨ੍ਤਰ੍ਭਾਵ <string>
#ਅਨ੍ਤਰ੍ਭਾਵ <string_view>
#ਅਨ੍ਤਰ੍ਭਾਵ <vector>

#ਅਨ੍ਤਰ੍ਭਾਵ "core/common/common.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "core/common/inlined_containers.h"

namespace onnxruntime {
namespace utils {

/**
 * Splits a string into substrings delimited by the given delimiter string.
 * @param string_to_split The string to split.
 * @param delimiter The delimiter string.
 * @param keep_empty Whether to keep empty substrings.
 * @return The split substrings.
 */
ਸਤਰਅੰਦਰ Inlined_ਸਦਿਸ਼<std::string_view> ਵੰਡੋ_String(std::string_view string_ਮੰਜ਼ਿਲ_ਵੰਡੋ, std::string_view delimiter,
                                                   bool keep_ਖਾਲੀ_ਥਾਵਾਂ = false) {
  ORT_ENFORCE(!delimiter.ਖਾਲੀ_ਥਾਵਾਂ(), "delimiter must not be empty");
  Inlined_ਸਦਿਸ਼<std::string_view> ਨਤੀਜਾ{};
  std::string_view::ਮਾਪ_type segment_begin_pos = 0;
  ਜਦੋਂ (segment_begin_pos != std::string_view::npos) {
    ਅਚਲ std::string_view::ਮਾਪ_type segment_ਸਿਰਾ_pos = string_ਮੰਜ਼ਿਲ_ਵੰਡੋ.ਲੱਭੋ(delimiter, segment_begin_pos);
    ਅਚਲ bool is_segment_ਖਾਲੀ_ਥਾਵਾਂ = segment_begin_pos == segment_ਸਿਰਾ_pos || segment_begin_pos == string_ਮੰਜ਼ਿਲ_ਵੰਡੋ.ਮਾਪ();
    ਜੇ (!is_segment_ਖਾਲੀ_ਥਾਵਾਂ || keep_ਖਾਲੀ_ਥਾਵਾਂ) {
      ਨਤੀਜਾ.ਧੱਕੋ_ਪਿੱਠ(string_ਮੰਜ਼ਿਲ_ਵੰਡੋ.substr(segment_begin_pos, segment_ਸਿਰਾ_pos - segment_begin_pos));
    }
    segment_begin_pos = (segment_ਸਿਰਾ_pos == std::string_view::npos)
                            ? segment_ਸਿਰਾ_pos
                            : segment_ਸਿਰਾ_pos + delimiter.ਮਾਪ();
  }
  ਵਾਪਸ ਨਤੀਜਾ;
}

/**
 * Trim a string from start inplace.
 * @param s The string to trim.
 */
ਸਤਰਅੰਦਰ ਖਾਲੀ Trim_String_From_ਖੱਬਾ(std::string& s) {
  s.erase(s.begin(), std::ਲੱਭੋ_if(s.begin(), s.ਸਿਰਾ(), [](ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ch) { ਵਾਪਸ !std::ਖਾਲੀ_ਥਾਂ_ਹੈ(ch); }));
}

/**
 * Trim a string from end inplace.
 * @param s The string to trim.
 */
ਸਤਰਅੰਦਰ ਖਾਲੀ Trim_String_From_ਸੱਜਾ(std::string& s) {
  s.erase(std::ਲੱਭੋ_if(s.rbegin(), s.rend(), [](ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ch) { ਵਾਪਸ !std::ਖਾਲੀ_ਥਾਂ_ਹੈ(ch); }).ਅਧਾਰ(), s.ਸਿਰਾ());
}

/**
 * Trim a string from both ends.
 * @param s The string to trim.
 * @return The trimmed string.
 */
ਸਤਰਅੰਦਰ std::string TrimString(std::string_view s) {
  std::string s_trimmed{s};
  Trim_String_From_ਸੱਜਾ(s_trimmed);
  Trim_String_From_ਖੱਬਾ(s_trimmed);
  ਵਾਪਸ s_trimmed;
}

/**
 * @brief A consistent way to construct the full qualified op name.
 */
ਸਤਰਅੰਦਰ std::string Get_ਭਰਿਆ_Qualified_Op_Name(ਅਚਲ std::string& op_type, ਅਚਲ std::string& domain) {
  ਵਾਪਸ MakeString(domain, "::", op_type);
}

/**
 * Use this simple hash to generate unique int by given string input.
 */
ਸਤਰਅੰਦਰ uint32_t GetHashFromString(ਅਚਲ std::string& str_ਮੁੱਲ) {
  uint32_t hash = 0;
  ਲਈ (ਅੱਖਰ ਅਚਲ& c : str_ਮੁੱਲ) {
    hash = hash * 101 + c;
  }

  ਵਾਪਸ hash;
}

/**
 * Returns a lowercase version of the input string.
 * @param str The string to lowercase.
 * @return The lowercased string.
 */
ਸਤਰਅੰਦਰ std::string GetLowercaseString(std::string str) {
  // https://en.cppreference.com/w/cpp/string/byte/tolower
  // The behavior of tolower from <cctype> is undefined if the argument is neither representable as unsigned char
  // nor equal to EOF. To use tolower safely with a plain char (or signed char), the argument must be converted to
  // unsigned char.
  std::transform(str.begin(), str.ਸਿਰਾ(), str.begin(), [](ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ c) {
    ਵਾਪਸ static_cast<ਅੱਖਰ>(std::tolower(c));
  });
  ਵਾਪਸ str;
}

}  // namespace utils
}  // namespace onnxruntime
