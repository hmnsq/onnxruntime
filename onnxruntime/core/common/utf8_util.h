// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#অন্তর্ভাব "core/common/common.h"

#অন্তর্ভাব <string>

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
অন্তঃরেখ bool utf8_bytes(unsigned অক্ষর ch, আকার_t& len) {
  যদি ((ch & 0x80) == 0) {
    len = 1;
    ফেরত true;
  }
  যদি ((ch & 0xE0) == 0xC0) {
    len = 2;
    ফেরত true;
  }
  যদি ((ch & 0xF0) == 0xE0) {
    len = 3;
    ফেরত true;
  }
  যদি ((ch & 0xF8) == 0xF0) {
    len = 4;
    ফেরত true;
  }
  ফেরত false;
}

// Computes length of the utf8 string in characters
অন্তঃরেখ bool utf8_len(ধ্রুবক unsigned অক্ষর* s, আকার_t bytes, আকার_t& len) {
  আকার_t ফলাফল = 0;
  যখন (bytes > 0) {
    আকার_t char_bytes = 0;
    bool valid = utf8_bytes(*s, char_bytes);
    যদি (!valid || bytes < char_bytes) {
      ফেরত false;
    }
    bytes -= char_bytes;
    s += char_bytes;
    ++ফলাফল;
  }
  len = ফলাফল;
  ফেরত true;
}

