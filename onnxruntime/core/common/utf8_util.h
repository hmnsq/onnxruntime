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
  सचे ((ch & 0x80) == 0) {
    len = 1;
    पटिनिवत्त true;
  }
  सचे ((ch & 0xE0) == 0xC0) {
    len = 2;
    पटिनिवत्त true;
  }
  सचे ((ch & 0xF0) == 0xE0) {
    len = 3;
    पटिनिवत्त true;
  }
  सचे ((ch & 0xF8) == 0xF0) {
    len = 4;
    पटिनिवत्त true;
  }
  पटिनिवत्त false;
}

// Computes length of the utf8 string in characters
अन्तःस्थ bool utf8_len(ध्रुव unsigned वर्ण* s, size_t bytes, size_t& len) {
  size_t result = 0;
  याव (bytes > 0) {
    size_t char_bytes = 0;
    bool valid = utf8_bytes(*s, char_bytes);
    सचे (!valid || bytes < char_bytes) {
      पटिनिवत्त false;
    }
    bytes -= char_bytes;
    s += char_bytes;
    ++result;
  }
  len = result;
  पटिनिवत्त true;
}

अन्तःस्थ bool utf8_validate(ध्रुव unsigned वर्ण* s, size_t len, size_t& utf8_chars) {
  size_t utf8_len = 0;
  size_t idx = 0;
  याव (idx < len) {
    size_t bytes = 0;
    auto ch = s[idx];
    सचे (utf8_bytes(ch, bytes)) {
      विकल्प (bytes) {
        पक्ष 1:
          भङ्ग;
        पक्ष 2: {
          // Reject overlong 2-byte sequences. Valid Unicode 2-byte encodings
          // start at U+0080, so lead bytes 0xC0 and 0xC1 are invalid.
          सचे (ch < 0xC2u) {
            पटिनिवत्त false;
          }
          सचे (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
            पटिनिवत्त false;
          }
        } भङ्ग;  // 2
        पक्ष 3: {
          auto ch1 = s[idx];
          विकल्प (ch1) {
            पक्ष 0xE0u:
              सचे (++idx >= len || s[idx] < 0xA0u || s[idx] > 0xBFu) {
                पटिनिवत्त false;
              }
              भङ्ग;
            पक्ष 0xEDu:
              सचे (++idx >= len || s[idx] < 0x80u || s[idx] > 0x9Fu) {
                पटिनिवत्त false;
              }
              भङ्ग;
            स्वभाव: {
              सचे ((ch1 >= 0xE1u && ch1 <= 0xECu) ||
                  (ch1 >= 0xEEu && ch1 <= 0xEFu)) {
                सचे (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
                  पटिनिवत्त false;
                }
              } अञ्ञथा {
                पटिनिवत्त false;
              }
            } भङ्ग;
          }
          // validate byte 3
          सचे (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
            पटिनिवत्त false;
          }
        } भङ्ग;  // 3
        पक्ष 4: {
          auto ch1 = s[idx];
          विकल्प (ch1) {
            पक्ष 0xF0u: {
              सचे (++idx >= len || s[idx] < 0x90u || s[idx] > 0xBFu) {
                पटिनिवत्त false;
              }
            } भङ्ग;
            पक्ष 0xF4u: {
              सचे (++idx >= len || s[idx] < 0x80u || s[idx] > 0x8Fu) {
                पटिनिवत्त false;
              }
            } भङ्ग;
            स्वभाव: {
              सचे (ch1 >= 0xF1u && ch1 <= 0xF3u) {
                सचे (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
                  पटिनिवत्त false;
                }
              } अञ्ञथा {
                पटिनिवत्त false;
              }
            } भङ्ग;
          }
          // validate bytes 3 and 4
          size_t stop = idx + 2;
          याव (idx < stop) {
            सचे (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
              पटिनिवत्त false;
            }
          }
        } भङ्ग;  // 4
        स्वभाव:
          // no chars longer than 4
          पटिनिवत्त false;
      }  // switch bytes
      ++idx;
      ++utf8_len;
    } अञ्ञथा {
      पटिनिवत्त false;
    }
  }
  // End index must match
  // the end of the last byte sequence.
  सचे (idx != len) {
    पटिनिवत्त false;
  }
  utf8_chars = utf8_len;
  पटिनिवत्त true;
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
  अत्थाय (wchar_t wc : wstr) {
    char32_t cp = static_cast<char32_t>(wc);
    सचे (cp >= 0xD800 && cp <= 0xDFFF) {
      ORT_THROW("Invalid Unicode surrogate codepoint U+", std::hex, static_cast<uint32_t>(cp));
    }
    सचे (cp <= 0x7F) {
      result += 1;
    } अञ्ञथा सचे (cp <= 0x7FF) {
      result += 2;
    } अञ्ञथा सचे (cp <= 0xFFFF) {
      result += 3;
    } अञ्ञथा सचे (cp <= 0x10FFFF) {
      result += 4;
    } अञ्ञथा {
      ORT_THROW("Invalid Unicode codepoint U+", std::hex, static_cast<uint32_t>(cp));
    }
  }
  पटिनिवत्त result;
}

