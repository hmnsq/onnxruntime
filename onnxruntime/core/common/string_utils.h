// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#لارښوونه once

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
مضمن Inlined_ویکټور<std::string_view> وېشل_String(std::string_view string_منزل_وېشل, std::string_view delimiter,
                                                   bool keep_تشه = false) {
  ORT_ENFORCE(!delimiter.تشه(), "delimiter must not be empty");
  Inlined_ویکټور<std::string_view> پایله{};
  std::string_view::اندازه_type segment_begin_pos = 0;
  طالما (segment_begin_pos != std::string_view::npos) {
    ثابت std::string_view::اندازه_type segment_پای_ټکی_pos = string_منزل_وېشل.پیدا_کړه(delimiter, segment_begin_pos);
    ثابت bool is_segment_تشه = segment_begin_pos == segment_پای_ټکی_pos || segment_begin_pos == string_منزل_وېشل.اندازه();
    إذا (!is_segment_تشه || keep_تشه) {
      پایله.ټېل_شا(string_منزل_وېشل.substr(segment_begin_pos, segment_پای_ټکی_pos - segment_begin_pos));
    }
    segment_begin_pos = (segment_پای_ټکی_pos == std::string_view::npos)
                            ? segment_پای_ټکی_pos
                            : segment_پای_ټکی_pos + delimiter.اندازه();
  }
  أرجع پایله;
}

/**
 * Trim a string from start inplace.
 * @param s The string to trim.
 */
مضمن فراغ Trim_String_From_کیڼ(std::string& s) {
  s.erase(s.begin(), std::پیدا_کړه_if(s.begin(), s.پای_ټکی(), [](بینښه حرف ch) { أرجع !std::که_تشه(ch); }));
}

/**
 * Trim a string from end inplace.
 * @param s The string to trim.
 */
مضمن فراغ Trim_String_From_ښی(std::string& s) {
  s.erase(std::پیدا_کړه_if(s.rbegin(), s.rend(), [](بینښه حرف ch) { أرجع !std::که_تشه(ch); }).بنسټ(), s.پای_ټکی());
}

/**
 * Trim a string from both ends.
 * @param s The string to trim.
 * @return The trimmed string.
 */
مضمن std::string TrimString(std::string_view s) {
  std::string s_trimmed{s};
  Trim_String_From_ښی(s_trimmed);
  Trim_String_From_کیڼ(s_trimmed);
  أرجع s_trimmed;
}

/**
 * @brief A consistent way to construct the full qualified op name.
 */
مضمن std::string Get_ډک_Qualified_Op_Name(ثابت std::string& op_type, ثابت std::string& domain) {
  أرجع MakeString(domain, "::", op_type);
}

/**
 * Use this simple hash to generate unique int by given string input.
 */
مضمن uint32_t GetHashFromString(ثابت std::string& str_ارزښت) {
  uint32_t hash = 0;
  لكل (حرف ثابت& c : str_ارزښت) {
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
  std::transform(str.begin(), str.پای_ټکی(), str.begin(), [](بینښه حرف c) {
    أرجع static_cast<حرف>(std::tolower(c));
  });
  أرجع str;
}

}  // namespace utils
}  // namespace onnxruntime
