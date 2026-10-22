// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#సూచన once

#అన్తర్భావ <algorithm>
#అన్తర్భావ <cctype>
#అన్తర్భావ <string>
#అన్తర్భావ <string_view>
#అన్తర్భావ <vector>

#అన్తర్భావ "core/common/common.h"
#అన్తర్భావ "core/common/inlined_containers.h"

namespace onnxruntime {
namespace utils {

/**
 * Splits a string into substrings delimited by the given delimiter string.
 * @param string_to_split The string to split.
 * @param delimiter The delimiter string.
 * @param keep_empty Whether to keep empty substrings.
 * @return The split substrings.
 */
అంతఃవరుస Inlined_సదిశ<std::string_view> విభజించు_String(std::string_view string_గమ్యం_విభజించు, std::string_view delimiter,
                                                   bool keep_ఖాళీలు = false) {
  ORT_ENFORCE(!delimiter.ఖాళీలు(), "delimiter must not be empty");
  Inlined_సదిశ<std::string_view> ఫలితం{};
  std::string_view::కొలత_type segment_begin_pos = 0;
  అయితే (segment_begin_pos != std::string_view::npos) {
    స్థిరరాశి std::string_view::కొలత_type segment_చివర_pos = string_గమ్యం_విభజించు.కనుగొను(delimiter, segment_begin_pos);
    స్థిరరాశి bool is_segment_ఖాళీలు = segment_begin_pos == segment_చివర_pos || segment_begin_pos == string_గమ్యం_విభజించు.కొలత();
    ఒకవేళ (!is_segment_ఖాళీలు || keep_ఖాళీలు) {
      ఫలితం.నెట్టు_వెనుక(string_గమ్యం_విభజించు.substr(segment_begin_pos, segment_చివర_pos - segment_begin_pos));
    }
    segment_begin_pos = (segment_చివర_pos == std::string_view::npos)
                            ? segment_చివర_pos
                            : segment_చివర_pos + delimiter.కొలత();
  }
  తిరిగి ఫలితం;
}

/**
 * Trim a string from start inplace.
 * @param s The string to trim.
 */
అంతఃవరుస శూన్య Trim_String_From_ఎడమ(std::string& s) {
  s.erase(s.begin(), std::కనుగొను_if(s.begin(), s.చివర(), [](గుర్తులేని అక్షరం ch) { తిరిగి !std::ఖాళీనా(ch); }));
}

/**
 * Trim a string from end inplace.
 * @param s The string to trim.
 */
అంతఃవరుస శూన్య Trim_String_From_కుడి(std::string& s) {
  s.erase(std::కనుగొను_if(s.rbegin(), s.rend(), [](గుర్తులేని అక్షరం ch) { తిరిగి !std::ఖాళీనా(ch); }).పునాది(), s.చివర());
}

/**
 * Trim a string from both ends.
 * @param s The string to trim.
 * @return The trimmed string.
 */
అంతఃవరుస std::string TrimString(std::string_view s) {
  std::string s_trimmed{s};
  Trim_String_From_కుడి(s_trimmed);
  Trim_String_From_ఎడమ(s_trimmed);
  తిరిగి s_trimmed;
}

/**
 * @brief A consistent way to construct the full qualified op name.
 */
అంతఃవరుస std::string Get_నిండిన_Qualified_Op_Name(స్థిరరాశి std::string& op_type, స్థిరరాశి std::string& domain) {
  తిరిగి MakeString(domain, "::", op_type);
}

/**
 * Use this simple hash to generate unique int by given string input.
 */
అంతఃవరుస uint32_t GetHashFromString(స్థిరరాశి std::string& str_విలువ) {
  uint32_t hash = 0;
  కొరకు (అక్షరం స్థిరరాశి& c : str_విలువ) {
    hash = hash * 101 + c;
  }

  తిరిగి hash;
}

/**
 * Returns a lowercase version of the input string.
 * @param str The string to lowercase.
 * @return The lowercased string.
 */
అంతఃవరుస std::string GetLowercaseString(std::string str) {
  // https://en.cppreference.com/w/cpp/string/byte/tolower
  // The behavior of tolower from <cctype> is undefined if the argument is neither representable as unsigned char
  // nor equal to EOF. To use tolower safely with a plain char (or signed char), the argument must be converted to
  // unsigned char.
  std::transform(str.begin(), str.చివర(), str.begin(), [](గుర్తులేని అక్షరం c) {
    తిరిగి static_cast<అక్షరం>(std::tolower(c));
  });
  తిరిగి str;
}

}  // namespace utils
}  // namespace onnxruntime
