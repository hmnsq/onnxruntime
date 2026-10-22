// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#અન્તર્ભાવ "core/common/common.h"

#અન્તર્ભાવ <string>

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
અંતઃરેખ bool utf8_bytes(unsigned અક્ષર ch, size_t& len) {
  જો ((ch & 0x80) == 0) {
    len = 1;
    પાછા true;
  }
  જો ((ch & 0xE0) == 0xC0) {
    len = 2;
    પાછા true;
  }
  જો ((ch & 0xF0) == 0xE0) {
    len = 3;
    પાછા true;
  }
  જો ((ch & 0xF8) == 0xF0) {
    len = 4;
    પાછા true;
  }
  પાછા false;
}

// Computes length of the utf8 string in characters
અંતઃરેખ bool utf8_len(અચલ unsigned અક્ષર* s, size_t bytes, size_t& len) {
  size_t result = 0;
  જ્યારે (bytes > 0) {
    size_t char_bytes = 0;
    bool valid = utf8_bytes(*s, char_bytes);
    જો (!valid || bytes < char_bytes) {
      પાછા false;
    }
    bytes -= char_bytes;
    s += char_bytes;
    ++result;
  }
  len = result;
  પાછા true;
}

અંતઃરેખ bool utf8_validate(અચલ unsigned અક્ષર* s, size_t len, size_t& utf8_chars) {
  size_t utf8_len = 0;
  size_t idx = 0;
  જ્યારે (idx < len) {
    size_t bytes = 0;
    auto ch = s[idx];
    જો (utf8_bytes(ch, bytes)) {
      પસંદગી (bytes) {
        સ્થિતિ 1:
          તોડો;
        સ્થિતિ 2: {
          // Reject overlong 2-byte sequences. Valid Unicode 2-byte encodings
          // start at U+0080, so lead bytes 0xC0 and 0xC1 are invalid.
          જો (ch < 0xC2u) {
            પાછા false;
          }
          જો (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
            પાછા false;
          }
        } તોડો;  // 2
        સ્થિતિ 3: {
          auto ch1 = s[idx];
          પસંદગી (ch1) {
            સ્થિતિ 0xE0u:
              જો (++idx >= len || s[idx] < 0xA0u || s[idx] > 0xBFu) {
                પાછા false;
              }
              તોડો;
            સ્થિતિ 0xEDu:
              જો (++idx >= len || s[idx] < 0x80u || s[idx] > 0x9Fu) {
                પાછા false;
              }
              તોડો;
            મૂળભૂત: {
              જો ((ch1 >= 0xE1u && ch1 <= 0xECu) ||
                  (ch1 >= 0xEEu && ch1 <= 0xEFu)) {
                જો (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
                  પાછા false;
                }
              } નહીંતર {
                પાછા false;
              }
            } તોડો;
          }
          // validate byte 3
          જો (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
            પાછા false;
          }
        } તોડો;  // 3
        સ્થિતિ 4: {
          auto ch1 = s[idx];
          પસંદગી (ch1) {
            સ્થિતિ 0xF0u: {
              જો (++idx >= len || s[idx] < 0x90u || s[idx] > 0xBFu) {
                પાછા false;
              }
            } તોડો;
            સ્થિતિ 0xF4u: {
              જો (++idx >= len || s[idx] < 0x80u || s[idx] > 0x8Fu) {
                પાછા false;
              }
            } તોડો;
            મૂળભૂત: {
              જો (ch1 >= 0xF1u && ch1 <= 0xF3u) {
                જો (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
                  પાછા false;
                }
              } નહીંતર {
                પાછા false;
              }
            } તોડો;
          }
          // validate bytes 3 and 4
          size_t stop = idx + 2;
          જ્યારે (idx < stop) {
            જો (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
              પાછા false;
            }
          }
        } તોડો;  // 4
        મૂળભૂત:
          // no chars longer than 4
          પાછા false;
      }  // switch bytes
      ++idx;
      ++utf8_len;
    } નહીંતર {
      પાછા false;
    }
  }
  // End index must match
  // the end of the last byte sequence.
  જો (idx != len) {
    પાછા false;
  }
  utf8_chars = utf8_len;
  પાછા true;
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

static_assert(કદ(wchar_t) >= 4,
              "Non-Windows UTF-8/wchar_t conversion helpers require wchar_t to be at least 32 bits.");

/// Compute the number of UTF-8 bytes required to encode a wide string.
અંતઃરેખ size_t WideToUtf8RequiredSize(અચલ std::wstring& wstr) {
  size_t result = 0;
  માટે (wchar_t wc : wstr) {
    char32_t cp = static_cast<char32_t>(wc);
    જો (cp >= 0xD800 && cp <= 0xDFFF) {
      ORT_THROW("Invalid Unicode surrogate codepoint U+", std::hex, static_cast<uint32_t>(cp));
    }
    જો (cp <= 0x7F) {
      result += 1;
    } નહીંતર જો (cp <= 0x7FF) {
      result += 2;
    } નહીંતર જો (cp <= 0xFFFF) {
      result += 3;
    } નહીંતર જો (cp <= 0x10FFFF) {
      result += 4;
    } નહીંતર {
      ORT_THROW("Invalid Unicode codepoint U+", std::hex, static_cast<uint32_t>(cp));
    }
  }
  પાછા result;
}

/// Convert a wide string to UTF-8, writing into a pre-allocated std::string.
/// The string is resized to the actual number of bytes written.
અંતઃરેખ Status WideToUtf8(અચલ std::wstring& wstr, std::string& str) {
  જો (wstr.empty()) {
    str.clear();
    પાછા Status::OK();
  }

  અક્ષર* dest = str.data();
  અક્ષર* dest_end = dest + str.size();

  માટે (wchar_t wc : wstr) {
    char32_t cp = static_cast<char32_t>(wc);
    જો (cp >= 0xD800 && cp <= 0xDFFF) {
      પાછા ORT_MAKE_STATUS(ONNXRUNTIME, FAIL,
                             "Invalid Unicode surrogate codepoint during UTF-8 conversion");
    }
    જો (cp <= 0x7F) {
      અચલ size_t remaining = static_cast<size_t>(dest_end - dest);
      જો (remaining < 1) {
        પાછા ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<અક્ષર>(cp);
    } નહીંતર જો (cp <= 0x7FF) {
      અચલ size_t remaining = static_cast<size_t>(dest_end - dest);
      જો (remaining < 2) {
        પાછા ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<અક્ષર>(0xC0 | (cp >> 6));
      *dest++ = static_cast<અક્ષર>(0x80 | (cp & 0x3F));
    } નહીંતર જો (cp <= 0xFFFF) {
      અચલ size_t remaining = static_cast<size_t>(dest_end - dest);
      જો (remaining < 3) {
        પાછા ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<અક્ષર>(0xE0 | (cp >> 12));
      *dest++ = static_cast<અક્ષર>(0x80 | ((cp >> 6) & 0x3F));
      *dest++ = static_cast<અક્ષર>(0x80 | (cp & 0x3F));
    } નહીંતર જો (cp <= 0x10FFFF) {
      અચલ size_t remaining = static_cast<size_t>(dest_end - dest);
      જો (remaining < 4) {
        પાછા ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<અક્ષર>(0xF0 | (cp >> 18));
      *dest++ = static_cast<અક્ષર>(0x80 | ((cp >> 12) & 0x3F));
      *dest++ = static_cast<અક્ષર>(0x80 | ((cp >> 6) & 0x3F));
      *dest++ = static_cast<અક્ષર>(0x80 | (cp & 0x3F));
    } નહીંતર {
      પાછા ORT_MAKE_STATUS(ONNXRUNTIME, FAIL,
                             "Invalid Unicode codepoint during UTF-8 conversion");
    }
  }

  str.resize(static_cast<size_t>(dest - str.data()));
  પાછા Status::OK();
}

/// Convert a UTF-8 string to a wide string, writing into a pre-allocated std::wstring.
/// The wstring is resized to the actual number of characters written.
/// Validates continuation bytes and rejects overlong encodings and surrogates.
અંતઃરેખ Status Utf8ToWide(અચલ std::string& str, std::wstring& wstr) {
  જો (str.empty()) {
    wstr.clear();
    પાછા Status::OK();
  }

  જો (wstr.size() < str.size()) {
    wstr.resize(str.size());
  }

  અચલ auto* src = reinterpret_cast<અચલ unsigned અક્ષર*>(str.data());
  અચલ auto* src_end = src + str.size();
  wchar_t* dest = wstr.data();

  જ્યારે (src < src_end) {
    char32_t cp = 0;
    size_t byte_len = 0;

    જો ((*src & 0x80) == 0) {
      cp = *src;
      byte_len = 1;
    } નહીંતર જો ((*src & 0xE0) == 0xC0) {
      byte_len = 2;
      જો (static_cast<size_t>(src_end - src) < 2) {
        પાછા ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      જો ((src[1] & 0xC0) != 0x80) {
        પાછા ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x1F) << 6) |
           static_cast<char32_t>(src[1] & 0x3F);
      // Reject overlong encoding (must be >= 0x80 for 2-byte)
      જો (cp < 0x80) {
        પાછા ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
    } નહીંતર જો ((*src & 0xF0) == 0xE0) {
      byte_len = 3;
      જો (static_cast<size_t>(src_end - src) < 3) {
        પાછા ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      જો ((src[1] & 0xC0) != 0x80 || (src[2] & 0xC0) != 0x80) {
        પાછા ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x0F) << 12) |
           (static_cast<char32_t>(src[1] & 0x3F) << 6) |
           static_cast<char32_t>(src[2] & 0x3F);
      // Reject overlong encoding (must be >= 0x800 for 3-byte)
      જો (cp < 0x800) {
        પાછા ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
      // Reject UTF-16 surrogates (U+D800..U+DFFF)
      જો (cp >= 0xD800 && cp <= 0xDFFF) {
        પાછા ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8: surrogate codepoint");
      }
    } નહીંતર જો ((*src & 0xF8) == 0xF0) {
      byte_len = 4;
      જો (static_cast<size_t>(src_end - src) < 4) {
        પાછા ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      જો ((src[1] & 0xC0) != 0x80 || (src[2] & 0xC0) != 0x80 || (src[3] & 0xC0) != 0x80) {
        પાછા ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x07) << 18) |
           (static_cast<char32_t>(src[1] & 0x3F) << 12) |
           (static_cast<char32_t>(src[2] & 0x3F) << 6) |
           static_cast<char32_t>(src[3] & 0x3F);
      // Reject overlong encoding (must be >= 0x10000 for 4-byte)
      જો (cp < 0x10000) {
        પાછા ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
      // Reject codepoints beyond Unicode range
      જો (cp > 0x10FFFF) {
        પાછા ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8: codepoint beyond U+10FFFF");
      }
    } નહીંતર {
      પાછા ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8 lead byte");
    }

    *dest++ = static_cast<wchar_t>(cp);
    src += byte_len;
  }

  wstr.resize(static_cast<size_t>(dest - wstr.data()));
  પાછા Status::OK();
}

/// Convenience: convert UTF-8 string to wstring (throws on error).
અંતઃરેખ std::wstring Utf8ToWideString(અચલ std::string& s) {
  // UTF-8 byte count is an upper bound on wchar_t count
  std::wstring result;
  result.resize(s.size());
  ORT_THROW_IF_ERROR(Utf8ToWide(s, result));
  પાછા result;
}

#endif  // !_WIN32

}  // namespace utf8_util
}  // namespace onnxruntime
