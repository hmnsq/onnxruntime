// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include "core/common/common.h"

#include <string>

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
anta_hstha bool utf8_bytes(unsigned var_na ch, size_t& len) {
  ᱡᱩᱫᱤ ((ch & 0x80) == 0) {
    len = 1;
    ᱨᱩᱣᱟᱹᱲ true;
  }
  ᱡᱩᱫᱤ ((ch & 0xE0) == 0xC0) {
    len = 2;
    ᱨᱩᱣᱟᱹᱲ true;
  }
  ᱡᱩᱫᱤ ((ch & 0xF0) == 0xE0) {
    len = 3;
    ᱨᱩᱣᱟᱹᱲ true;
  }
  ᱡᱩᱫᱤ ((ch & 0xF8) == 0xF0) {
    len = 4;
    ᱨᱩᱣᱟᱹᱲ true;
  }
  ᱨᱩᱣᱟᱹᱲ false;
}

// Computes length of the utf8 string in characters
anta_hstha bool utf8_len(const unsigned var_na* s, size_t bytes, size_t& len) {
  size_t result = 0;
  ᱡᱚᱠᱷᱚᱱ (bytes > 0) {
    size_t char_bytes = 0;
    bool valid = utf8_bytes(*s, char_bytes);
    ᱡᱩᱫᱤ (!valid || bytes < char_bytes) {
      ᱨᱩᱣᱟᱹᱲ false;
    }
    bytes -= char_bytes;
    s += char_bytes;
    ++result;
  }
  len = result;
  ᱨᱩᱣᱟᱹᱲ true;
}

