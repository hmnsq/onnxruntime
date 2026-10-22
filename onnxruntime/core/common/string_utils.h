// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ہدایت once

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
اندرونی Inlined_سمتیہ<std::string_view> بانٹو_String(std::string_view string_مقصد_بانٹو, std::string_view delimiter,
                                                   bool keep_خالی_خانے = false) {
  ORT_ENFORCE(!delimiter.خالی_خانے(), "delimiter must not be empty");
  Inlined_سمتیہ<std::string_view> نتیجہ{};
  std::string_view::سائز_type segment_begin_pos = 0;
  جب (segment_begin_pos != std::string_view::npos) {
    ثابت std::string_view::سائز_type segment_انتہا_pos = string_مقصد_بانٹو.ڈھونڈو(delimiter, segment_begin_pos);
    ثابت bool is_segment_خالی_خانے = segment_begin_pos == segment_انتہا_pos || segment_begin_pos == string_مقصد_بانٹو.سائز();
    اگر (!is_segment_خالی_خانے || keep_خالی_خانے) {
      نتیجہ.ڈالو_عقب(string_مقصد_بانٹو.substr(segment_begin_pos, segment_انتہا_pos - segment_begin_pos));
    }
    segment_begin_pos = (segment_انتہا_pos == std::string_view::npos)
                            ? segment_انتہا_pos
                            : segment_انتہا_pos + delimiter.سائز();
  }
  واپس نتیجہ;
}

/**
 * Trim a string from start inplace.
 * @param s The string to trim.
 */
اندرونی خالی Trim_String_From_بایاں(std::string& s) {
  s.erase(s.begin(), std::ڈھونڈو_if(s.begin(), s.انتہا(), [](غیرعلامتی حرف ch) { واپس !std::خلا_ہے(ch); }));
}

/**
 * Trim a string from end inplace.
 * @param s The string to trim.
 */
اندرونی خالی Trim_String_From_دایاں(std::string& s) {
  s.erase(std::ڈھونڈو_if(s.rbegin(), s.rend(), [](غیرعلامتی حرف ch) { واپس !std::خلا_ہے(ch); }).بنیاد(), s.انتہا());
}

/**
 * Trim a string from both ends.
 * @param s The string to trim.
 * @return The trimmed string.
 */
اندرونی std::string TrimString(std::string_view s) {
  std::string s_trimmed{s};
  Trim_String_From_دایاں(s_trimmed);
  Trim_String_From_بایاں(s_trimmed);
  واپس s_trimmed;
}

/**
 * @brief A consistent way to construct the full qualified op name.
 */
اندرونی std::string Get_بھرا_Qualified_Op_Name(ثابت std::string& op_type, ثابت std::string& domain) {
  واپس MakeString(domain, "::", op_type);
}

/**
 * Use this simple hash to generate unique int by given string input.
 */
اندرونی uint32_t GetHashFromString(ثابت std::string& str_قدر) {
  uint32_t hash = 0;
  برائے (حرف ثابت& c : str_قدر) {
    hash = hash * 101 + c;
  }

  واپس hash;
}

/**
 * Returns a lowercase version of the input string.
 * @param str The string to lowercase.
 * @return The lowercased string.
 */
اندرونی std::string GetLowercaseString(std::string str) {
  // https://en.cppreference.com/w/cpp/string/byte/tolower
  // The behavior of tolower from <cctype> is undefined if the argument is neither representable as unsigned char
  // nor equal to EOF. To use tolower safely with a plain char (or signed char), the argument must be converted to
  // unsigned char.
  std::transform(str.begin(), str.انتہا(), str.begin(), [](غیرعلامتی حرف c) {
    واپس static_cast<حرف>(std::tolower(c));
  });
  واپس str;
}

}  // namespace utils
}  // namespace onnxruntime