অন্তঃরেখ bool utf8_validate(ধ্রুবক unsigned অক্ষর* s, আকার_t len, আকার_t& utf8_chars) {
  আকার_t utf8_len = 0;
  আকার_t idx = 0;
  যখন (idx < len) {
    আকার_t bytes = 0;
    auto ch = s[idx];
    যদি (utf8_bytes(ch, bytes)) {
      নির্বাচন (bytes) {
        ক্ষেত্র 1:
          বিরতি;
        ক্ষেত্র 2: {
          // Reject overlong 2-byte sequences. Valid Unicode 2-byte encodings
          // start at U+0080, so lead bytes 0xC0 and 0xC1 are invalid.
          যদি (ch < 0xC2u) {
            ফেরত false;
          }
          যদি (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
            ফেরত false;
          }
        } বিরতি;  // 2
        ক্ষেত্র 3: {
          auto ch1 = s[idx];
          নির্বাচন (ch1) {
            ক্ষেত্র 0xE0u:
              যদি (++idx >= len || s[idx] < 0xA0u || s[idx] > 0xBFu) {
                ফেরত false;
              }
              বিরতি;
            ক্ষেত্র 0xEDu:
              যদি (++idx >= len || s[idx] < 0x80u || s[idx] > 0x9Fu) {
                ফেরত false;
              }
              বিরতি;
            পূর্বনির্ধারিত: {
              যদি ((ch1 >= 0xE1u && ch1 <= 0xECu) ||
                  (ch1 >= 0xEEu && ch1 <= 0xEFu)) {
                যদি (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
                  ফেরত false;
                }
              } অন্যথা {
                ফেরত false;
              }
            } বিরতি;
          }
          // validate byte 3
          যদি (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
            ফেরত false;
          }
        } বিরতি;  // 3
        ক্ষেত্র 4: {
          auto ch1 = s[idx];
          নির্বাচন (ch1) {
            ক্ষেত্র 0xF0u: {
              যদি (++idx >= len || s[idx] < 0x90u || s[idx] > 0xBFu) {
                ফেরত false;
              }
            } বিরতি;
            ক্ষেত্র 0xF4u: {
              যদি (++idx >= len || s[idx] < 0x80u || s[idx] > 0x8Fu) {
                ফেরত false;
              }
            } বিরতি;
            পূর্বনির্ধারিত: {
              যদি (ch1 >= 0xF1u && ch1 <= 0xF3u) {
                যদি (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
                  ফেরত false;
                }
              } অন্যথা {
                ফেরত false;
              }
            } বিরতি;
          }
          // validate bytes 3 and 4
          আকার_t stop = idx + 2;
          যখন (idx < stop) {
            যদি (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
              ফেরত false;
            }
          }
        } বিরতি;  // 4
        পূর্বনির্ধারিত:
          // no chars longer than 4
          ফেরত false;
      }  // switch bytes
      ++idx;
      ++utf8_len;
    } অন্যথা {
      ফেরত false;
    }
  }
  // End index must match
  // the end of the last byte sequence.
  যদি (idx != len) {
    ফেরত false;
  }
  utf8_chars = utf8_len;
  ফেরত true;
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
#ifndef _WIN32

static_assert(আকার(wchar_t) >= 4,
              "Non-Windows UTF-8/wchar_t conversion helpers require wchar_t to be at least 32 bits.");

/// Compute the number of UTF-8 bytes required to encode a wide string.
অন্তঃরেখ আকার_t Wide_To_Utf8_Required_আকার(ধ্রুবক std::wstring& wstr) {
  আকার_t ফলাফল = 0;
  জন্য (wchar_t wc : wstr) {
    char32_t cp = static_cast<char32_t>(wc);
    যদি (cp >= 0xD800 && cp <= 0xDFFF) {
      ORT_THROW("Invalid Unicode surrogate codepoint U+", std::hex, static_cast<uint32_t>(cp));
    }
    যদি (cp <= 0x7F) {
      ফলাফল += 1;
    } অন্যথা যদি (cp <= 0x7FF) {
      ফলাফল += 2;
    } অন্যথা যদি (cp <= 0xFFFF) {
      ফলাফল += 3;
    } অন্যথা যদি (cp <= 0x10FFFF) {
      ফলাফল += 4;
    } অন্যথা {
      ORT_THROW("Invalid Unicode codepoint U+", std::hex, static_cast<uint32_t>(cp));
    }
  }
  ফেরত ফলাফল;
}

/// Convert a wide string to UTF-8, writing into a pre-allocated std::string.
/// The string is resized to the actual number of bytes written.
অন্তঃরেখ Status WideToUtf8(ধ্রুবক std::wstring& wstr, std::string& str) {
  যদি (wstr.empty()) {
    str.clear();
    ফেরত Status::OK();
  }

  অক্ষর* dest = str.তথ্য();
  অক্ষর* dest_end = dest + str.আকার_2();

  জন্য (wchar_t wc : wstr) {
    char32_t cp = static_cast<char32_t>(wc);
    যদি (cp >= 0xD800 && cp <= 0xDFFF) {
      ফেরত ORT_MAKE_STATUS(ONNXRUNTIME, FAIL,
                             "Invalid Unicode surrogate codepoint during UTF-8 conversion");
    }
    যদি (cp <= 0x7F) {
      ধ্রুবক আকার_t remaining = static_cast<আকার_t>(dest_end - dest);
      যদি (remaining < 1) {
        ফেরত ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<অক্ষর>(cp);
    } অন্যথা যদি (cp <= 0x7FF) {
      ধ্রুবক আকার_t remaining = static_cast<আকার_t>(dest_end - dest);
      যদি (remaining < 2) {
        ফেরত ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<অক্ষর>(0xC0 | (cp >> 6));
      *dest++ = static_cast<অক্ষর>(0x80 | (cp & 0x3F));
    } অন্যথা যদি (cp <= 0xFFFF) {
      ধ্রুবক আকার_t remaining = static_cast<আকার_t>(dest_end - dest);
      যদি (remaining < 3) {
        ফেরত ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<অক্ষর>(0xE0 | (cp >> 12));
      *dest++ = static_cast<অক্ষর>(0x80 | ((cp >> 6) & 0x3F));
      *dest++ = static_cast<অক্ষর>(0x80 | (cp & 0x3F));
    } অন্যথা যদি (cp <= 0x10FFFF) {
      ধ্রুবক আকার_t remaining = static_cast<আকার_t>(dest_end - dest);
      যদি (remaining < 4) {
        ফেরত ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<অক্ষর>(0xF0 | (cp >> 18));
      *dest++ = static_cast<অক্ষর>(0x80 | ((cp >> 12) & 0x3F));
      *dest++ = static_cast<অক্ষর>(0x80 | ((cp >> 6) & 0x3F));
      *dest++ = static_cast<অক্ষর>(0x80 | (cp & 0x3F));
    } অন্যথা {
      ফেরত ORT_MAKE_STATUS(ONNXRUNTIME, FAIL,
                             "Invalid Unicode codepoint during UTF-8 conversion");
    }
  }

  str.resize(static_cast<আকার_t>(dest - str.তথ্য()));
  ফেরত Status::OK();
}

/// Convert a UTF-8 string to a wide string, writing into a pre-allocated std::wstring.
/// The wstring is resized to the actual number of characters written.
/// Validates continuation bytes and rejects overlong encodings and surrogates.
অন্তঃরেখ Status Utf8ToWide(ধ্রুবক std::string& str, std::wstring& wstr) {
  যদি (str.empty()) {
    wstr.clear();
    ফেরত Status::OK();
  }

  যদি (wstr.আকার_2() < str.আকার_2()) {
    wstr.resize(str.আকার_2());
  }

  ধ্রুবক auto* src = reinterpret_cast<ধ্রুবক unsigned অক্ষর*>(str.তথ্য());
  ধ্রুবক auto* src_end = src + str.আকার_2();
  wchar_t* dest = wstr.তথ্য();

  যখন (src < src_end) {
    char32_t cp = 0;
    আকার_t byte_len = 0;

    যদি ((*src & 0x80) == 0) {
      cp = *src;
      byte_len = 1;
    } অন্যথা যদি ((*src & 0xE0) == 0xC0) {
      byte_len = 2;
      যদি (static_cast<আকার_t>(src_end - src) < 2) {
        ফেরত ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      যদি ((src[1] & 0xC0) != 0x80) {
        ফেরত ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x1F) << 6) |
           static_cast<char32_t>(src[1] & 0x3F);
      // Reject overlong encoding (must be >= 0x80 for 2-byte)
      যদি (cp < 0x80) {
        ফেরত ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
    } অন্যথা যদি ((*src & 0xF0) == 0xE0) {
      byte_len = 3;
      যদি (static_cast<আকার_t>(src_end - src) < 3) {
        ফেরত ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      যদি ((src[1] & 0xC0) != 0x80 || (src[2] & 0xC0) != 0x80) {
        ফেরত ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x0F) << 12) |
           (static_cast<char32_t>(src[1] & 0x3F) << 6) |
           static_cast<char32_t>(src[2] & 0x3F);
      // Reject overlong encoding (must be >= 0x800 for 3-byte)
      যদি (cp < 0x800) {
        ফেরত ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
      // Reject UTF-16 surrogates (U+D800..U+DFFF)
      যদি (cp >= 0xD800 && cp <= 0xDFFF) {
        ফেরত ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8: surrogate codepoint");
      }
    } অন্যথা যদি ((*src & 0xF8) == 0xF0) {
      byte_len = 4;
      যদি (static_cast<আকার_t>(src_end - src) < 4) {
        ফেরত ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      যদি ((src[1] & 0xC0) != 0x80 || (src[2] & 0xC0) != 0x80 || (src[3] & 0xC0) != 0x80) {
        ফেরত ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x07) << 18) |
           (static_cast<char32_t>(src[1] & 0x3F) << 12) |
           (static_cast<char32_t>(src[2] & 0x3F) << 6) |
           static_cast<char32_t>(src[3] & 0x3F);
      // Reject overlong encoding (must be >= 0x10000 for 4-byte)
      যদি (cp < 0x10000) {
        ফেরত ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
      // Reject codepoints beyond Unicode range
      যদি (cp > 0x10FFFF) {
        ফেরত ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8: codepoint beyond U+10FFFF");
      }
    } অন্যথা {
      ফেরত ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8 lead byte");
    }

    *dest++ = static_cast<wchar_t>(cp);
    src += byte_len;
  }

  wstr.resize(static_cast<আকার_t>(dest - wstr.তথ্য()));
  ফেরত Status::OK();
}

/// Convenience: convert UTF-8 string to wstring (throws on error).
অন্তঃরেখ std::wstring Utf8ToWideString(ধ্রুবক std::string& s) {
  // UTF-8 byte count is an upper bound on wchar_t count
  std::wstring ফলাফল;
  ফলাফল.resize(s.আকার_2());
  ORT_THROW_IF_ERROR(Utf8ToWide(s, ফলাফল));
  ফেরত ফলাফল;
}

#endif  // !_WIN32

}  // namespace utf8_util
}  // namespace onnxruntime
