// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#ಅನ್ತರ್ಭಾವ "core/common/common.h"

#ಅನ್ತರ್ಭಾವ <string>

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
ಅನ್ತಃಸ್ಥ bool utf8_bytes(unsigned ವರ್ಣ ch, size_t& len) {
  ಯದಿ ((ch & 0x80) == 0) {
    len = 1;
    ಪ್ರತ್ಯಾಗಮ true;
  }
  ಯದಿ ((ch & 0xE0) == 0xC0) {
    len = 2;
    ಪ್ರತ್ಯಾಗಮ true;
  }
  ಯದಿ ((ch & 0xF0) == 0xE0) {
    len = 3;
    ಪ್ರತ್ಯಾಗಮ true;
  }
  ಯದಿ ((ch & 0xF8) == 0xF0) {
    len = 4;
    ಪ್ರತ್ಯಾಗಮ true;
  }
  ಪ್ರತ್ಯಾಗಮ false;
}

// Computes length of the utf8 string in characters
ಅನ್ತಃಸ್ಥ bool utf8_len(ಧ್ರುವ unsigned ವರ್ಣ* s, size_t bytes, size_t& len) {
  size_t result = 0;
  ಯಾವತ್ (bytes > 0) {
    size_t char_bytes = 0;
    bool valid = utf8_bytes(*s, char_bytes);
    ಯದಿ (!valid || bytes < char_bytes) {
      ಪ್ರತ್ಯಾಗಮ false;
    }
    bytes -= char_bytes;
    s += char_bytes;
    ++result;
  }
  len = result;
  ಪ್ರತ್ಯಾಗಮ true;
}

