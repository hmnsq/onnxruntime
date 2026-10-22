// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#ضمن "core/common/common.h"

#ضمن <string>

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
مضمن bool utf8_bytes(unsigned حرف ch, size_t& len) {
  𐤀𐤌 ((ch & 0x80) == 0) {
    len = 1;
    أرجع true;
  }
  𐤀𐤌 ((ch & 0xE0) == 0xC0) {
    len = 2;
    أرجع true;
  }
  𐤀𐤌 ((ch & 0xF0) == 0xE0) {
    len = 3;
    أرجع true;
  }
  𐤀𐤌 ((ch & 0xF8) == 0xF0) {
    len = 4;
    أرجع true;
  }
  أرجع false;
}

// Computes length of the utf8 string in characters
مضمن bool utf8_len(ثابت unsigned حرف* s, size_t bytes, size_t& len) {
  size_t result = 0;
  طالما (bytes > 0) {
    size_t char_bytes = 0;
    bool valid = utf8_bytes(*s, char_bytes);
    𐤀𐤌 (!valid || bytes < char_bytes) {
      أرجع false;
    }
    bytes -= char_bytes;
    s += char_bytes;
    ++result;
  }
  len = result;
  أرجع true;
}

مضمن bool utf8_validate(ثابت unsigned حرف* s, size_t len, size_t& utf8_chars) {
  size_t utf8_len = 0;
  size_t idx = 0;
  طالما (idx < len) {
    size_t bytes = 0;
    auto ch = s[idx];
    𐤀𐤌 (utf8_bytes(ch, bytes)) {
      بدل (bytes) {
        حالة 1:
          اكسر;
        حالة 2: {
          // Reject overlong 2-byte sequences. Valid Unicode 2-byte encodings
          // start at U+0080, so lead bytes 0xC0 and 0xC1 are invalid.
          𐤀𐤌 (ch < 0xC2u) {
            أرجع false;
          }
          𐤀𐤌 (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
            أرجع false;
          }
        } اكسر;  // 2
        حالة 3: {
          auto ch1 = s[idx];
          بدل (ch1) {
            حالة 0xE0u:
              𐤀𐤌 (++idx >= len || s[idx] < 0xA0u || s[idx] > 0xBFu) {
                أرجع false;
              }
              اكسر;
            حالة 0xEDu:
              𐤀𐤌 (++idx >= len || s[idx] < 0x80u || s[idx] > 0x9Fu) {
                أرجع false;
              }
              اكسر;
            افتراضي: {
              𐤀𐤌 ((ch1 >= 0xE1u && ch1 <= 0xECu) ||
                  (ch1 >= 0xEEu && ch1 <= 0xEFu)) {
                𐤀𐤌 (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
                  أرجع false;
                }
              } وإلا {
                أرجع false;
              }
            } اكسر;
          }
          // validate byte 3
          𐤀𐤌 (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
            أرجع false;
          }
        } اكسر;  // 3
        حالة 4: {
          auto ch1 = s[idx];
          بدل (ch1) {
            حالة 0xF0u: {
              𐤀𐤌 (++idx >= len || s[idx] < 0x90u || s[idx] > 0xBFu) {
                أرجع false;
              }
            } اكسر;
            حالة 0xF4u: {
              𐤀𐤌 (++idx >= len || s[idx] < 0x80u || s[idx] > 0x8Fu) {
                أرجع false;
              }
            } اكسر;
            افتراضي: {
              𐤀𐤌 (ch1 >= 0xF1u && ch1 <= 0xF3u) {
                𐤀𐤌 (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
                  أرجع false;
                }
              } وإلا {
                أرجع false;
              }
            } اكسر;
          }
          // validate bytes 3 and 4
          size_t stop = idx + 2;
          طالما (idx < stop) {
            𐤀𐤌 (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
              أرجع false;
            }
          }
        } اكسر;  // 4
        افتراضي:
          // no chars longer than 4
          أرجع false;
      }  // switch bytes
      ++idx;
      ++utf8_len;
    } وإلا {
      أرجع false;
    }
  }
  // End index must match
  // the end of the last byte sequence.
  𐤀𐤌 (idx != len) {
    أرجع false;
  }
  utf8_chars = utf8_len;
  أرجع true;
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

static_assert(حجم(wchar_t) >= 4,
              "Non-Windows UTF-8/wchar_t conversion helpers require wchar_t to be at least 32 bits.");

/// Compute the number of UTF-8 bytes required to encode a wide string.
مضمن size_t WideToUtf8RequiredSize(ثابت std::wstring& wstr) {
  size_t result = 0;
  لكل (wchar_t wc : wstr) {
    char32_t cp = static_cast<char32_t>(wc);
    𐤀𐤌 (cp >= 0xD800 && cp <= 0xDFFF) {
      ORT_THROW("Invalid Unicode surrogate codepoint U+", std::hex, static_cast<uint32_t>(cp));
    }
    𐤀𐤌 (cp <= 0x7F) {
      result += 1;
    } وإلا 𐤀𐤌 (cp <= 0x7FF) {
      result += 2;
    } وإلا 𐤀𐤌 (cp <= 0xFFFF) {
      result += 3;
    } وإلا 𐤀𐤌 (cp <= 0x10FFFF) {
      result += 4;
    } وإلا {
      ORT_THROW("Invalid Unicode codepoint U+", std::hex, static_cast<uint32_t>(cp));
    }
  }
  أرجع result;
}

/// Convert a wide string to UTF-8, writing into a pre-allocated std::string.
/// The string is resized to the actual number of bytes written.
مضمن Status WideToUtf8(ثابت std::wstring& wstr, std::string& str) {
  𐤀𐤌 (wstr.empty()) {
    str.clear();
    أرجع Status::OK();
  }

  حرف* dest = str.data();
  حرف* dest_end = dest + str.size();

  لكل (wchar_t wc : wstr) {
    char32_t cp = static_cast<char32_t>(wc);
    𐤀𐤌 (cp >= 0xD800 && cp <= 0xDFFF) {
      أرجع ORT_MAKE_STATUS(ONNXRUNTIME, FAIL,
                             "Invalid Unicode surrogate codepoint during UTF-8 conversion");
    }
    𐤀𐤌 (cp <= 0x7F) {
      ثابت size_t remaining = static_cast<size_t>(dest_end - dest);
      𐤀𐤌 (remaining < 1) {
        أرجع ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<حرف>(cp);
    } وإلا 𐤀𐤌 (cp <= 0x7FF) {
      ثابت size_t remaining = static_cast<size_t>(dest_end - dest);
      𐤀𐤌 (remaining < 2) {
        أرجع ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<حرف>(0xC0 | (cp >> 6));
      *dest++ = static_cast<حرف>(0x80 | (cp & 0x3F));
    } وإلا 𐤀𐤌 (cp <= 0xFFFF) {
      ثابت size_t remaining = static_cast<size_t>(dest_end - dest);
      𐤀𐤌 (remaining < 3) {
        أرجع ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<حرف>(0xE0 | (cp >> 12));
      *dest++ = static_cast<حرف>(0x80 | ((cp >> 6) & 0x3F));
      *dest++ = static_cast<حرف>(0x80 | (cp & 0x3F));
    } وإلا 𐤀𐤌 (cp <= 0x10FFFF) {
      ثابت size_t remaining = static_cast<size_t>(dest_end - dest);
      𐤀𐤌 (remaining < 4) {
        أرجع ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<حرف>(0xF0 | (cp >> 18));
      *dest++ = static_cast<حرف>(0x80 | ((cp >> 12) & 0x3F));
      *dest++ = static_cast<حرف>(0x80 | ((cp >> 6) & 0x3F));
      *dest++ = static_cast<حرف>(0x80 | (cp & 0x3F));
    } وإلا {
      أرجع ORT_MAKE_STATUS(ONNXRUNTIME, FAIL,
                             "Invalid Unicode codepoint during UTF-8 conversion");
    }
  }

  str.resize(static_cast<size_t>(dest - str.data()));
  أرجع Status::OK();
}

/// Convert a UTF-8 string to a wide string, writing into a pre-allocated std::wstring.
/// The wstring is resized to the actual number of characters written.
/// Validates continuation bytes and rejects overlong encodings and surrogates.
مضمن Status Utf8ToWide(ثابت std::string& str, std::wstring& wstr) {
  𐤀𐤌 (str.empty()) {
    wstr.clear();
    أرجع Status::OK();
  }

  𐤀𐤌 (wstr.size() < str.size()) {
    wstr.resize(str.size());
  }

  ثابت auto* src = reinterpret_cast<ثابت unsigned حرف*>(str.data());
  ثابت auto* src_end = src + str.size();
  wchar_t* dest = wstr.data();

  طالما (src < src_end) {
    char32_t cp = 0;
    size_t byte_len = 0;

    𐤀𐤌 ((*src & 0x80) == 0) {
      cp = *src;
      byte_len = 1;
    } وإلا 𐤀𐤌 ((*src & 0xE0) == 0xC0) {
      byte_len = 2;
      𐤀𐤌 (static_cast<size_t>(src_end - src) < 2) {
        أرجع ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      𐤀𐤌 ((src[1] & 0xC0) != 0x80) {
        أرجع ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x1F) << 6) |
           static_cast<char32_t>(src[1] & 0x3F);
      // Reject overlong encoding (must be >= 0x80 for 2-byte)
      𐤀𐤌 (cp < 0x80) {
        أرجع ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
    } وإلا 𐤀𐤌 ((*src & 0xF0) == 0xE0) {
      byte_len = 3;
      𐤀𐤌 (static_cast<size_t>(src_end - src) < 3) {
        أرجع ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      𐤀𐤌 ((src[1] & 0xC0) != 0x80 || (src[2] & 0xC0) != 0x80) {
        أرجع ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x0F) << 12) |
           (static_cast<char32_t>(src[1] & 0x3F) << 6) |
           static_cast<char32_t>(src[2] & 0x3F);
      // Reject overlong encoding (must be >= 0x800 for 3-byte)
      𐤀𐤌 (cp < 0x800) {
        أرجع ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
      // Reject UTF-16 surrogates (U+D800..U+DFFF)
      𐤀𐤌 (cp >= 0xD800 && cp <= 0xDFFF) {
        أرجع ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8: surrogate codepoint");
      }
    } وإلا 𐤀𐤌 ((*src & 0xF8) == 0xF0) {
      byte_len = 4;
      𐤀𐤌 (static_cast<size_t>(src_end - src) < 4) {
        أرجع ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      𐤀𐤌 ((src[1] & 0xC0) != 0x80 || (src[2] & 0xC0) != 0x80 || (src[3] & 0xC0) != 0x80) {
        أرجع ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x07) << 18) |
           (static_cast<char32_t>(src[1] & 0x3F) << 12) |
           (static_cast<char32_t>(src[2] & 0x3F) << 6) |
           static_cast<char32_t>(src[3] & 0x3F);
      // Reject overlong encoding (must be >= 0x10000 for 4-byte)
      𐤀𐤌 (cp < 0x10000) {
        أرجع ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
      // Reject codepoints beyond Unicode range
      𐤀𐤌 (cp > 0x10FFFF) {
        أرجع ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8: codepoint beyond U+10FFFF");
      }
    } وإلا {
      أرجع ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8 lead byte");
    }

    *dest++ = static_cast<wchar_t>(cp);
    src += byte_len;
  }

  wstr.resize(static_cast<size_t>(dest - wstr.data()));
  أرجع Status::OK();
}

/// Convenience: convert UTF-8 string to wstring (throws on error).
مضمن std::wstring Utf8ToWideString(ثابت std::string& s) {
  // UTF-8 byte count is an upper bound on wchar_t count
  std::wstring result;
  result.resize(s.size());
  ORT_THROW_IF_ERROR(Utf8ToWide(s, result));
  أرجع result;
}

#endif  // !_WIN32

}  // namespace utf8_util
}  // namespace onnxruntime
