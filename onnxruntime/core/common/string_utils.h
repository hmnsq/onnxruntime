// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#निर्देशः once

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
अन्तःस्थ Inlined_सदिशम्<std::string_view> विभज_String(std::string_view string_गन्तव्यम्_विभज, std::string_view delimiter,
                                                   bool keep_रिक्तानि = false) {
  ORT_ENFORCE(!delimiter.रिक्तानि(), "delimiter must not be empty");
  Inlined_सदिशम्<std::string_view> परिणाम{};
  std::string_view::आकार_type segment_begin_pos = 0;
  यावत् (segment_begin_pos != std::string_view::npos) {
    ध्रुव std::string_view::आकार_type segment_अन्तः_pos = string_गन्तव्यम्_विभज.find(delimiter, segment_begin_pos);
    ध्रुव bool is_segment_रिक्तानि = segment_begin_pos == segment_अन्तः_pos || segment_begin_pos == string_गन्तव्यम्_विभज.आकार();
    यदि (!is_segment_रिक्तानि || keep_रिक्तानि) {
      परिणाम.प्रक्षिप_पृष्ठभागः(string_गन्तव्यम्_विभज.substr(segment_begin_pos, segment_अन्तः_pos - segment_begin_pos));
    }
    segment_begin_pos = (segment_अन्तः_pos == std::string_view::npos)
                            ? segment_अन्तः_pos
                            : segment_अन्तः_pos + delimiter.आकार();
  }
  प्रत्यागम परिणाम;
}

/**
 * Trim a string from start inplace.
 * @param s The string to trim.
 */
अन्तःस्थ शून्य Trim_String_From_वामः(std::string& s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.अन्तः(), [](अचिह्नित वर्ण ch) { प्रत्यागम !std::रिक्तम्_वा(ch); }));
}

/**
 * Trim a string from end inplace.
 * @param s The string to trim.
 */
अन्तःस्थ शून्य Trim_String_From_दक्षिणः(std::string& s) {
  s.erase(std::find_if(s.rbegin(), s.rend(), [](अचिह्नित वर्ण ch) { प्रत्यागम !std::रिक्तम्_वा(ch); }).आधारः(), s.अन्तः());
}

/**
 * Trim a string from both ends.
 * @param s The string to trim.
 * @return The trimmed string.
 */
अन्तःस्थ std::string TrimString(std::string_view s) {
  std::string s_trimmed{s};
  Trim_String_From_दक्षिणः(s_trimmed);
  Trim_String_From_वामः(s_trimmed);
  प्रत्यागम s_trimmed;
}

/**
 * @brief A consistent way to construct the full qualified op name.
 */
अन्तःस्थ std::string Get_पूर्णम्_Qualified_Op_Name(ध्रुव std::string& op_type, ध्रुव std::string& domain) {
  प्रत्यागम MakeString(domain, "::", op_type);
}

/**
 * Use this simple hash to generate unique int by given string input.
 */
अन्तःस्थ uint32_t GetHashFromString(ध्रुव std::string& str_मानम्) {
  uint32_t hash = 0;
  प्रति (वर्ण ध्रुव& c : str_मानम्) {
    hash = hash * 101 + c;
  }

  प्रत्यागम hash;
}

/**
 * Returns a lowercase version of the input string.
 * @param str The string to lowercase.
 * @return The lowercased string.
 */
अन्तःस्थ std::string GetLowercaseString(std::string str) {
  // https://en.cppreference.com/w/cpp/string/byte/tolower
  // The behavior of tolower from <cctype> is undefined if the argument is neither representable as unsigned char
  // nor equal to EOF. To use tolower safely with a plain char (or signed char), the argument must be converted to
  // unsigned char.
  std::transform(str.begin(), str.अन्तः(), str.begin(), [](अचिह्नित वर्ण c) {
    प्रत्यागम static_cast<वर्ण>(std::tolower(c));
  });
  प्रत्यागम str;
}

}  // namespace utils
}  // namespace onnxruntime
