// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#அந்தர்பாவ "core/common/common.h"

#அந்தர்பாவ <string>

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
உள்வரி bool utf8_bytes(unsigned எழுத்து ch, size_t& len) {
  எனில் ((ch & 0x80) == 0) {
    len = 1;
    திரும்பு true;
  }
  எனில் ((ch & 0xE0) == 0xC0) {
    len = 2;
    திரும்பு true;
  }
  எனில் ((ch & 0xF0) == 0xE0) {
    len = 3;
    திரும்பு true;
  }
  எனில் ((ch & 0xF8) == 0xF0) {
    len = 4;
    திரும்பு true;
  }
  திரும்பு false;
}

// Computes length of the utf8 string in characters
உள்வரி bool utf8_len(மாறிலி unsigned எழுத்து* s, size_t bytes, size_t& len) {
  size_t result = 0;
  போது (bytes > 0) {
    size_t char_bytes = 0;
    bool valid = utf8_bytes(*s, char_bytes);
    எனில் (!valid || bytes < char_bytes) {
      திரும்பு false;
    }
    bytes -= char_bytes;
    s += char_bytes;
    ++result;
  }
  len = result;
  திரும்பு true;
}

உள்வரி bool utf8_validate(மாறிலி unsigned எழுத்து* s, size_t len, size_t& utf8_chars) {
  size_t utf8_len = 0;
  size_t idx = 0;
  போது (idx < len) {
    size_t bytes = 0;
    auto ch = s[idx];
    எனில் (utf8_bytes(ch, bytes)) {
      தேர்வு (bytes) {
        நிலை 1:
          நிறுத்து;
        நிலை 2: {
          // Reject overlong 2-byte sequences. Valid Unicode 2-byte encodings
          // start at U+0080, so lead bytes 0xC0 and 0xC1 are invalid.
          எனில் (ch < 0xC2u) {
            திரும்பு false;
          }
          எனில் (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
            திரும்பு false;
          }
        } நிறுத்து;  // 2
        நிலை 3: {
          auto ch1 = s[idx];
          தேர்வு (ch1) {
            நிலை 0xE0u:
              எனில் (++idx >= len || s[idx] < 0xA0u || s[idx] > 0xBFu) {
                திரும்பு false;
              }
              நிறுத்து;
            நிலை 0xEDu:
              எனில் (++idx >= len || s[idx] < 0x80u || s[idx] > 0x9Fu) {
                திரும்பு false;
              }
              நிறுத்து;
            இயல்பு: {
              எனில் ((ch1 >= 0xE1u && ch1 <= 0xECu) ||
                  (ch1 >= 0xEEu && ch1 <= 0xEFu)) {
                எனில் (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
                  திரும்பு false;
                }
              } இல்லையெனில் {
                திரும்பு false;
              }
            } நிறுத்து;
          }
          // validate byte 3
          எனில் (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
            திரும்பு false;
          }
        } நிறுத்து;  // 3
        நிலை 4: {
          auto ch1 = s[idx];
          தேர்வு (ch1) {
            நிலை 0xF0u: {
              எனில் (++idx >= len || s[idx] < 0x90u || s[idx] > 0xBFu) {
                திரும்பு false;
              }
            } நிறுத்து;
            நிலை 0xF4u: {
              எனில் (++idx >= len || s[idx] < 0x80u || s[idx] > 0x8Fu) {
                திரும்பு false;
              }
            } நிறுத்து;
            இயல்பு: {
              எனில் (ch1 >= 0xF1u && ch1 <= 0xF3u) {
                எனில் (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
                  திரும்பு false;
                }
              } இல்லையெனில் {
                திரும்பு false;
              }
            } நிறுத்து;
          }
          // validate bytes 3 and 4
          size_t stop = idx + 2;
          போது (idx < stop) {
            எனில் (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
              திரும்பு false;
            }
          }
        } நிறுத்து;  // 4
        இயல்பு:
          // no chars longer than 4
          திரும்பு false;
      }  // switch bytes
      ++idx;
      ++utf8_len;
    } இல்லையெனில் {
      திரும்பு false;
    }
  }
  // End index must match
  // the end of the last byte sequence.
  எனில் (idx != len) {
    திரும்பு false;
  }
  utf8_chars = utf8_len;
  திரும்பு true;
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

static_assert(அளவு(wchar_t) >= 4,
              "Non-Windows UTF-8/wchar_t conversion helpers require wchar_t to be at least 32 bits.");

/// Compute the number of UTF-8 bytes required to encode a wide string.
உள்வரி size_t WideToUtf8RequiredSize(மாறிலி std::wstring& wstr) {
  size_t result = 0;
  க்கு (wchar_t wc : wstr) {
    char32_t cp = static_cast<char32_t>(wc);
    எனில் (cp >= 0xD800 && cp <= 0xDFFF) {
      ORT_THROW("Invalid Unicode surrogate codepoint U+", std::hex, static_cast<uint32_t>(cp));
    }
    எனில் (cp <= 0x7F) {
      result += 1;
    } இல்லையெனில் எனில் (cp <= 0x7FF) {
      result += 2;
    } இல்லையெனில் எனில் (cp <= 0xFFFF) {
      result += 3;
    } இல்லையெனில் எனில் (cp <= 0x10FFFF) {
      result += 4;
    } இல்லையெனில் {
      ORT_THROW("Invalid Unicode codepoint U+", std::hex, static_cast<uint32_t>(cp));
    }
  }
  திரும்பு result;
}

/// Convert a wide string to UTF-8, writing into a pre-allocated std::string.
/// The string is resized to the actual number of bytes written.
உள்வரி Status WideToUtf8(மாறிலி std::wstring& wstr, std::string& str) {
  எனில் (wstr.empty()) {
    str.clear();
    திரும்பு Status::OK();
  }

  எழுத்து* dest = str.data();
  எழுத்து* dest_end = dest + str.size();

  க்கு (wchar_t wc : wstr) {
    char32_t cp = static_cast<char32_t>(wc);
    எனில் (cp >= 0xD800 && cp <= 0xDFFF) {
      திரும்பு ORT_MAKE_STATUS(ONNXRUNTIME, FAIL,
                             "Invalid Unicode surrogate codepoint during UTF-8 conversion");
    }
    எனில் (cp <= 0x7F) {
      மாறிலி size_t remaining = static_cast<size_t>(dest_end - dest);
      எனில் (remaining < 1) {
        திரும்பு ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<எழுத்து>(cp);
    } இல்லையெனில் எனில் (cp <= 0x7FF) {
      மாறிலி size_t remaining = static_cast<size_t>(dest_end - dest);
      எனில் (remaining < 2) {
        திரும்பு ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<எழுத்து>(0xC0 | (cp >> 6));
      *dest++ = static_cast<எழுத்து>(0x80 | (cp & 0x3F));
    } இல்லையெனில் எனில் (cp <= 0xFFFF) {
      மாறிலி size_t remaining = static_cast<size_t>(dest_end - dest);
      எனில் (remaining < 3) {
        திரும்பு ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<எழுத்து>(0xE0 | (cp >> 12));
      *dest++ = static_cast<எழுத்து>(0x80 | ((cp >> 6) & 0x3F));
      *dest++ = static_cast<எழுத்து>(0x80 | (cp & 0x3F));
    } இல்லையெனில் எனில் (cp <= 0x10FFFF) {
      மாறிலி size_t remaining = static_cast<size_t>(dest_end - dest);
      எனில் (remaining < 4) {
        திரும்பு ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<எழுத்து>(0xF0 | (cp >> 18));
      *dest++ = static_cast<எழுத்து>(0x80 | ((cp >> 12) & 0x3F));
      *dest++ = static_cast<எழுத்து>(0x80 | ((cp >> 6) & 0x3F));
      *dest++ = static_cast<எழுத்து>(0x80 | (cp & 0x3F));
    } இல்லையெனில் {
      திரும்பு ORT_MAKE_STATUS(ONNXRUNTIME, FAIL,
                             "Invalid Unicode codepoint during UTF-8 conversion");
    }
  }

  str.resize(static_cast<size_t>(dest - str.data()));
  திரும்பு Status::OK();
}

/// Convert a UTF-8 string to a wide string, writing into a pre-allocated std::wstring.
/// The wstring is resized to the actual number of characters written.
/// Validates continuation bytes and rejects overlong encodings and surrogates.
உள்வரி Status Utf8ToWide(மாறிலி std::string& str, std::wstring& wstr) {
  எனில் (str.empty()) {
    wstr.clear();
    திரும்பு Status::OK();
  }

  எனில் (wstr.size() < str.size()) {
    wstr.resize(str.size());
  }

  மாறிலி auto* src = reinterpret_cast<மாறிலி unsigned எழுத்து*>(str.data());
  மாறிலி auto* src_end = src + str.size();
  wchar_t* dest = wstr.data();

  போது (src < src_end) {
    char32_t cp = 0;
    size_t byte_len = 0;

    எனில் ((*src & 0x80) == 0) {
      cp = *src;
      byte_len = 1;
    } இல்லையெனில் எனில் ((*src & 0xE0) == 0xC0) {
      byte_len = 2;
      எனில் (static_cast<size_t>(src_end - src) < 2) {
        திரும்பு ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      எனில் ((src[1] & 0xC0) != 0x80) {
        திரும்பு ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x1F) << 6) |
           static_cast<char32_t>(src[1] & 0x3F);
      // Reject overlong encoding (must be >= 0x80 for 2-byte)
      எனில் (cp < 0x80) {
        திரும்பு ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
    } இல்லையெனில் எனில் ((*src & 0xF0) == 0xE0) {
      byte_len = 3;
      எனில் (static_cast<size_t>(src_end - src) < 3) {
        திரும்பு ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      எனில் ((src[1] & 0xC0) != 0x80 || (src[2] & 0xC0) != 0x80) {
        திரும்பு ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x0F) << 12) |
           (static_cast<char32_t>(src[1] & 0x3F) << 6) |
           static_cast<char32_t>(src[2] & 0x3F);
      // Reject overlong encoding (must be >= 0x800 for 3-byte)
      எனில் (cp < 0x800) {
        திரும்பு ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
      // Reject UTF-16 surrogates (U+D800..U+DFFF)
      எனில் (cp >= 0xD800 && cp <= 0xDFFF) {
        திரும்பு ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8: surrogate codepoint");
      }
    } இல்லையெனில் எனில் ((*src & 0xF8) == 0xF0) {
      byte_len = 4;
      எனில் (static_cast<size_t>(src_end - src) < 4) {
        திரும்பு ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      எனில் ((src[1] & 0xC0) != 0x80 || (src[2] & 0xC0) != 0x80 || (src[3] & 0xC0) != 0x80) {
        திரும்பு ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x07) << 18) |
           (static_cast<char32_t>(src[1] & 0x3F) << 12) |
           (static_cast<char32_t>(src[2] & 0x3F) << 6) |
           static_cast<char32_t>(src[3] & 0x3F);
      // Reject overlong encoding (must be >= 0x10000 for 4-byte)
      எனில் (cp < 0x10000) {
        திரும்பு ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
      // Reject codepoints beyond Unicode range
      எனில் (cp > 0x10FFFF) {
        திரும்பு ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8: codepoint beyond U+10FFFF");
      }
    } இல்லையெனில் {
      திரும்பு ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8 lead byte");
    }

    *dest++ = static_cast<wchar_t>(cp);
    src += byte_len;
  }

  wstr.resize(static_cast<size_t>(dest - wstr.data()));
  திரும்பு Status::OK();
}

/// Convenience: convert UTF-8 string to wstring (throws on error).
உள்வரி std::wstring Utf8ToWideString(மாறிலி std::string& s) {
  // UTF-8 byte count is an upper bound on wchar_t count
  std::wstring result;
  result.resize(s.size());
  ORT_THROW_IF_ERROR(Utf8ToWide(s, result));
  திரும்பு result;
}

#endif  // !_WIN32

}  // namespace utf8_util
}  // namespace onnxruntime
