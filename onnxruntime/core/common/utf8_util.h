// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#సూచన once

#అన్తర్భావ "core/common/common.h"

#అన్తర్భావ <string>

namespace onnxruntime {
namespace utf8_util {

/// <summary>
/// Classifies a UTF-8 lead byte by encoded length.
/// This is a structural prefix check only; full well-formedness validation
/// is handled by utf8_validate.
/// </summary>
/// <param name="ch">lead byte candidate</param>
/// <param name="len">decoded byte length</param>
/// <returns>false if the byte does not match any 1-4 byte UTF-8 lead-byte prefix</returns>
అంతఃవరుస bool utf8_బైట్లు(గుర్తులేని అక్షరం ch, కొలత_t& len) {
  ఒకవేళ ((ch & 0x80) == 0) {
    len = 1;
    తిరిగి true;
  }
  ఒకవేళ ((ch & 0xE0) == 0xC0) {
    len = 2;
    తిరిగి true;
  }
  ఒకవేళ ((ch & 0xF0) == 0xE0) {
    len = 3;
    తిరిగి true;
  }
  ఒకవేళ ((ch & 0xF8) == 0xF0) {
    len = 4;
    తిరిగి true;
  }
  తిరిగి false;
}

// Computes length of the utf8 string in characters
అంతఃవరుస bool utf8_len(స్థిరరాశి గుర్తులేని అక్షరం* s, కొలత_t బైట్లు, కొలత_t& len) {
  కొలత_t ఫలితం = 0;
  అయితే (బైట్లు > 0) {
    కొలత_t char_బైట్లు = 0;
    bool valid = utf8_బైట్లు(*s, char_బైట్లు);
    ఒకవేళ (!valid || బైట్లు < char_బైట్లు) {
      తిరిగి false;
    }
    బైట్లు -= char_బైట్లు;
    s += char_బైట్లు;
    ++ఫలితం;
  }
  len = ఫలితం;
  తిరిగి true;
}

అంతఃవరుస bool utf8_validate(స్థిరరాశి గుర్తులేని అక్షరం* s, కొలత_t len, కొలత_t& utf8_chars) {
  కొలత_t utf8_len = 0;
  కొలత_t idx = 0;
  అయితే (idx < len) {
    కొలత_t బైట్లు = 0;
    స్వయంచాలక ch = s[idx];
    ఒకవేళ (utf8_బైట్లు(ch, బైట్లు)) {
      ఎంపిక (బైట్లు) {
        స్థితి 1:
          ఆపు;
        స్థితి 2: {
          // Reject overlong 2-byte sequences. Valid Unicode 2-byte encodings
          // start at U+0080, so lead bytes 0xC0 and 0xC1 are invalid.
          ఒకవేళ (ch < 0xC2u) {
            తిరిగి false;
          }
          ఒకవేళ (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
            తిరిగి false;
          }
        } ఆపు;  // 2
        స్థితి 3: {
          స్వయంచాలక ch1 = s[idx];
          ఎంపిక (ch1) {
            స్థితి 0xE0u:
              ఒకవేళ (++idx >= len || s[idx] < 0xA0u || s[idx] > 0xBFu) {
                తిరిగి false;
              }
              ఆపు;
            స్థితి 0xEDu:
              ఒకవేళ (++idx >= len || s[idx] < 0x80u || s[idx] > 0x9Fu) {
                తిరిగి false;
              }
              ఆపు;
            అప్రమేయ: {
              ఒకవేళ ((ch1 >= 0xE1u && ch1 <= 0xECu) ||
                  (ch1 >= 0xEEu && ch1 <= 0xEFu)) {
                ఒకవేళ (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
                  తిరిగి false;
                }
              } లేకపోతే {
                తిరిగి false;
              }
            } ఆపు;
          }
          // validate byte 3
          ఒకవేళ (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
            తిరిగి false;
          }
        } ఆపు;  // 3
        స్థితి 4: {
          స్వయంచాలక ch1 = s[idx];
          ఎంపిక (ch1) {
            స్థితి 0xF0u: {
              ఒకవేళ (++idx >= len || s[idx] < 0x90u || s[idx] > 0xBFu) {
                తిరిగి false;
              }
            } ఆపు;
            స్థితి 0xF4u: {
              ఒకవేళ (++idx >= len || s[idx] < 0x80u || s[idx] > 0x8Fu) {
                తిరిగి false;
              }
            } ఆపు;
            అప్రమేయ: {
              ఒకవేళ (ch1 >= 0xF1u && ch1 <= 0xF3u) {
                ఒకవేళ (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
                  తిరిగి false;
                }
              } లేకపోతే {
                తిరిగి false;
              }
            } ఆపు;
          }
          // validate bytes 3 and 4
          కొలత_t stop = idx + 2;
          అయితే (idx < stop) {
            ఒకవేళ (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
              తిరిగి false;
            }
          }
        } ఆపు;  // 4
        అప్రమేయ:
          // no chars longer than 4
          తిరిగి false;
      }  // switch bytes
      ++idx;
      ++utf8_len;
    } లేకపోతే {
      తిరిగి false;
    }
  }
  // End index must match
  // the end of the last byte sequence.
  ఒకవేళ (idx != len) {
    తిరిగి false;
  }
  utf8_chars = utf8_len;
  తిరిగి true;
}

// UTF-8 <-> wchar_t conversion utilities for non-Windows builds.
// These helpers operate on one wchar_t code unit per Unicode scalar value.
// They are fully Unicode-correct on platforms where wchar_t stores scalar values
// directly, which is commonly the case for 32-bit wchar_t builds such as Linux,
// macOS, and wasm.
// They do not implement UTF-16 surrogate-pair handling, so non-Windows builds
// with 16-bit wchar_t cannot represent supplementary-plane characters correctly
// via these helpers.
// On Windows, use the Win32 MultiByteToWideChar/WideCharToMultiByte APIs instead.
#నిర్వచితం_కాకపోతే _WIN32

static_assert(పరిమాణం(wchar_t) >= 4,
              "Non-Windows UTF-8/wchar_t conversion helpers require wchar_t to be at least 32 bits.");

/// Compute the number of UTF-8 bytes required to encode a wide string.
అంతఃవరుస కొలత_t Wide_గమ్యం_Utf8_Required_కొలత(స్థిరరాశి std::wstring& wstr) {
  కొలత_t ఫలితం = 0;
  కొరకు (wchar_t wc : wstr) {
    char32_t cp = static_cast<char32_t>(wc);
    ఒకవేళ (cp >= 0xD800 && cp <= 0xDFFF) {
      ORT_THROW("Invalid Unicode surrogate codepoint U+", std::hex, static_cast<uint32_t>(cp));
    }
    ఒకవేళ (cp <= 0x7F) {
      ఫలితం += 1;
    } లేకపోతే ఒకవేళ (cp <= 0x7FF) {
      ఫలితం += 2;
    } లేకపోతే ఒకవేళ (cp <= 0xFFFF) {
      ఫలితం += 3;
    } లేకపోతే ఒకవేళ (cp <= 0x10FFFF) {
      ఫలితం += 4;
    } లేకపోతే {
      ORT_THROW("Invalid Unicode codepoint U+", std::hex, static_cast<uint32_t>(cp));
    }
  }
  తిరిగి ఫలితం;
}

/// Convert a wide string to UTF-8, writing into a pre-allocated std::string.
/// The string is resized to the actual number of bytes written.
అంతఃవరుస స్థితి_3 Wide_గమ్యం_Utf8(స్థిరరాశి std::wstring& wstr, std::string& str) {
  ఒకవేళ (wstr.ఖాళీలు()) {
    str.clear();
    తిరిగి స్థితి_3::OK();
  }

  అక్షరం* dest = str.దత్తాంశం();
  అక్షరం* dest_చివర = dest + str.కొలత();

  కొరకు (wchar_t wc : wstr) {
    char32_t cp = static_cast<char32_t>(wc);
    ఒకవేళ (cp >= 0xD800 && cp <= 0xDFFF) {
      తిరిగి ORT_MAKE_స్థితి(ONNXRUNTIME, FAIL,
                             "Invalid Unicode surrogate codepoint during UTF-8 conversion");
    }
    ఒకవేళ (cp <= 0x7F) {
      స్థిరరాశి కొలత_t మిగిలిన = static_cast<కొలత_t>(dest_చివర - dest);
      ఒకవేళ (మిగిలిన < 1) {
        తిరిగి ORT_MAKE_స్థితి(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<అక్షరం>(cp);
    } లేకపోతే ఒకవేళ (cp <= 0x7FF) {
      స్థిరరాశి కొలత_t మిగిలిన = static_cast<కొలత_t>(dest_చివర - dest);
      ఒకవేళ (మిగిలిన < 2) {
        తిరిగి ORT_MAKE_స్థితి(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<అక్షరం>(0xC0 | (cp >> 6));
      *dest++ = static_cast<అక్షరం>(0x80 | (cp & 0x3F));
    } లేకపోతే ఒకవేళ (cp <= 0xFFFF) {
      స్థిరరాశి కొలత_t మిగిలిన = static_cast<కొలత_t>(dest_చివర - dest);
      ఒకవేళ (మిగిలిన < 3) {
        తిరిగి ORT_MAKE_స్థితి(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<అక్షరం>(0xE0 | (cp >> 12));
      *dest++ = static_cast<అక్షరం>(0x80 | ((cp >> 6) & 0x3F));
      *dest++ = static_cast<అక్షరం>(0x80 | (cp & 0x3F));
    } లేకపోతే ఒకవేళ (cp <= 0x10FFFF) {
      స్థిరరాశి కొలత_t మిగిలిన = static_cast<కొలత_t>(dest_చివర - dest);
      ఒకవేళ (మిగిలిన < 4) {
        తిరిగి ORT_MAKE_స్థితి(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<అక్షరం>(0xF0 | (cp >> 18));
      *dest++ = static_cast<అక్షరం>(0x80 | ((cp >> 12) & 0x3F));
      *dest++ = static_cast<అక్షరం>(0x80 | ((cp >> 6) & 0x3F));
      *dest++ = static_cast<అక్షరం>(0x80 | (cp & 0x3F));
    } లేకపోతే {
      తిరిగి ORT_MAKE_స్థితి(ONNXRUNTIME, FAIL,
                             "Invalid Unicode codepoint during UTF-8 conversion");
    }
  }

  str.resize(static_cast<కొలత_t>(dest - str.దత్తాంశం()));
  తిరిగి స్థితి_3::OK();
}

/// Convert a UTF-8 string to a wide string, writing into a pre-allocated std::wstring.
/// The wstring is resized to the actual number of characters written.
/// Validates continuation bytes and rejects overlong encodings and surrogates.
అంతఃవరుస స్థితి_3 Utf8_గమ్యం_Wide(స్థిరరాశి std::string& str, std::wstring& wstr) {
  ఒకవేళ (str.ఖాళీలు()) {
    wstr.clear();
    తిరిగి స్థితి_3::OK();
  }

  ఒకవేళ (wstr.కొలత() < str.కొలత()) {
    wstr.resize(str.కొలత());
  }

  స్థిరరాశి స్వయంచాలక* src = reinterpret_cast<స్థిరరాశి గుర్తులేని అక్షరం*>(str.దత్తాంశం());
  స్థిరరాశి స్వయంచాలక* src_చివర = src + str.కొలత();
  wchar_t* dest = wstr.దత్తాంశం();

  అయితే (src < src_చివర) {
    char32_t cp = 0;
    కొలత_t byte_len = 0;

    ఒకవేళ ((*src & 0x80) == 0) {
      cp = *src;
      byte_len = 1;
    } లేకపోతే ఒకవేళ ((*src & 0xE0) == 0xC0) {
      byte_len = 2;
      ఒకవేళ (static_cast<కొలత_t>(src_చివర - src) < 2) {
        తిరిగి ORT_MAKE_స్థితి(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      ఒకవేళ ((src[1] & 0xC0) != 0x80) {
        తిరిగి ORT_MAKE_స్థితి(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x1F) << 6) |
           static_cast<char32_t>(src[1] & 0x3F);
      // Reject overlong encoding (must be >= 0x80 for 2-byte)
      ఒకవేళ (cp < 0x80) {
        తిరిగి ORT_MAKE_స్థితి(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
    } లేకపోతే ఒకవేళ ((*src & 0xF0) == 0xE0) {
      byte_len = 3;
      ఒకవేళ (static_cast<కొలత_t>(src_చివర - src) < 3) {
        తిరిగి ORT_MAKE_స్థితి(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      ఒకవేళ ((src[1] & 0xC0) != 0x80 || (src[2] & 0xC0) != 0x80) {
        తిరిగి ORT_MAKE_స్థితి(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x0F) << 12) |
           (static_cast<char32_t>(src[1] & 0x3F) << 6) |
           static_cast<char32_t>(src[2] & 0x3F);
      // Reject overlong encoding (must be >= 0x800 for 3-byte)
      ఒకవేళ (cp < 0x800) {
        తిరిగి ORT_MAKE_స్థితి(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
      // Reject UTF-16 surrogates (U+D800..U+DFFF)
      ఒకవేళ (cp >= 0xD800 && cp <= 0xDFFF) {
        తిరిగి ORT_MAKE_స్థితి(ONNXRUNTIME, FAIL, "Invalid UTF-8: surrogate codepoint");
      }
    } లేకపోతే ఒకవేళ ((*src & 0xF8) == 0xF0) {
      byte_len = 4;
      ఒకవేళ (static_cast<కొలత_t>(src_చివర - src) < 4) {
        తిరిగి ORT_MAKE_స్థితి(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      ఒకవేళ ((src[1] & 0xC0) != 0x80 || (src[2] & 0xC0) != 0x80 || (src[3] & 0xC0) != 0x80) {
        తిరిగి ORT_MAKE_స్థితి(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x07) << 18) |
           (static_cast<char32_t>(src[1] & 0x3F) << 12) |
           (static_cast<char32_t>(src[2] & 0x3F) << 6) |
           static_cast<char32_t>(src[3] & 0x3F);
      // Reject overlong encoding (must be >= 0x10000 for 4-byte)
      ఒకవేళ (cp < 0x10000) {
        తిరిగి ORT_MAKE_స్థితి(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
      // Reject codepoints beyond Unicode range
      ఒకవేళ (cp > 0x10FFFF) {
        తిరిగి ORT_MAKE_స్థితి(ONNXRUNTIME, FAIL, "Invalid UTF-8: codepoint beyond U+10FFFF");
      }
    } లేకపోతే {
      తిరిగి ORT_MAKE_స్థితి(ONNXRUNTIME, FAIL, "Invalid UTF-8 lead byte");
    }

    *dest++ = static_cast<wchar_t>(cp);
    src += byte_len;
  }

  wstr.resize(static_cast<కొలత_t>(dest - wstr.దత్తాంశం()));
  తిరిగి స్థితి_3::OK();
}

/// Convenience: convert UTF-8 string to wstring (throws on error).
అంతఃవరుస std::wstring Utf8_గమ్యం_Wide_String(స్థిరరాశి std::string& s) {
  // UTF-8 byte count is an upper bound on wchar_t count
  std::wstring ఫలితం;
  ఫలితం.resize(s.కొలత());
  ORT_THROW_IF_ERROR(Utf8_గమ్యం_Wide(s, ఫలితం));
  తిరిగి ఫలితం;
}

#అయితే_ముగింపు  // !_WIN32

}  // namespace utf8_util
}  // namespace onnxruntime
