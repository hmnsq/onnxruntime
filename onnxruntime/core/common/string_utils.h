// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#هدايت once

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
مضمن Inlined_ويڪٽر<std::string_view> ورهايو_String(std::string_view string_منزل_ورهايو, std::string_view delimiter,
                                                   bool keep_خالي_جايون = false) {
  ORT_ENFORCE(!delimiter.خالي_جايون(), "delimiter must not be empty");
  Inlined_ويڪٽر<std::string_view> نتيجو{};
  std::string_view::سائيز_type segment_begin_pos = 0;
  طالما (segment_begin_pos != std::string_view::npos) {
    ثابت std::string_view::سائيز_type segment_پڇاڙي_pos = string_منزل_ورهايو.ڳوليو(delimiter, segment_begin_pos);
    ثابت bool is_segment_خالي_جايون = segment_begin_pos == segment_پڇاڙي_pos || segment_begin_pos == string_منزل_ورهايو.سائيز();
    إذا (!is_segment_خالي_جايون || keep_خالي_جايون) {
      نتيجو.ڌڪو_پٺ(string_منزل_ورهايو.substr(segment_begin_pos, segment_پڇاڙي_pos - segment_begin_pos));
    }
    segment_begin_pos = (segment_پڇاڙي_pos == std::string_view::npos)
                            ? segment_پڇاڙي_pos
                            : segment_پڇاڙي_pos + delimiter.سائيز();
  }
  أرجع نتيجو;
}

/**
 * Trim a string from start inplace.
 * @param s The string to trim.
 */
مضمن فراغ Trim_String_From_کاٻو(std::string& s) {
  s.erase(s.begin(), std::ڳوليو_if(s.begin(), s.پڇاڙي(), [](بنانشان حرف ch) { أرجع !std::خال_آهي(ch); }));
}

/**
 * Trim a string from end inplace.
 * @param s The string to trim.
 */
مضمن فراغ Trim_String_From_ساڄو(std::string& s) {
  s.erase(std::ڳوليو_if(s.rbegin(), s.rend(), [](بنانشان حرف ch) { أرجع !std::خال_آهي(ch); }).بنياد(), s.پڇاڙي());
}

/**
 * Trim a string from both ends.
 * @param s The string to trim.
 * @return The trimmed string.
 */
مضمن std::string TrimString(std::string_view s) {
  std::string s_trimmed{s};
  Trim_String_From_ساڄو(s_trimmed);
  Trim_String_From_کاٻو(s_trimmed);
  أرجع s_trimmed;
}

/**
 * @brief A consistent way to construct the full qualified op name.
 */
مضمن std::string Get_ڀريل_Qualified_Op_Name(ثابت std::string& op_type, ثابت std::string& domain) {
  أرجع MakeString(domain, "::", op_type);
}

/**
 * Use this simple hash to generate unique int by given string input.
 */
مضمن uint32_t GetHashFromString(ثابت std::string& str_قدر) {
  uint32_t hash = 0;
  لكل (حرف ثابت& c : str_قدر) {
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
  std::transform(str.begin(), str.پڇاڙي(), str.begin(), [](بنانشان حرف c) {
    أرجع static_cast<حرف>(std::tolower(c));
  });
  أرجع str;
}

}  // namespace utils
}  // namespace onnxruntime
