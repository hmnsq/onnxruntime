// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#അന്തര്ഭാവ "core/common/common.h"

#അന്തര്ഭാവ <string>

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
വരിയിൽ bool utf8_bytes(unsigned അക്ഷരം ch, size_t& len) {
  എങ്കിൽ ((ch & 0x80) == 0) {
    len = 1;
    തിരികെ true;
  }
  എങ്കിൽ ((ch & 0xE0) == 0xC0) {
    len = 2;
    തിരികെ true;
  }
  എങ്കിൽ ((ch & 0xF0) == 0xE0) {
    len = 3;
    തിരികെ true;
  }
  എങ്കിൽ ((ch & 0xF8) == 0xF0) {
    len = 4;
    തിരികെ true;
  }
  തിരികെ false;
}

// Computes length of the utf8 string in characters
വരിയിൽ bool utf8_len(സ്ഥിരം unsigned അക്ഷരം* s, size_t bytes, size_t& len) {
  size_t result = 0;
  ആകുമ്പോൾ (bytes > 0) {
    size_t char_bytes = 0;
    bool valid = utf8_bytes(*s, char_bytes);
    എങ്കിൽ (!valid || bytes < char_bytes) {
      തിരികെ false;
    }
    bytes -= char_bytes;
    s += char_bytes;
    ++result;
  }
  len = result;
  തിരികെ true;
}