ಅನ್ತಃಸ್ಥ bool utf8_validate(ಧ್ರುವ unsigned ವರ್ಣ* s, size_t len, size_t& utf8_chars) {
  size_t utf8_len = 0;
  size_t idx = 0;
  ಯಾವತ್ (idx < len) {
    size_t bytes = 0;
    auto ch = s[idx];
    ಯದಿ (utf8_bytes(ch, bytes)) {
      ವಿಕಲ್ಪ (bytes) {
        ಪಕ್ಷ 1:
          ಭಙ್ಗ;
        ಪಕ್ಷ 2: {
          // Reject overlong 2-byte sequences. Valid Unicode 2-byte encodings
          // start at U+0080, so lead bytes 0xC0 and 0xC1 are invalid.
          ಯದಿ (ch < 0xC2u) {
            ಪ್ರತ್ಯಾಗಮ false;
          }
          ಯದಿ (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
            ಪ್ರತ್ಯಾಗಮ false;
          }
        } ಭಙ್ಗ;  // 2
        ಪಕ್ಷ 3: {
          auto ch1 = s[idx];
          ವಿಕಲ್ಪ (ch1) {
            ಪಕ್ಷ 0xE0u:
              ಯದಿ (++idx >= len || s[idx] < 0xA0u || s[idx] > 0xBFu) {
                ಪ್ರತ್ಯಾಗಮ false;
              }
              ಭಙ್ಗ;
            ಪಕ್ಷ 0xEDu:
              ಯದಿ (++idx >= len || s[idx] < 0x80u || s[idx] > 0x9Fu) {
                ಪ್ರತ್ಯಾಗಮ false;
              }
              ಭಙ್ಗ;
            ಸ್ವಭಾವ: {
              ಯದಿ ((ch1 >= 0xE1u && ch1 <= 0xECu) ||
                  (ch1 >= 0xEEu && ch1 <= 0xEFu)) {
                ಯದಿ (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
                  ಪ್ರತ್ಯಾಗಮ false;
                }
              } ಅನ್ಯಥಾ {
                ಪ್ರತ್ಯಾಗಮ false;
              }
            } ಭಙ್ಗ;
          }
          // validate byte 3
          ಯದಿ (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
            ಪ್ರತ್ಯಾಗಮ false;
          }
        } ಭಙ್ಗ;  // 3
        ಪಕ್ಷ 4: {
          auto ch1 = s[idx];
          ವಿಕಲ್ಪ (ch1) {
            ಪಕ್ಷ 0xF0u: {
              ಯದಿ (++idx >= len || s[idx] < 0x90u || s[idx] > 0xBFu) {
                ಪ್ರತ್ಯಾಗಮ false;
              }
            } ಭಙ್ಗ;
            ಪಕ್ಷ 0xF4u: {
              ಯದಿ (++idx >= len || s[idx] < 0x80u || s[idx] > 0x8Fu) {
                ಪ್ರತ್ಯಾಗಮ false;
              }
            } ಭಙ್ಗ;
            ಸ್ವಭಾವ: {
              ಯದಿ (ch1 >= 0xF1u && ch1 <= 0xF3u) {
                ಯದಿ (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
                  ಪ್ರತ್ಯಾಗಮ false;
                }
              } ಅನ್ಯಥಾ {
                ಪ್ರತ್ಯಾಗಮ false;
              }
            } ಭಙ್ಗ;
          }
          // validate bytes 3 and 4
          size_t stop = idx + 2;
          ಯಾವತ್ (idx < stop) {
            ಯದಿ (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
              ಪ್ರತ್ಯಾಗಮ false;
            }
          }
        } ಭಙ್ಗ;  // 4
        ಸ್ವಭಾವ:
          // no chars longer than 4
          ಪ್ರತ್ಯಾಗಮ false;
      }  // switch bytes
      ++idx;
      ++utf8_len;
    } ಅನ್ಯಥಾ {
      ಪ್ರತ್ಯಾಗಮ false;
    }
  }
  // End index must match
  // the end of the last byte sequence.
  ಯದಿ (idx != len) {
    ಪ್ರತ್ಯಾಗಮ false;
  }
  utf8_chars = utf8_len;
  ಪ್ರತ್ಯಾಗಮ true;
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

static_assert(ಪರಿಮಾಣ(wchar_t) >= 4,
              "Non-Windows UTF-8/wchar_t conversion helpers require wchar_t to be at least 32 bits.");

/// Compute the number of UTF-8 bytes required to encode a wide string.
ಅನ್ತಃಸ್ಥ size_t WideToUtf8RequiredSize(ಧ್ರುವ std::wstring& wstr) {
  size_t result = 0;
  ಪ್ರತಿ (wchar_t wc : wstr) {
    char32_t cp = static_cast<char32_t>(wc);
    ಯದಿ (cp >= 0xD800 && cp <= 0xDFFF) {
      ORT_THROW("Invalid Unicode surrogate codepoint U+", std::hex, static_cast<uint32_t>(cp));
    }
    ಯದಿ (cp <= 0x7F) {
      result += 1;
    } ಅನ್ಯಥಾ ಯದಿ (cp <= 0x7FF) {
      result += 2;
    } ಅನ್ಯಥಾ ಯದಿ (cp <= 0xFFFF) {
      result += 3;
    } ಅನ್ಯಥಾ ಯದಿ (cp <= 0x10FFFF) {
      result += 4;
    } ಅನ್ಯಥಾ {
      ORT_THROW("Invalid Unicode codepoint U+", std::hex, static_cast<uint32_t>(cp));
    }
  }
  ಪ್ರತ್ಯಾಗಮ result;
}

/// Convert a wide string to UTF-8, writing into a pre-allocated std::string.
/// The string is resized to the actual number of bytes written.
ಅನ್ತಃಸ್ಥ Status WideToUtf8(ಧ್ರುವ std::wstring& wstr, std::string& str) {
  ಯದಿ (wstr.empty()) {
    str.clear();
    ಪ್ರತ್ಯಾಗಮ Status::OK();
  }

  ವರ್ಣ* dest = str.data();
  ವರ್ಣ* dest_end = dest + str.size();

  ಪ್ರತಿ (wchar_t wc : wstr) {
    char32_t cp = static_cast<char32_t>(wc);
    ಯದಿ (cp >= 0xD800 && cp <= 0xDFFF) {
      ಪ್ರತ್ಯಾಗಮ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL,
                             "Invalid Unicode surrogate codepoint during UTF-8 conversion");
    }
    ಯದಿ (cp <= 0x7F) {
      ಧ್ರುವ size_t remaining = static_cast<size_t>(dest_end - dest);
      ಯದಿ (remaining < 1) {
        ಪ್ರತ್ಯಾಗಮ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<ವರ್ಣ>(cp);
    } ಅನ್ಯಥಾ ಯದಿ (cp <= 0x7FF) {
      ಧ್ರುವ size_t remaining = static_cast<size_t>(dest_end - dest);
      ಯದಿ (remaining < 2) {
        ಪ್ರತ್ಯಾಗಮ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<ವರ್ಣ>(0xC0 | (cp >> 6));
      *dest++ = static_cast<ವರ್ಣ>(0x80 | (cp & 0x3F));
    } ಅನ್ಯಥಾ ಯದಿ (cp <= 0xFFFF) {
      ಧ್ರುವ size_t remaining = static_cast<size_t>(dest_end - dest);
      ಯದಿ (remaining < 3) {
        ಪ್ರತ್ಯಾಗಮ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<ವರ್ಣ>(0xE0 | (cp >> 12));
      *dest++ = static_cast<ವರ್ಣ>(0x80 | ((cp >> 6) & 0x3F));
      *dest++ = static_cast<ವರ್ಣ>(0x80 | (cp & 0x3F));
    } ಅನ್ಯಥಾ ಯದಿ (cp <= 0x10FFFF) {
      ಧ್ರುವ size_t remaining = static_cast<size_t>(dest_end - dest);
      ಯದಿ (remaining < 4) {
        ಪ್ರತ್ಯಾಗಮ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<ವರ್ಣ>(0xF0 | (cp >> 18));
      *dest++ = static_cast<ವರ್ಣ>(0x80 | ((cp >> 12) & 0x3F));
      *dest++ = static_cast<ವರ್ಣ>(0x80 | ((cp >> 6) & 0x3F));
      *dest++ = static_cast<ವರ್ಣ>(0x80 | (cp & 0x3F));
    } ಅನ್ಯಥಾ {
      ಪ್ರತ್ಯಾಗಮ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL,
                             "Invalid Unicode codepoint during UTF-8 conversion");
    }
  }

  str.resize(static_cast<size_t>(dest - str.data()));
  ಪ್ರತ್ಯಾಗಮ Status::OK();
}

/// Convert a UTF-8 string to a wide string, writing into a pre-allocated std::wstring.
/// The wstring is resized to the actual number of characters written.
/// Validates continuation bytes and rejects overlong encodings and surrogates.
ಅನ್ತಃಸ್ಥ Status Utf8ToWide(ಧ್ರುವ std::string& str, std::wstring& wstr) {
  ಯದಿ (str.empty()) {
    wstr.clear();
    ಪ್ರತ್ಯಾಗಮ Status::OK();
  }

  ಯದಿ (wstr.size() < str.size()) {
    wstr.resize(str.size());
  }

  ಧ್ರುವ auto* src = reinterpret_cast<ಧ್ರುವ unsigned ವರ್ಣ*>(str.data());
  ಧ್ರುವ auto* src_end = src + str.size();
  wchar_t* dest = wstr.data();

  ಯಾವತ್ (src < src_end) {
    char32_t cp = 0;
    size_t byte_len = 0;

    ಯದಿ ((*src & 0x80) == 0) {
      cp = *src;
      byte_len = 1;
    } ಅನ್ಯಥಾ ಯದಿ ((*src & 0xE0) == 0xC0) {
      byte_len = 2;
      ಯದಿ (static_cast<size_t>(src_end - src) < 2) {
        ಪ್ರತ್ಯಾಗಮ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      ಯದಿ ((src[1] & 0xC0) != 0x80) {
        ಪ್ರತ್ಯಾಗಮ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x1F) << 6) |
           static_cast<char32_t>(src[1] & 0x3F);
      // Reject overlong encoding (must be >= 0x80 for 2-byte)
      ಯದಿ (cp < 0x80) {
        ಪ್ರತ್ಯಾಗಮ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
    } ಅನ್ಯಥಾ ಯದಿ ((*src & 0xF0) == 0xE0) {
      byte_len = 3;
      ಯದಿ (static_cast<size_t>(src_end - src) < 3) {
        ಪ್ರತ್ಯಾಗಮ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      ಯದಿ ((src[1] & 0xC0) != 0x80 || (src[2] & 0xC0) != 0x80) {
        ಪ್ರತ್ಯಾಗಮ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x0F) << 12) |
           (static_cast<char32_t>(src[1] & 0x3F) << 6) |
           static_cast<char32_t>(src[2] & 0x3F);
      // Reject overlong encoding (must be >= 0x800 for 3-byte)
      ಯದಿ (cp < 0x800) {
        ಪ್ರತ್ಯಾಗಮ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
      // Reject UTF-16 surrogates (U+D800..U+DFFF)
      ಯದಿ (cp >= 0xD800 && cp <= 0xDFFF) {
        ಪ್ರತ್ಯಾಗಮ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8: surrogate codepoint");
      }
    } ಅನ್ಯಥಾ ಯದಿ ((*src & 0xF8) == 0xF0) {
      byte_len = 4;
      ಯದಿ (static_cast<size_t>(src_end - src) < 4) {
        ಪ್ರತ್ಯಾಗಮ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      ಯದಿ ((src[1] & 0xC0) != 0x80 || (src[2] & 0xC0) != 0x80 || (src[3] & 0xC0) != 0x80) {
        ಪ್ರತ್ಯಾಗಮ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x07) << 18) |
           (static_cast<char32_t>(src[1] & 0x3F) << 12) |
           (static_cast<char32_t>(src[2] & 0x3F) << 6) |
           static_cast<char32_t>(src[3] & 0x3F);
      // Reject overlong encoding (must be >= 0x10000 for 4-byte)
      ಯದಿ (cp < 0x10000) {
        ಪ್ರತ್ಯಾಗಮ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
      // Reject codepoints beyond Unicode range
      ಯದಿ (cp > 0x10FFFF) {
        ಪ್ರತ್ಯಾಗಮ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8: codepoint beyond U+10FFFF");
      }
    } ಅನ್ಯಥಾ {
      ಪ್ರತ್ಯಾಗಮ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8 lead byte");
    }

    *dest++ = static_cast<wchar_t>(cp);
    src += byte_len;
  }

  wstr.resize(static_cast<size_t>(dest - wstr.data()));
  ಪ್ರತ್ಯಾಗಮ Status::OK();
}

/// Convenience: convert UTF-8 string to wstring (throws on error).
ಅನ್ತಃಸ್ಥ std::wstring Utf8ToWideString(ಧ್ರುವ std::string& s) {
  // UTF-8 byte count is an upper bound on wchar_t count
  std::wstring result;
  result.resize(s.size());
  ORT_THROW_IF_ERROR(Utf8ToWide(s, result));
  ಪ್ರತ್ಯಾಗಮ result;
}

#endif  // !_WIN32

}  // namespace utf8_util
}  // namespace onnxruntime