/// Convert a wide string to UTF-8, writing into a pre-allocated std::string.
/// The string is resized to the actual number of bytes written.
अन्तःस्थ Status WideToUtf8(ध्रुव std::wstring& wstr, std::string& str) {
  सचे (wstr.empty()) {
    str.clear();
    पटिनिवत्त Status::OK();
  }

  वर्ण* dest = str.data();
  वर्ण* dest_end = dest + str.size();

  अत्थाय (wchar_t wc : wstr) {
    char32_t cp = static_cast<char32_t>(wc);
    सचे (cp >= 0xD800 && cp <= 0xDFFF) {
      पटिनिवत्त ORT_MAKE_STATUS(ONNXRUNTIME, FAIL,
                             "Invalid Unicode surrogate codepoint during UTF-8 conversion");
    }
    सचे (cp <= 0x7F) {
      ध्रुव size_t remaining = static_cast<size_t>(dest_end - dest);
      सचे (remaining < 1) {
        पटिनिवत्त ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<वर्ण>(cp);
    } अञ्ञथा सचे (cp <= 0x7FF) {
      ध्रुव size_t remaining = static_cast<size_t>(dest_end - dest);
      सचे (remaining < 2) {
        पटिनिवत्त ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<वर्ण>(0xC0 | (cp >> 6));
      *dest++ = static_cast<वर्ण>(0x80 | (cp & 0x3F));
    } अञ्ञथा सचे (cp <= 0xFFFF) {
      ध्रुव size_t remaining = static_cast<size_t>(dest_end - dest);
      सचे (remaining < 3) {
        पटिनिवत्त ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<वर्ण>(0xE0 | (cp >> 12));
      *dest++ = static_cast<वर्ण>(0x80 | ((cp >> 6) & 0x3F));
      *dest++ = static_cast<वर्ण>(0x80 | (cp & 0x3F));
    } अञ्ञथा सचे (cp <= 0x10FFFF) {
      ध्रुव size_t remaining = static_cast<size_t>(dest_end - dest);
      सचे (remaining < 4) {
        पटिनिवत्त ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<वर्ण>(0xF0 | (cp >> 18));
      *dest++ = static_cast<वर्ण>(0x80 | ((cp >> 12) & 0x3F));
      *dest++ = static_cast<वर्ण>(0x80 | ((cp >> 6) & 0x3F));
      *dest++ = static_cast<वर्ण>(0x80 | (cp & 0x3F));
    } अञ्ञथा {
      पटिनिवत्त ORT_MAKE_STATUS(ONNXRUNTIME, FAIL,
                             "Invalid Unicode codepoint during UTF-8 conversion");
    }
  }

  str.resize(static_cast<size_t>(dest - str.data()));
  पटिनिवत्त Status::OK();
}

/// Convert a UTF-8 string to a wide string, writing into a pre-allocated std::wstring.
/// The wstring is resized to the actual number of characters written.
/// Validates continuation bytes and rejects overlong encodings and surrogates.
अन्तःस्थ Status Utf8ToWide(ध्रुव std::string& str, std::wstring& wstr) {
  सचे (str.empty()) {
    wstr.clear();
    पटिनिवत्त Status::OK();
  }

  सचे (wstr.size() < str.size()) {
    wstr.resize(str.size());
  }

  ध्रुव auto* src = reinterpret_cast<ध्रुव unsigned वर्ण*>(str.data());
  ध्रुव auto* src_end = src + str.size();
  wchar_t* dest = wstr.data();

  याव (src < src_end) {
    char32_t cp = 0;
    size_t byte_len = 0;

    सचे ((*src & 0x80) == 0) {
      cp = *src;
      byte_len = 1;
    } अञ्ञथा सचे ((*src & 0xE0) == 0xC0) {
      byte_len = 2;
      सचे (static_cast<size_t>(src_end - src) < 2) {
        पटिनिवत्त ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      सचे ((src[1] & 0xC0) != 0x80) {
        पटिनिवत्त ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x1F) << 6) |
           static_cast<char32_t>(src[1] & 0x3F);
      // Reject overlong encoding (must be >= 0x80 for 2-byte)
      सचे (cp < 0x80) {
        पटिनिवत्त ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
    } अञ्ञथा सचे ((*src & 0xF0) == 0xE0) {
      byte_len = 3;
      सचे (static_cast<size_t>(src_end - src) < 3) {
        पटिनिवत्त ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      सचे ((src[1] & 0xC0) != 0x80 || (src[2] & 0xC0) != 0x80) {
        पटिनिवत्त ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x0F) << 12) |
           (static_cast<char32_t>(src[1] & 0x3F) << 6) |
           static_cast<char32_t>(src[2] & 0x3F);
      // Reject overlong encoding (must be >= 0x800 for 3-byte)
      सचे (cp < 0x800) {
        पटिनिवत्त ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
      // Reject UTF-16 surrogates (U+D800..U+DFFF)
      सचे (cp >= 0xD800 && cp <= 0xDFFF) {
        पटिनिवत्त ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8: surrogate codepoint");
      }
    } अञ्ञथा सचे ((*src & 0xF8) == 0xF0) {
      byte_len = 4;
      सचे (static_cast<size_t>(src_end - src) < 4) {
        पटिनिवत्त ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      सचे ((src[1] & 0xC0) != 0x80 || (src[2] & 0xC0) != 0x80 || (src[3] & 0xC0) != 0x80) {
        पटिनिवत्त ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x07) << 18) |
           (static_cast<char32_t>(src[1] & 0x3F) << 12) |
           (static_cast<char32_t>(src[2] & 0x3F) << 6) |
           static_cast<char32_t>(src[3] & 0x3F);
      // Reject overlong encoding (must be >= 0x10000 for 4-byte)
      सचे (cp < 0x10000) {
        पटिनिवत्त ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
      // Reject codepoints beyond Unicode range
      सचे (cp > 0x10FFFF) {
        पटिनिवत्त ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8: codepoint beyond U+10FFFF");
      }
    } अञ्ञथा {
      पटिनिवत्त ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8 lead byte");
    }

    *dest++ = static_cast<wchar_t>(cp);
    src += byte_len;
  }

  wstr.resize(static_cast<size_t>(dest - wstr.data()));
  पटिनिवत्त Status::OK();
}

/// Convenience: convert UTF-8 string to wstring (throws on error).
अन्तःस्थ std::wstring Utf8ToWideString(ध्रुव std::string& s) {
  // UTF-8 byte count is an upper bound on wchar_t count
  std::wstring result;
  result.resize(s.size());
  ORT_THROW_IF_ERROR(Utf8ToWide(s, result));
  पटिनिवत्त result;
}

#endif  // !_WIN32

}  // namespace utf8_util
}  // namespace onnxruntime