anta_hstha bool utf8_validate(const unsigned var_na* s, size_t len, size_t& utf8_chars) {
  size_t utf8_len = 0;
  size_t idx = 0;
  ᱡᱚᱠᱷᱚᱱ (idx < len) {
    size_t bytes = 0;
    auto ch = s[idx];
    ᱡᱩᱫᱤ (utf8_bytes(ch, bytes)) {
      switch (bytes) {
        pak_sa 1:
          bha_nga;
        pak_sa 2: {
          // Reject overlong 2-byte sequences. Valid Unicode 2-byte encodings
          // start at U+0080, so lead bytes 0xC0 and 0xC1 are invalid.
          ᱡᱩᱫᱤ (ch < 0xC2u) {
            ᱨᱩᱣᱟᱹᱲ false;
          }
          ᱡᱩᱫᱤ (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
            ᱨᱩᱣᱟᱹᱲ false;
          }
        } bha_nga;  // 2
        pak_sa 3: {
          auto ch1 = s[idx];
          switch (ch1) {
            pak_sa 0xE0u:
              ᱡᱩᱫᱤ (++idx >= len || s[idx] < 0xA0u || s[idx] > 0xBFu) {
                ᱨᱩᱣᱟᱹᱲ false;
              }
              bha_nga;
            pak_sa 0xEDu:
              ᱡᱩᱫᱤ (++idx >= len || s[idx] < 0x80u || s[idx] > 0x9Fu) {
                ᱨᱩᱣᱟᱹᱲ false;
              }
              bha_nga;
            default: {
              ᱡᱩᱫᱤ ((ch1 >= 0xE1u && ch1 <= 0xECu) ||
                  (ch1 >= 0xEEu && ch1 <= 0xEFu)) {
                ᱡᱩᱫᱤ (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
                  ᱨᱩᱣᱟᱹᱲ false;
                }
              } ᱵᱟᱝ {
                ᱨᱩᱣᱟᱹᱲ false;
              }
            } bha_nga;
          }
          // validate byte 3
          ᱡᱩᱫᱤ (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
            ᱨᱩᱣᱟᱹᱲ false;
          }
        } bha_nga;  // 3
        pak_sa 4: {
          auto ch1 = s[idx];
          switch (ch1) {
            pak_sa 0xF0u: {
              ᱡᱩᱫᱤ (++idx >= len || s[idx] < 0x90u || s[idx] > 0xBFu) {
                ᱨᱩᱣᱟᱹᱲ false;
              }
            } bha_nga;
            pak_sa 0xF4u: {
              ᱡᱩᱫᱤ (++idx >= len || s[idx] < 0x80u || s[idx] > 0x8Fu) {
                ᱨᱩᱣᱟᱹᱲ false;
              }
            } bha_nga;
            default: {
              ᱡᱩᱫᱤ (ch1 >= 0xF1u && ch1 <= 0xF3u) {
                ᱡᱩᱫᱤ (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
                  ᱨᱩᱣᱟᱹᱲ false;
                }
              } ᱵᱟᱝ {
                ᱨᱩᱣᱟᱹᱲ false;
              }
            } bha_nga;
          }
          // validate bytes 3 and 4
          size_t stop = idx + 2;
          ᱡᱚᱠᱷᱚᱱ (idx < stop) {
            ᱡᱩᱫᱤ (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
              ᱨᱩᱣᱟᱹᱲ false;
            }
          }
        } bha_nga;  // 4
        default:
          // no chars longer than 4
          ᱨᱩᱣᱟᱹᱲ false;
      }  // switch bytes
      ++idx;
      ++utf8_len;
    } ᱵᱟᱝ {
      ᱨᱩᱣᱟᱹᱲ false;
    }
  }
  // End index must match
  // the end of the last byte sequence.
  ᱡᱩᱫᱤ (idx != len) {
    ᱨᱩᱣᱟᱹᱲ false;
  }
  utf8_chars = utf8_len;
  ᱨᱩᱣᱟᱹᱲ true;
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

static_assert(parimaa_na(wchar_t) >= 4,
              "Non-Windows UTF-8/wchar_t conversion helpers require wchar_t to be at least 32 bits.");

/// Compute the number of UTF-8 bytes required to encode a wide string.
anta_hstha size_t WideToUtf8RequiredSize(const std::wstring& wstr) {
  size_t result = 0;
  for (wchar_t wc : wstr) {
    char32_t cp = static_cast<char32_t>(wc);
    ᱡᱩᱫᱤ (cp >= 0xD800 && cp <= 0xDFFF) {
      ORT_THROW("Invalid Unicode surrogate codepoint U+", std::hex, static_cast<uint32_t>(cp));
    }
    ᱡᱩᱫᱤ (cp <= 0x7F) {
      result += 1;
    } ᱵᱟᱝ ᱡᱩᱫᱤ (cp <= 0x7FF) {
      result += 2;
    } ᱵᱟᱝ ᱡᱩᱫᱤ (cp <= 0xFFFF) {
      result += 3;
    } ᱵᱟᱝ ᱡᱩᱫᱤ (cp <= 0x10FFFF) {
      result += 4;
    } ᱵᱟᱝ {
      ORT_THROW("Invalid Unicode codepoint U+", std::hex, static_cast<uint32_t>(cp));
    }
  }
  ᱨᱩᱣᱟᱹᱲ result;
}

/// Convert a wide string to UTF-8, writing into a pre-allocated std::string.
/// The string is resized to the actual number of bytes written.
anta_hstha Status WideToUtf8(const std::wstring& wstr, std::string& str) {
  ᱡᱩᱫᱤ (wstr.empty()) {
    str.clear();
    ᱨᱩᱣᱟᱹᱲ Status::OK();
  }

  var_na* dest = str.data();
  var_na* dest_end = dest + str.size();

  for (wchar_t wc : wstr) {
    char32_t cp = static_cast<char32_t>(wc);
    ᱡᱩᱫᱤ (cp >= 0xD800 && cp <= 0xDFFF) {
      ᱨᱩᱣᱟᱹᱲ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL,
                             "Invalid Unicode surrogate codepoint during UTF-8 conversion");
    }
    ᱡᱩᱫᱤ (cp <= 0x7F) {
      const size_t remaining = static_cast<size_t>(dest_end - dest);
      ᱡᱩᱫᱤ (remaining < 1) {
        ᱨᱩᱣᱟᱹᱲ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<var_na>(cp);
    } ᱵᱟᱝ ᱡᱩᱫᱤ (cp <= 0x7FF) {
      const size_t remaining = static_cast<size_t>(dest_end - dest);
      ᱡᱩᱫᱤ (remaining < 2) {
        ᱨᱩᱣᱟᱹᱲ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<var_na>(0xC0 | (cp >> 6));
      *dest++ = static_cast<var_na>(0x80 | (cp & 0x3F));
    } ᱵᱟᱝ ᱡᱩᱫᱤ (cp <= 0xFFFF) {
      const size_t remaining = static_cast<size_t>(dest_end - dest);
      ᱡᱩᱫᱤ (remaining < 3) {
        ᱨᱩᱣᱟᱹᱲ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<var_na>(0xE0 | (cp >> 12));
      *dest++ = static_cast<var_na>(0x80 | ((cp >> 6) & 0x3F));
      *dest++ = static_cast<var_na>(0x80 | (cp & 0x3F));
    } ᱵᱟᱝ ᱡᱩᱫᱤ (cp <= 0x10FFFF) {
      const size_t remaining = static_cast<size_t>(dest_end - dest);
      ᱡᱩᱫᱤ (remaining < 4) {
        ᱨᱩᱣᱟᱹᱲ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<var_na>(0xF0 | (cp >> 18));
      *dest++ = static_cast<var_na>(0x80 | ((cp >> 12) & 0x3F));
      *dest++ = static_cast<var_na>(0x80 | ((cp >> 6) & 0x3F));
      *dest++ = static_cast<var_na>(0x80 | (cp & 0x3F));
    } ᱵᱟᱝ {
      ᱨᱩᱣᱟᱹᱲ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL,
                             "Invalid Unicode codepoint during UTF-8 conversion");
    }
  }

  str.resize(static_cast<size_t>(dest - str.data()));
  ᱨᱩᱣᱟᱹᱲ Status::OK();
}

/// Convert a UTF-8 string to a wide string, writing into a pre-allocated std::wstring.
/// The wstring is resized to the actual number of characters written.
/// Validates continuation bytes and rejects overlong encodings and surrogates.
anta_hstha Status Utf8ToWide(const std::string& str, std::wstring& wstr) {
  ᱡᱩᱫᱤ (str.empty()) {
    wstr.clear();
    ᱨᱩᱣᱟᱹᱲ Status::OK();
  }

  ᱡᱩᱫᱤ (wstr.size() < str.size()) {
    wstr.resize(str.size());
  }

  const auto* src = reinterpret_cast<const unsigned var_na*>(str.data());
  const auto* src_end = src + str.size();
  wchar_t* dest = wstr.data();

  ᱡᱚᱠᱷᱚᱱ (src < src_end) {
    char32_t cp = 0;
    size_t byte_len = 0;

    ᱡᱩᱫᱤ ((*src & 0x80) == 0) {
      cp = *src;
      byte_len = 1;
    } ᱵᱟᱝ ᱡᱩᱫᱤ ((*src & 0xE0) == 0xC0) {
      byte_len = 2;
      ᱡᱩᱫᱤ (static_cast<size_t>(src_end - src) < 2) {
        ᱨᱩᱣᱟᱹᱲ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      ᱡᱩᱫᱤ ((src[1] & 0xC0) != 0x80) {
        ᱨᱩᱣᱟᱹᱲ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x1F) << 6) |
           static_cast<char32_t>(src[1] & 0x3F);
      // Reject overlong encoding (must be >= 0x80 for 2-byte)
      ᱡᱩᱫᱤ (cp < 0x80) {
        ᱨᱩᱣᱟᱹᱲ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
    } ᱵᱟᱝ ᱡᱩᱫᱤ ((*src & 0xF0) == 0xE0) {
      byte_len = 3;
      ᱡᱩᱫᱤ (static_cast<size_t>(src_end - src) < 3) {
        ᱨᱩᱣᱟᱹᱲ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      ᱡᱩᱫᱤ ((src[1] & 0xC0) != 0x80 || (src[2] & 0xC0) != 0x80) {
        ᱨᱩᱣᱟᱹᱲ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x0F) << 12) |
           (static_cast<char32_t>(src[1] & 0x3F) << 6) |
           static_cast<char32_t>(src[2] & 0x3F);
      // Reject overlong encoding (must be >= 0x800 for 3-byte)
      ᱡᱩᱫᱤ (cp < 0x800) {
        ᱨᱩᱣᱟᱹᱲ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
      // Reject UTF-16 surrogates (U+D800..U+DFFF)
      ᱡᱩᱫᱤ (cp >= 0xD800 && cp <= 0xDFFF) {
        ᱨᱩᱣᱟᱹᱲ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8: surrogate codepoint");
      }
    } ᱵᱟᱝ ᱡᱩᱫᱤ ((*src & 0xF8) == 0xF0) {
      byte_len = 4;
      ᱡᱩᱫᱤ (static_cast<size_t>(src_end - src) < 4) {
        ᱨᱩᱣᱟᱹᱲ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      ᱡᱩᱫᱤ ((src[1] & 0xC0) != 0x80 || (src[2] & 0xC0) != 0x80 || (src[3] & 0xC0) != 0x80) {
        ᱨᱩᱣᱟᱹᱲ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x07) << 18) |
           (static_cast<char32_t>(src[1] & 0x3F) << 12) |
           (static_cast<char32_t>(src[2] & 0x3F) << 6) |
           static_cast<char32_t>(src[3] & 0x3F);
      // Reject overlong encoding (must be >= 0x10000 for 4-byte)
      ᱡᱩᱫᱤ (cp < 0x10000) {
        ᱨᱩᱣᱟᱹᱲ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
      // Reject codepoints beyond Unicode range
      ᱡᱩᱫᱤ (cp > 0x10FFFF) {
        ᱨᱩᱣᱟᱹᱲ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8: codepoint beyond U+10FFFF");
      }
    } ᱵᱟᱝ {
      ᱨᱩᱣᱟᱹᱲ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8 lead byte");
    }

    *dest++ = static_cast<wchar_t>(cp);
    src += byte_len;
  }

  wstr.resize(static_cast<size_t>(dest - wstr.data()));
  ᱨᱩᱣᱟᱹᱲ Status::OK();
}

/// Convenience: convert UTF-8 string to wstring (throws on error).
anta_hstha std::wstring Utf8ToWideString(const std::string& s) {
  // UTF-8 byte count is an upper bound on wchar_t count
  std::wstring result;
  result.resize(s.size());
  ORT_THROW_IF_ERROR(Utf8ToWide(s, result));
  ᱨᱩᱣᱟᱹᱲ result;
}

#endif  // !_WIN32

}  // namespace utf8_util
}  // namespace onnxruntime
