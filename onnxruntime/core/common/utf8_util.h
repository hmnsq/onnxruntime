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
inline bool utf8_bytes(unsigned char ch, size_t& len) {
  如果 ((ch & 0x80) == 0) {
    len = 1;
    返回 true;
  }
  如果 ((ch & 0xE0) == 0xC0) {
    len = 2;
    返回 true;
  }
  如果 ((ch & 0xF0) == 0xE0) {
    len = 3;
    返回 true;
  }
  如果 ((ch & 0xF8) == 0xF0) {
    len = 4;
    返回 true;
  }
  返回 false;
}

// Computes length of the utf8 string in characters
inline bool utf8_len(const unsigned char* s, size_t bytes, size_t& len) {
  size_t result = 0;
  当 (bytes > 0) {
    size_t char_bytes = 0;
    bool valid = utf8_bytes(*s, char_bytes);
    如果 (!valid || bytes < char_bytes) {
      返回 false;
    }
    bytes -= char_bytes;
    s += char_bytes;
    ++result;
  }
  len = result;
  返回 true;
}

inline bool utf8_validate(const unsigned char* s, size_t len, size_t& utf8_chars) {
  size_t utf8_len = 0;
  size_t idx = 0;
  当 (idx < len) {
    size_t bytes = 0;
    auto ch = s[idx];
    如果 (utf8_bytes(ch, bytes)) {
      switch (bytes) {
        case 1:
          break;
        case 2: {
          // Reject overlong 2-byte sequences. Valid Unicode 2-byte encodings
          // start at U+0080, so lead bytes 0xC0 and 0xC1 are invalid.
          如果 (ch < 0xC2u) {
            返回 false;
          }
          如果 (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
            返回 false;
          }
        } break;  // 2
        case 3: {
          auto ch1 = s[idx];
          switch (ch1) {
            case 0xE0u:
              如果 (++idx >= len || s[idx] < 0xA0u || s[idx] > 0xBFu) {
                返回 false;
              }
              break;
            case 0xEDu:
              如果 (++idx >= len || s[idx] < 0x80u || s[idx] > 0x9Fu) {
                返回 false;
              }
              break;
            default: {
              如果 ((ch1 >= 0xE1u && ch1 <= 0xECu) ||
                  (ch1 >= 0xEEu && ch1 <= 0xEFu)) {
                如果 (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
                  返回 false;
                }
              } 否则 {
                返回 false;
              }
            } break;
          }
          // validate byte 3
          如果 (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
            返回 false;
          }
        } break;  // 3
        case 4: {
          auto ch1 = s[idx];
          switch (ch1) {
            case 0xF0u: {
              如果 (++idx >= len || s[idx] < 0x90u || s[idx] > 0xBFu) {
                返回 false;
              }
            } break;
            case 0xF4u: {
              如果 (++idx >= len || s[idx] < 0x80u || s[idx] > 0x8Fu) {
                返回 false;
              }
            } break;
            default: {
              如果 (ch1 >= 0xF1u && ch1 <= 0xF3u) {
                如果 (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
                  返回 false;
                }
              } 否则 {
                返回 false;
              }
            } break;
          }
          // validate bytes 3 and 4
          size_t stop = idx + 2;
          当 (idx < stop) {
            如果 (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
              返回 false;
            }
          }
        } break;  // 4
        default:
          // no chars longer than 4
          返回 false;
      }  // switch bytes
      ++idx;
      ++utf8_len;
    } 否则 {
      返回 false;
    }
  }
  // End index must match
  // the end of the last byte sequence.
  如果 (idx != len) {
    返回 false;
  }
  utf8_chars = utf8_len;
  返回 true;
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

static_assert(sizeof(wchar_t) >= 4,
              "Non-Windows UTF-8/wchar_t conversion helpers require wchar_t to be at least 32 bits.");

/// Compute the number of UTF-8 bytes required to encode a wide string.
inline size_t WideToUtf8RequiredSize(const std::wstring& wstr) {
  size_t result = 0;
  为 (wchar_t wc : wstr) {
    char32_t cp = static_cast<char32_t>(wc);
    如果 (cp >= 0xD800 && cp <= 0xDFFF) {
      ORT_THROW("Invalid Unicode surrogate codepoint U+", std::hex, static_cast<uint32_t>(cp));
    }
    如果 (cp <= 0x7F) {
      result += 1;
    } 否则 如果 (cp <= 0x7FF) {
      result += 2;
    } 否则 如果 (cp <= 0xFFFF) {
      result += 3;
    } 否则 如果 (cp <= 0x10FFFF) {
      result += 4;
    } 否则 {
      ORT_THROW("Invalid Unicode codepoint U+", std::hex, static_cast<uint32_t>(cp));
    }
  }
  返回 result;
}

/// Convert a wide string to UTF-8, writing into a pre-allocated std::string.
/// The string is resized to the actual number of bytes written.
inline Status WideToUtf8(const std::wstring& wstr, std::string& str) {
  如果 (wstr.empty()) {
    str.clear();
    返回 Status::OK();
  }

  char* dest = str.data();
  char* dest_end = dest + str.size();

  为 (wchar_t wc : wstr) {
    char32_t cp = static_cast<char32_t>(wc);
    如果 (cp >= 0xD800 && cp <= 0xDFFF) {
      返回 ORT_MAKE_STATUS(ONNXRUNTIME, FAIL,
                             "Invalid Unicode surrogate codepoint during UTF-8 conversion");
    }
    如果 (cp <= 0x7F) {
      const size_t remaining = static_cast<size_t>(dest_end - dest);
      如果 (remaining < 1) {
        返回 ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<char>(cp);
    } 否则 如果 (cp <= 0x7FF) {
      const size_t remaining = static_cast<size_t>(dest_end - dest);
      如果 (remaining < 2) {
        返回 ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<char>(0xC0 | (cp >> 6));
      *dest++ = static_cast<char>(0x80 | (cp & 0x3F));
    } 否则 如果 (cp <= 0xFFFF) {
      const size_t remaining = static_cast<size_t>(dest_end - dest);
      如果 (remaining < 3) {
        返回 ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<char>(0xE0 | (cp >> 12));
      *dest++ = static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
      *dest++ = static_cast<char>(0x80 | (cp & 0x3F));
    } 否则 如果 (cp <= 0x10FFFF) {
      const size_t remaining = static_cast<size_t>(dest_end - dest);
      如果 (remaining < 4) {
        返回 ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<char>(0xF0 | (cp >> 18));
      *dest++ = static_cast<char>(0x80 | ((cp >> 12) & 0x3F));
      *dest++ = static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
      *dest++ = static_cast<char>(0x80 | (cp & 0x3F));
    } 否则 {
      返回 ORT_MAKE_STATUS(ONNXRUNTIME, FAIL,
                             "Invalid Unicode codepoint during UTF-8 conversion");
    }
  }

  str.resize(static_cast<size_t>(dest - str.data()));
  返回 Status::OK();
}

/// Convert a UTF-8 string to a wide string, writing into a pre-allocated std::wstring.
/// The wstring is resized to the actual number of characters written.
/// Validates continuation bytes and rejects overlong encodings and surrogates.
inline Status Utf8ToWide(const std::string& str, std::wstring& wstr) {
  如果 (str.empty()) {
    wstr.clear();
    返回 Status::OK();
  }

  如果 (wstr.size() < str.size()) {
    wstr.resize(str.size());
  }

  const auto* src = reinterpret_cast<const unsigned char*>(str.data());
  const auto* src_end = src + str.size();
  wchar_t* dest = wstr.data();

  当 (src < src_end) {
    char32_t cp = 0;
    size_t byte_len = 0;

    如果 ((*src & 0x80) == 0) {
      cp = *src;
      byte_len = 1;
    } 否则 如果 ((*src & 0xE0) == 0xC0) {
      byte_len = 2;
      如果 (static_cast<size_t>(src_end - src) < 2) {
        返回 ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      如果 ((src[1] & 0xC0) != 0x80) {
        返回 ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x1F) << 6) |
           static_cast<char32_t>(src[1] & 0x3F);
      // Reject overlong encoding (must be >= 0x80 for 2-byte)
      如果 (cp < 0x80) {
        返回 ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
    } 否则 如果 ((*src & 0xF0) == 0xE0) {
      byte_len = 3;
      如果 (static_cast<size_t>(src_end - src) < 3) {
        返回 ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      如果 ((src[1] & 0xC0) != 0x80 || (src[2] & 0xC0) != 0x80) {
        返回 ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x0F) << 12) |
           (static_cast<char32_t>(src[1] & 0x3F) << 6) |
           static_cast<char32_t>(src[2] & 0x3F);
      // Reject overlong encoding (must be >= 0x800 for 3-byte)
      如果 (cp < 0x800) {
        返回 ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
      // Reject UTF-16 surrogates (U+D800..U+DFFF)
      如果 (cp >= 0xD800 && cp <= 0xDFFF) {
        返回 ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8: surrogate codepoint");
      }
    } 否则 如果 ((*src & 0xF8) == 0xF0) {
      byte_len = 4;
      如果 (static_cast<size_t>(src_end - src) < 4) {
        返回 ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      如果 ((src[1] & 0xC0) != 0x80 || (src[2] & 0xC0) != 0x80 || (src[3] & 0xC0) != 0x80) {
        返回 ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x07) << 18) |
           (static_cast<char32_t>(src[1] & 0x3F) << 12) |
           (static_cast<char32_t>(src[2] & 0x3F) << 6) |
           static_cast<char32_t>(src[3] & 0x3F);
      // Reject overlong encoding (must be >= 0x10000 for 4-byte)
      如果 (cp < 0x10000) {
        返回 ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
      // Reject codepoints beyond Unicode range
      如果 (cp > 0x10FFFF) {
        返回 ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8: codepoint beyond U+10FFFF");
      }
    } 否则 {
      返回 ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8 lead byte");
    }

    *dest++ = static_cast<wchar_t>(cp);
    src += byte_len;
  }

  wstr.resize(static_cast<size_t>(dest - wstr.data()));
  返回 Status::OK();
}

/// Convenience: convert UTF-8 string to wstring (throws on error).
inline std::wstring Utf8ToWideString(const std::string& s) {
  // UTF-8 byte count is an upper bound on wchar_t count
  std::wstring result;
  result.resize(s.size());
  ORT_THROW_IF_ERROR(Utf8ToWide(s, result));
  返回 result;
}

#endif  // !_WIN32

}  // namespace utf8_util
}  // namespace onnxruntime
