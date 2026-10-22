// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#अन्तर्भाव "core/common/common.h"

#अन्तर्भाव <string>

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
अन्तःस्थ bool utf8_bytes(unsigned वर्ण ch, size_t& len) {
  जइ ((ch & 0x80) == 0) {
    len = 1;
    प्रत्यागम true;
  }
  जइ ((ch & 0xE0) == 0xC0) {
    len = 2;
    प्रत्यागम true;
  }
  जइ ((ch & 0xF0) == 0xE0) {
    len = 3;
    प्रत्यागम true;
  }
  जइ ((ch & 0xF8) == 0xF0) {
    len = 4;
    प्रत्यागम true;
  }
  प्रत्यागम false;
}

// Computes length of the utf8 string in characters
अन्तःस्थ bool utf8_len(ध्रुव unsigned वर्ण* s, size_t bytes, size_t& len) {
  size_t result = 0;
  जाव (bytes > 0) {
    size_t char_bytes = 0;
    bool valid = utf8_bytes(*s, char_bytes);
    जइ (!valid || bytes < char_bytes) {
      प्रत्यागम false;
    }
    bytes -= char_bytes;
    s += char_bytes;
    ++result;
  }
  len = result;
  प्रत्यागम true;
}

अन्तःस्थ bool utf8_validate(ध्रुव unsigned वर्ण* s, size_t len, size_t& utf8_chars) {
  size_t utf8_len = 0;
  size_t idx = 0;
  जाव (idx < len) {
    size_t bytes = 0;
    auto ch = s[idx];
    जइ (utf8_bytes(ch, bytes)) {
      विकल्प (bytes) {
        पक्ष 1:
          भङ्ग;
        पक्ष 2: {
          // Reject overlong 2-byte sequences. Valid Unicode 2-byte encodings
          // start at U+0080, so lead bytes 0xC0 and 0xC1 are invalid.
          जइ (ch < 0xC2u) {
            प्रत्यागम false;
          }
          जइ (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
            प्रत्यागम false;
          }
        } भङ्ग;  // 2
        पक्ष 3: {
          auto ch1 = s[idx];
          विकल्प (ch1) {
            पक्ष 0xE0u:
              जइ (++idx >= len || s[idx] < 0xA0u || s[idx] > 0xBFu) {
                प्रत्यागम false;
              }
              भङ्ग;
            पक्ष 0xEDu:
              जइ (++idx >= len || s[idx] < 0x80u || s[idx] > 0x9Fu) {
                प्रत्यागम false;
              }
              भङ्ग;
            स्वभाव: {
              जइ ((ch1 >= 0xE1u && ch1 <= 0xECu) ||
                  (ch1 >= 0xEEu && ch1 <= 0xEFu)) {
                जइ (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
                  प्रत्यागम false;
                }
              } अण्णहा {
                प्रत्यागम false;
              }
            } भङ्ग;
          }
          // validate byte 3
          जइ (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
            प्रत्यागम false;
          }
        } भङ्ग;  // 3
        पक्ष 4: {
          auto ch1 = s[idx];
          विकल्प (ch1) {
            पक्ष 0xF0u: {
              जइ (++idx >= len || s[idx] < 0x90u || s[idx] > 0xBFu) {
                प्रत्यागम false;
              }
            } भङ्ग;
            पक्ष 0xF4u: {
              जइ (++idx >= len || s[idx] < 0x80u || s[idx] > 0x8Fu) {
                प्रत्यागम false;
              }
            } भङ्ग;
            स्वभाव: {
              जइ (ch1 >= 0xF1u && ch1 <= 0xF3u) {
                जइ (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
                  प्रत्यागम false;
                }
              } अण्णहा {
                प्रत्यागम false;
              }
            } भङ्ग;
          }
          // validate bytes 3 and 4
          size_t stop = idx + 2;
          जाव (idx < stop) {
            जइ (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
              प्रत्यागम false;
            }
          }
        } भङ्ग;  // 4
        स्वभाव:
          // no chars longer than 4
          प्रत्यागम false;
      }  // switch bytes
      ++idx;
      ++utf8_len;
    } अण्णहा {
      प्रत्यागम false;
    }
  }
  // End index must match
  // the end of the last byte sequence.
  जइ (idx != len) {
    प्रत्यागम false;
  }
  utf8_chars = utf8_len;
  प्रत्यागम true;
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

static_assert(परिमाण(wchar_t) >= 4,
              "Non-Windows UTF-8/wchar_t conversion helpers require wchar_t to be at least 32 bits.");

/// Compute the number of UTF-8 bytes required to encode a wide string.
अन्तःस्थ size_t WideToUtf8RequiredSize(ध्रुव std::wstring& wstr) {
  size_t result = 0;
  प्रति (wchar_t wc : wstr) {
    char32_t cp = static_cast<char32_t>(wc);
    जइ (cp >= 0xD800 && cp <= 0xDFFF) {
      ORT_THROW("Invalid Unicode surrogate codepoint U+", std::hex, static_cast<uint32_t>(cp));
    }
    जइ (cp <= 0x7F) {
      result += 1;
    } अण्णहा जइ (cp <= 0x7FF) {
      result += 2;
    } अण्णहा जइ (cp <= 0xFFFF) {
      result += 3;
    } अण्णहा जइ (cp <= 0x10FFFF) {
      result += 4;
    } अण्णहा {
      ORT_THROW("Invalid Unicode codepoint U+", std::hex, static_cast<uint32_t>(cp));
    }
  }
  प्रत्यागम result;
}

/// Convert a wide string to UTF-8, writing into a pre-allocated std::string.
/// The string is resized to the actual number of bytes written.
अन्तःस्थ Status WideToUtf8(ध्रुव std::wstring& wstr, std::string& str) {
  जइ (wstr.empty()) {
    str.clear();
    प्रत्यागम Status::OK();
  }

  वर्ण* dest = str.data();
  वर्ण* dest_end = dest + str.size();

  प्रति (wchar_t wc : wstr) {
    char32_t cp = static_cast<char32_t>(wc);
    जइ (cp >= 0xD800 && cp <= 0xDFFF) {
      प्रत्यागम ORT_MAKE_STATUS(ONNXRUNTIME, FAIL,
                             "Invalid Unicode surrogate codepoint during UTF-8 conversion");
    }
    जइ (cp <= 0x7F) {
      ध्रुव size_t remaining = static_cast<size_t>(dest_end - dest);
      जइ (remaining < 1) {
        प्रत्यागम ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<वर्ण>(cp);
    } अण्णहा जइ (cp <= 0x7FF) {
      ध्रुव size_t remaining = static_cast<size_t>(dest_end - dest);
      जइ (remaining < 2) {
        प्रत्यागम ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<वर्ण>(0xC0 | (cp >> 6));
      *dest++ = static_cast<वर्ण>(0x80 | (cp & 0x3F));
    } अण्णहा जइ (cp <= 0xFFFF) {
      ध्रुव size_t remaining = static_cast<size_t>(dest_end - dest);
      जइ (remaining < 3) {
        प्रत्यागम ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<वर्ण>(0xE0 | (cp >> 12));
      *dest++ = static_cast<वर्ण>(0x80 | ((cp >> 6) & 0x3F));
      *dest++ = static_cast<वर्ण>(0x80 | (cp & 0x3F));
    } अण्णहा जइ (cp <= 0x10FFFF) {
      ध्रुव size_t remaining = static_cast<size_t>(dest_end - dest);
      जइ (remaining < 4) {
        प्रत्यागम ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<वर्ण>(0xF0 | (cp >> 18));
      *dest++ = static_cast<वर्ण>(0x80 | ((cp >> 12) & 0x3F));
      *dest++ = static_cast<वर्ण>(0x80 | ((cp >> 6) & 0x3F));
      *dest++ = static_cast<वर्ण>(0x80 | (cp & 0x3F));
    } अण्णहा {
      प्रत्यागम ORT_MAKE_STATUS(ONNXRUNTIME, FAIL,
                             "Invalid Unicode codepoint during UTF-8 conversion");
    }
  }

  str.resize(static_cast<size_t>(dest - str.data()));
  प्रत्यागम Status::OK();
}

/// Convert a UTF-8 string to a wide string, writing into a pre-allocated std::wstring.
/// The wstring is resized to the actual number of characters written.
/// Validates continuation bytes and rejects overlong encodings and surrogates.
अन्तःस्थ Status Utf8ToWide(ध्रुव std::string& str, std::wstring& wstr) {
  जइ (str.empty()) {
    wstr.clear();
    प्रत्यागम Status::OK();
  }

  जइ (wstr.size() < str.size()) {
    wstr.resize(str.size());
  }

  ध्रुव auto* src = reinterpret_cast<ध्रुव unsigned वर्ण*>(str.data());
  ध्रुव auto* src_end = src + str.size();
  wchar_t* dest = wstr.data();

  जाव (src < src_end) {
    char32_t cp = 0;
    size_t byte_len = 0;

    जइ ((*src & 0x80) == 0) {
      cp = *src;
      byte_len = 1;
    } अण्णहा जइ ((*src & 0xE0) == 0xC0) {
      byte_len = 2;
      जइ (static_cast<size_t>(src_end - src) < 2) {
        प्रत्यागम ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      जइ ((src[1] & 0xC0) != 0x80) {
        प्रत्यागम ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x1F) << 6) |
           static_cast<char32_t>(src[1] & 0x3F);
      // Reject overlong encoding (must be >= 0x80 for 2-byte)
      जइ (cp < 0x80) {
        प्रत्यागम ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
    } अण्णहा जइ ((*src & 0xF0) == 0xE0) {
      byte_len = 3;
      जइ (static_cast<size_t>(src_end - src) < 3) {
        प्रत्यागम ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      जइ ((src[1] & 0xC0) != 0x80 || (src[2] & 0xC0) != 0x80) {
        प्रत्यागम ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x0F) << 12) |
           (static_cast<char32_t>(src[1] & 0x3F) << 6) |
           static_cast<char32_t>(src[2] & 0x3F);
      // Reject overlong encoding (must be >= 0x800 for 3-byte)
      जइ (cp < 0x800) {
        प्रत्यागम ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
      // Reject UTF-16 surrogates (U+D800..U+DFFF)
      जइ (cp >= 0xD800 && cp <= 0xDFFF) {
        प्रत्यागम ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8: surrogate codepoint");
      }
    } अण्णहा जइ ((*src & 0xF8) == 0xF0) {
      byte_len = 4;
      जइ (static_cast<size_t>(src_end - src) < 4) {
        प्रत्यागम ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      जइ ((src[1] & 0xC0) != 0x80 || (src[2] & 0xC0) != 0x80 || (src[3] & 0xC0) != 0x80) {
        प्रत्यागम ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x07) << 18) |
           (static_cast<char32_t>(src[1] & 0x3F) << 12) |
           (static_cast<char32_t>(src[2] & 0x3F) << 6) |
           static_cast<char32_t>(src[3] & 0x3F);
      // Reject overlong encoding (must be >= 0x10000 for 4-byte)
      जइ (cp < 0x10000) {
        प्रत्यागम ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
      // Reject codepoints beyond Unicode range
      जइ (cp > 0x10FFFF) {
        प्रत्यागम ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8: codepoint beyond U+10FFFF");
      }
    } अण्णहा {
      प्रत्यागम ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8 lead byte");
    }

    *dest++ = static_cast<wchar_t>(cp);
    src += byte_len;
  }

  wstr.resize(static_cast<size_t>(dest - wstr.data()));
  प्रत्यागम Status::OK();
}

/// Convenience: convert UTF-8 string to wstring (throws on error).
अन्तःस्थ std::wstring Utf8ToWideString(ध्रुव std::string& s) {
  // UTF-8 byte count is an upper bound on wchar_t count
  std::wstring result;
  result.resize(s.size());
  ORT_THROW_IF_ERROR(Utf8ToWide(s, result));
  प्रत्यागम result;
}

#endif  // !_WIN32

}  // namespace utf8_util
}  // namespace onnxruntime
