// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#অন্তর্ভাৱ "core/common/common.h"

#অন্তর্ভাৱ <string>

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
অন্তঃস্থ bool utf8_bytes(unsigned ৱর্ণ ch, size_t& len) {
  যদি ((ch & 0x80) == 0) {
    len = 1;
    ঘূৰাই true;
  }
  যদি ((ch & 0xE0) == 0xC0) {
    len = 2;
    ঘূৰাই true;
  }
  যদি ((ch & 0xF0) == 0xE0) {
    len = 3;
    ঘূৰাই true;
  }
  যদি ((ch & 0xF8) == 0xF0) {
    len = 4;
    ঘূৰাই true;
  }
  ঘূৰাই false;
}

// Computes length of the utf8 string in characters
অন্তঃস্থ bool utf8_len(ধ্রুৱ unsigned ৱর্ণ* s, size_t bytes, size_t& len) {
  size_t result = 0;
  যেতিয়ালৈ (bytes > 0) {
    size_t char_bytes = 0;
    bool valid = utf8_bytes(*s, char_bytes);
    যদি (!valid || bytes < char_bytes) {
      ঘূৰাই false;
    }
    bytes -= char_bytes;
    s += char_bytes;
    ++result;
  }
  len = result;
  ঘূৰাই true;
}