വരിയിൽ bool utf8_validate(സ്ഥിരം unsigned അക്ഷരം* s, size_t len, size_t& utf8_chars) {
  size_t utf8_len = 0;
  size_t idx = 0;
  ആകുമ്പോൾ (idx < len) {
    size_t bytes = 0;
    auto ch = s[idx];
    എങ്കിൽ (utf8_bytes(ch, bytes)) {
      തിരഞ്ഞെടുപ്പ് (bytes) {
        അവസ്ഥ 1:
          നിർത്തുക;
        അവസ്ഥ 2: {
          // Reject overlong 2-byte sequences. Valid Unicode 2-byte encodings
          // start at U+0080, so lead bytes 0xC0 and 0xC1 are invalid.
          എങ്കിൽ (ch < 0xC2u) {
            തിരികെ false;
          }
          എങ്കിൽ (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
            തിരികെ false;
          }
        } നിർത്തുക;  // 2
        അവസ്ഥ 3: {
          auto ch1 = s[idx];
          തിരഞ്ഞെടുപ്പ് (ch1) {
            അവസ്ഥ 0xE0u:
              എങ്കിൽ (++idx >= len || s[idx] < 0xA0u || s[idx] > 0xBFu) {
                തിരികെ false;
              }
              നിർത്തുക;
            അവസ്ഥ 0xEDu:
              എങ്കിൽ (++idx >= len || s[idx] < 0x80u || s[idx] > 0x9Fu) {
                തിരികെ false;
              }
              നിർത്തുക;
            സ്ഥിരസ്ഥിതി: {
              എങ്കിൽ ((ch1 >= 0xE1u && ch1 <= 0xECu) ||
                  (ch1 >= 0xEEu && ch1 <= 0xEFu)) {
                എങ്കിൽ (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
                  തിരികെ false;
                }
              } അല്ലെങ്കിൽ {
                തിരികെ false;
              }
            } നിർത്തുക;
          }
          // validate byte 3
          എങ്കിൽ (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
            തിരികെ false;
          }
        } നിർത്തുക;  // 3
        അവസ്ഥ 4: {
          auto ch1 = s[idx];
          തിരഞ്ഞെടുപ്പ് (ch1) {
            അവസ്ഥ 0xF0u: {
              എങ്കിൽ (++idx >= len || s[idx] < 0x90u || s[idx] > 0xBFu) {
                തിരികെ false;
              }
            } നിർത്തുക;
            അവസ്ഥ 0xF4u: {
              എങ്കിൽ (++idx >= len || s[idx] < 0x80u || s[idx] > 0x8Fu) {
                തിരികെ false;
              }
            } നിർത്തുക;
            സ്ഥിരസ്ഥിതി: {
              എങ്കിൽ (ch1 >= 0xF1u && ch1 <= 0xF3u) {
                എങ്കിൽ (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
                  തിരികെ false;
                }
              } അല്ലെങ്കിൽ {
                തിരികെ false;
              }
            } നിർത്തുക;
          }
          // validate bytes 3 and 4
          size_t stop = idx + 2;
          ആകുമ്പോൾ (idx < stop) {
            എങ്കിൽ (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
              തിരികെ false;
            }
          }
        } നിർത്തുക;  // 4
        സ്ഥിരസ്ഥിതി:
          // no chars longer than 4
          തിരികെ false;
      }  // switch bytes
      ++idx;
      ++utf8_len;
    } അല്ലെങ്കിൽ {
      തിരികെ false;
    }
  }
  // End index must match
  // the end of the last byte sequence.
  എങ്കിൽ (idx != len) {
    തിരികെ false;
  }
  utf8_chars = utf8_len;
  തിരികെ true;
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

static_assert(വലുപ്പം(wchar_t) >= 4,
              "Non-Windows UTF-8/wchar_t conversion helpers require wchar_t to be at least 32 bits.");

/// Compute the number of UTF-8 bytes required to encode a wide string.
വരിയിൽ size_t WideToUtf8RequiredSize(സ്ഥിരം std::wstring& wstr) {
  size_t result = 0;
  വേണ്ടി (wchar_t wc : wstr) {
    char32_t cp = static_cast<char32_t>(wc);
    എങ്കിൽ (cp >= 0xD800 && cp <= 0xDFFF) {
      ORT_THROW("Invalid Unicode surrogate codepoint U+", std::hex, static_cast<uint32_t>(cp));
    }
    എങ്കിൽ (cp <= 0x7F) {
      result += 1;
    } അല്ലെങ്കിൽ എങ്കിൽ (cp <= 0x7FF) {
      result += 2;
    } അല്ലെങ്കിൽ എങ്കിൽ (cp <= 0xFFFF) {
      result += 3;
    } അല്ലെങ്കിൽ എങ്കിൽ (cp <= 0x10FFFF) {
      result += 4;
    } അല്ലെങ്കിൽ {
      ORT_THROW("Invalid Unicode codepoint U+", std::hex, static_cast<uint32_t>(cp));
    }
  }
  തിരികെ result;
}

/// Convert a wide string to UTF-8, writing into a pre-allocated std::string.
/// The string is resized to the actual number of bytes written.
വരിയിൽ Status WideToUtf8(സ്ഥിരം std::wstring& wstr, std::string& str) {
  എങ്കിൽ (wstr.empty()) {
    str.clear();
    തിരികെ Status::OK();
  }

  അക്ഷരം* dest = str.data();
  അക്ഷരം* dest_end = dest + str.size();

  വേണ്ടി (wchar_t wc : wstr) {
    char32_t cp = static_cast<char32_t>(wc);
    എങ്കിൽ (cp >= 0xD800 && cp <= 0xDFFF) {
      തിരികെ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL,
                             "Invalid Unicode surrogate codepoint during UTF-8 conversion");
    }
    എങ്കിൽ (cp <= 0x7F) {
      സ്ഥിരം size_t remaining = static_cast<size_t>(dest_end - dest);
      എങ്കിൽ (remaining < 1) {
        തിരികെ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<അക്ഷരം>(cp);
    } അല്ലെങ്കിൽ എങ്കിൽ (cp <= 0x7FF) {
      സ്ഥിരം size_t remaining = static_cast<size_t>(dest_end - dest);
      എങ്കിൽ (remaining < 2) {
        തിരികെ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<അക്ഷരം>(0xC0 | (cp >> 6));
      *dest++ = static_cast<അക്ഷരം>(0x80 | (cp & 0x3F));
    } അല്ലെങ്കിൽ എങ്കിൽ (cp <= 0xFFFF) {
      സ്ഥിരം size_t remaining = static_cast<size_t>(dest_end - dest);
      എങ്കിൽ (remaining < 3) {
        തിരികെ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<അക്ഷരം>(0xE0 | (cp >> 12));
      *dest++ = static_cast<അക്ഷരം>(0x80 | ((cp >> 6) & 0x3F));
      *dest++ = static_cast<അക്ഷരം>(0x80 | (cp & 0x3F));
    } അല്ലെങ്കിൽ എങ്കിൽ (cp <= 0x10FFFF) {
      സ്ഥിരം size_t remaining = static_cast<size_t>(dest_end - dest);
      എങ്കിൽ (remaining < 4) {
        തിരികെ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<അക്ഷരം>(0xF0 | (cp >> 18));
      *dest++ = static_cast<അക്ഷരം>(0x80 | ((cp >> 12) & 0x3F));
      *dest++ = static_cast<അക്ഷരം>(0x80 | ((cp >> 6) & 0x3F));
      *dest++ = static_cast<അക്ഷരം>(0x80 | (cp & 0x3F));
    } അല്ലെങ്കിൽ {
      തിരികെ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL,
                             "Invalid Unicode codepoint during UTF-8 conversion");
    }
  }

  str.resize(static_cast<size_t>(dest - str.data()));
  തിരികെ Status::OK();
}

/// Convert a UTF-8 string to a wide string, writing into a pre-allocated std::wstring.
/// The wstring is resized to the actual number of characters written.
/// Validates continuation bytes and rejects overlong encodings and surrogates.
വരിയിൽ Status Utf8ToWide(സ്ഥിരം std::string& str, std::wstring& wstr) {
  എങ്കിൽ (str.empty()) {
    wstr.clear();
    തിരികെ Status::OK();
  }

  എങ്കിൽ (wstr.size() < str.size()) {
    wstr.resize(str.size());
  }

  സ്ഥിരം auto* src = reinterpret_cast<സ്ഥിരം unsigned അക്ഷരം*>(str.data());
  സ്ഥിരം auto* src_end = src + str.size();
  wchar_t* dest = wstr.data();

  ആകുമ്പോൾ (src < src_end) {
    char32_t cp = 0;
    size_t byte_len = 0;

    എങ്കിൽ ((*src & 0x80) == 0) {
      cp = *src;
      byte_len = 1;
    } അല്ലെങ്കിൽ എങ്കിൽ ((*src & 0xE0) == 0xC0) {
      byte_len = 2;
      എങ്കിൽ (static_cast<size_t>(src_end - src) < 2) {
        തിരികെ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      എങ്കിൽ ((src[1] & 0xC0) != 0x80) {
        തിരികെ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x1F) << 6) |
           static_cast<char32_t>(src[1] & 0x3F);
      // Reject overlong encoding (must be >= 0x80 for 2-byte)
      എങ്കിൽ (cp < 0x80) {
        തിരികെ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
    } അല്ലെങ്കിൽ എങ്കിൽ ((*src & 0xF0) == 0xE0) {
      byte_len = 3;
      എങ്കിൽ (static_cast<size_t>(src_end - src) < 3) {
        തിരികെ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      എങ്കിൽ ((src[1] & 0xC0) != 0x80 || (src[2] & 0xC0) != 0x80) {
        തിരികെ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x0F) << 12) |
           (static_cast<char32_t>(src[1] & 0x3F) << 6) |
           static_cast<char32_t>(src[2] & 0x3F);
      // Reject overlong encoding (must be >= 0x800 for 3-byte)
      എങ്കിൽ (cp < 0x800) {
        തിരികെ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
      // Reject UTF-16 surrogates (U+D800..U+DFFF)
      എങ്കിൽ (cp >= 0xD800 && cp <= 0xDFFF) {
        തിരികെ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8: surrogate codepoint");
      }
    } അല്ലെങ്കിൽ എങ്കിൽ ((*src & 0xF8) == 0xF0) {
      byte_len = 4;
      എങ്കിൽ (static_cast<size_t>(src_end - src) < 4) {
        തിരികെ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      എങ്കിൽ ((src[1] & 0xC0) != 0x80 || (src[2] & 0xC0) != 0x80 || (src[3] & 0xC0) != 0x80) {
        തിരികെ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x07) << 18) |
           (static_cast<char32_t>(src[1] & 0x3F) << 12) |
           (static_cast<char32_t>(src[2] & 0x3F) << 6) |
           static_cast<char32_t>(src[3] & 0x3F);
      // Reject overlong encoding (must be >= 0x10000 for 4-byte)
      എങ്കിൽ (cp < 0x10000) {
        തിരികെ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
      // Reject codepoints beyond Unicode range
      എങ്കിൽ (cp > 0x10FFFF) {
        തിരികെ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8: codepoint beyond U+10FFFF");
      }
    } അല്ലെങ്കിൽ {
      തിരികെ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8 lead byte");
    }

    *dest++ = static_cast<wchar_t>(cp);
    src += byte_len;
  }

  wstr.resize(static_cast<size_t>(dest - wstr.data()));
  തിരികെ Status::OK();
}

/// Convenience: convert UTF-8 string to wstring (throws on error).
വരിയിൽ std::wstring Utf8ToWideString(സ്ഥിരം std::string& s) {
  // UTF-8 byte count is an upper bound on wchar_t count
  std::wstring result;
  result.resize(s.size());
  ORT_THROW_IF_ERROR(Utf8ToWide(s, result));
  തിരികെ result;
}

#endif  // !_WIN32

}  // namespace utf8_util
}  // namespace onnxruntime