অন্তঃস্থ bool utf8_validate(ধ্রুৱ unsigned ৱর্ণ* s, size_t len, size_t& utf8_chars) {
  size_t utf8_len = 0;
  size_t idx = 0;
  যেতিয়ালৈ (idx < len) {
    size_t bytes = 0;
    auto ch = s[idx];
    যদি (utf8_bytes(ch, bytes)) {
      ৱিকল্প (bytes) {
        পক্ষ 1:
          ভঙ্গ;
        পক্ষ 2: {
          // Reject overlong 2-byte sequences. Valid Unicode 2-byte encodings
          // start at U+0080, so lead bytes 0xC0 and 0xC1 are invalid.
          যদি (ch < 0xC2u) {
            ঘূৰাই false;
          }
          যদি (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
            ঘূৰাই false;
          }
        } ভঙ্গ;  // 2
        পক্ষ 3: {
          auto ch1 = s[idx];
          ৱিকল্প (ch1) {
            পক্ষ 0xE0u:
              যদি (++idx >= len || s[idx] < 0xA0u || s[idx] > 0xBFu) {
                ঘূৰাই false;
              }
              ভঙ্গ;
            পক্ষ 0xEDu:
              যদি (++idx >= len || s[idx] < 0x80u || s[idx] > 0x9Fu) {
                ঘূৰাই false;
              }
              ভঙ্গ;
            স্ৱভাৱ: {
              যদি ((ch1 >= 0xE1u && ch1 <= 0xECu) ||
                  (ch1 >= 0xEEu && ch1 <= 0xEFu)) {
                যদি (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
                  ঘূৰাই false;
                }
              } অন্যথা {
                ঘূৰাই false;
              }
            } ভঙ্গ;
          }
          // validate byte 3
          যদি (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
            ঘূৰাই false;
          }
        } ভঙ্গ;  // 3
        পক্ষ 4: {
          auto ch1 = s[idx];
          ৱিকল্প (ch1) {
            পক্ষ 0xF0u: {
              যদি (++idx >= len || s[idx] < 0x90u || s[idx] > 0xBFu) {
                ঘূৰাই false;
              }
            } ভঙ্গ;
            পক্ষ 0xF4u: {
              যদি (++idx >= len || s[idx] < 0x80u || s[idx] > 0x8Fu) {
                ঘূৰাই false;
              }
            } ভঙ্গ;
            স্ৱভাৱ: {
              যদি (ch1 >= 0xF1u && ch1 <= 0xF3u) {
                যদি (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
                  ঘূৰাই false;
                }
              } অন্যথা {
                ঘূৰাই false;
              }
            } ভঙ্গ;
          }
          // validate bytes 3 and 4
          size_t stop = idx + 2;
          যেতিয়ালৈ (idx < stop) {
            যদি (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
              ঘূৰাই false;
            }
          }
        } ভঙ্গ;  // 4
        স্ৱভাৱ:
          // no chars longer than 4
          ঘূৰাই false;
      }  // switch bytes
      ++idx;
      ++utf8_len;
    } অন্যথা {
      ঘূৰাই false;
    }
  }
  // End index must match
  // the end of the last byte sequence.
  যদি (idx != len) {
    ঘূৰাই false;
  }
  utf8_chars = utf8_len;
  ঘূৰাই true;
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

static_assert(পরিমাণ(wchar_t) >= 4,
              "Non-Windows UTF-8/wchar_t conversion helpers require wchar_t to be at least 32 bits.");

/// Compute the number of UTF-8 bytes required to encode a wide string.
অন্তঃস্থ size_t WideToUtf8RequiredSize(ধ্রুৱ std::wstring& wstr) {
  size_t result = 0;
  বাবে (wchar_t wc : wstr) {
    char32_t cp = static_cast<char32_t>(wc);
    যদি (cp >= 0xD800 && cp <= 0xDFFF) {
      ORT_THROW("Invalid Unicode surrogate codepoint U+", std::hex, static_cast<uint32_t>(cp));
    }
    যদি (cp <= 0x7F) {
      result += 1;
    } অন্যথা যদি (cp <= 0x7FF) {
      result += 2;
    } অন্যথা যদি (cp <= 0xFFFF) {
      result += 3;
    } অন্যথা যদি (cp <= 0x10FFFF) {
      result += 4;
    } অন্যথা {
      ORT_THROW("Invalid Unicode codepoint U+", std::hex, static_cast<uint32_t>(cp));
    }
  }
  ঘূৰাই result;
}

/// Convert a wide string to UTF-8, writing into a pre-allocated std::string.
/// The string is resized to the actual number of bytes written.
অন্তঃস্থ Status WideToUtf8(ধ্রুৱ std::wstring& wstr, std::string& str) {
  যদি (wstr.empty()) {
    str.clear();
    ঘূৰাই Status::OK();
  }

  ৱর্ণ* dest = str.data();
  ৱর্ণ* dest_end = dest + str.size();

  বাবে (wchar_t wc : wstr) {
    char32_t cp = static_cast<char32_t>(wc);
    যদি (cp >= 0xD800 && cp <= 0xDFFF) {
      ঘূৰাই ORT_MAKE_STATUS(ONNXRUNTIME, FAIL,
                             "Invalid Unicode surrogate codepoint during UTF-8 conversion");
    }
    যদি (cp <= 0x7F) {
      ধ্রুৱ size_t remaining = static_cast<size_t>(dest_end - dest);
      যদি (remaining < 1) {
        ঘূৰাই ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<ৱর্ণ>(cp);
    } অন্যথা যদি (cp <= 0x7FF) {
      ধ্রুৱ size_t remaining = static_cast<size_t>(dest_end - dest);
      যদি (remaining < 2) {
        ঘূৰাই ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<ৱর্ণ>(0xC0 | (cp >> 6));
      *dest++ = static_cast<ৱর্ণ>(0x80 | (cp & 0x3F));
    } অন্যথা যদি (cp <= 0xFFFF) {
      ধ্রুৱ size_t remaining = static_cast<size_t>(dest_end - dest);
      যদি (remaining < 3) {
        ঘূৰাই ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<ৱর্ণ>(0xE0 | (cp >> 12));
      *dest++ = static_cast<ৱর্ণ>(0x80 | ((cp >> 6) & 0x3F));
      *dest++ = static_cast<ৱর্ণ>(0x80 | (cp & 0x3F));
    } অন্যথা যদি (cp <= 0x10FFFF) {
      ধ্রুৱ size_t remaining = static_cast<size_t>(dest_end - dest);
      যদি (remaining < 4) {
        ঘূৰাই ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<ৱর্ণ>(0xF0 | (cp >> 18));
      *dest++ = static_cast<ৱর্ণ>(0x80 | ((cp >> 12) & 0x3F));
      *dest++ = static_cast<ৱর্ণ>(0x80 | ((cp >> 6) & 0x3F));
      *dest++ = static_cast<ৱর্ণ>(0x80 | (cp & 0x3F));
    } অন্যথা {
      ঘূৰাই ORT_MAKE_STATUS(ONNXRUNTIME, FAIL,
                             "Invalid Unicode codepoint during UTF-8 conversion");
    }
  }

  str.resize(static_cast<size_t>(dest - str.data()));
  ঘূৰাই Status::OK();
}

/// Convert a UTF-8 string to a wide string, writing into a pre-allocated std::wstring.
/// The wstring is resized to the actual number of characters written.
/// Validates continuation bytes and rejects overlong encodings and surrogates.
অন্তঃস্থ Status Utf8ToWide(ধ্রুৱ std::string& str, std::wstring& wstr) {
  যদি (str.empty()) {
    wstr.clear();
    ঘূৰাই Status::OK();
  }

  যদি (wstr.size() < str.size()) {
    wstr.resize(str.size());
  }

  ধ্রুৱ auto* src = reinterpret_cast<ধ্রুৱ unsigned ৱর্ণ*>(str.data());
  ধ্রুৱ auto* src_end = src + str.size();
  wchar_t* dest = wstr.data();

  যেতিয়ালৈ (src < src_end) {
    char32_t cp = 0;
    size_t byte_len = 0;

    যদি ((*src & 0x80) == 0) {
      cp = *src;
      byte_len = 1;
    } অন্যথা যদি ((*src & 0xE0) == 0xC0) {
      byte_len = 2;
      যদি (static_cast<size_t>(src_end - src) < 2) {
        ঘূৰাই ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      যদি ((src[1] & 0xC0) != 0x80) {
        ঘূৰাই ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x1F) << 6) |
           static_cast<char32_t>(src[1] & 0x3F);
      // Reject overlong encoding (must be >= 0x80 for 2-byte)
      যদি (cp < 0x80) {
        ঘূৰাই ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
    } অন্যথা যদি ((*src & 0xF0) == 0xE0) {
      byte_len = 3;
      যদি (static_cast<size_t>(src_end - src) < 3) {
        ঘূৰাই ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      যদি ((src[1] & 0xC0) != 0x80 || (src[2] & 0xC0) != 0x80) {
        ঘূৰাই ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x0F) << 12) |
           (static_cast<char32_t>(src[1] & 0x3F) << 6) |
           static_cast<char32_t>(src[2] & 0x3F);
      // Reject overlong encoding (must be >= 0x800 for 3-byte)
      যদি (cp < 0x800) {
        ঘূৰাই ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
      // Reject UTF-16 surrogates (U+D800..U+DFFF)
      যদি (cp >= 0xD800 && cp <= 0xDFFF) {
        ঘূৰাই ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8: surrogate codepoint");
      }
    } অন্যথা যদি ((*src & 0xF8) == 0xF0) {
      byte_len = 4;
      যদি (static_cast<size_t>(src_end - src) < 4) {
        ঘূৰাই ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      যদি ((src[1] & 0xC0) != 0x80 || (src[2] & 0xC0) != 0x80 || (src[3] & 0xC0) != 0x80) {
        ঘূৰাই ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x07) << 18) |
           (static_cast<char32_t>(src[1] & 0x3F) << 12) |
           (static_cast<char32_t>(src[2] & 0x3F) << 6) |
           static_cast<char32_t>(src[3] & 0x3F);
      // Reject overlong encoding (must be >= 0x10000 for 4-byte)
      যদি (cp < 0x10000) {
        ঘূৰাই ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
      // Reject codepoints beyond Unicode range
      যদি (cp > 0x10FFFF) {
        ঘূৰাই ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8: codepoint beyond U+10FFFF");
      }
    } অন্যথা {
      ঘূৰাই ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8 lead byte");
    }

    *dest++ = static_cast<wchar_t>(cp);
    src += byte_len;
  }

  wstr.resize(static_cast<size_t>(dest - wstr.data()));
  ঘূৰাই Status::OK();
}

/// Convenience: convert UTF-8 string to wstring (throws on error).
অন্তঃস্থ std::wstring Utf8ToWideString(ধ্রুৱ std::string& s) {
  // UTF-8 byte count is an upper bound on wchar_t count
  std::wstring result;
  result.resize(s.size());
  ORT_THROW_IF_ERROR(Utf8ToWide(s, result));
  ঘূৰাই result;
}

#endif  // !_WIN32

}  // namespace utf8_util
}  // namespace onnxruntime
