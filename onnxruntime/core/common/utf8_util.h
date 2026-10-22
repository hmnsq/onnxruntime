// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#ଅନ୍ତର୍ଭାଵ "core/common/common.h"

#ଅନ୍ତର୍ଭାଵ <string>

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
ଧାଡ଼ିଭିତରେ bool utf8_bytes(unsigned ଅକ୍ଷର ch, size_t& len) {
  ଯଦି ((ch & 0x80) == 0) {
    len = 1;
    ଫେରନ୍ତୁ true;
  }
  ଯଦି ((ch & 0xE0) == 0xC0) {
    len = 2;
    ଫେରନ୍ତୁ true;
  }
  ଯଦି ((ch & 0xF0) == 0xE0) {
    len = 3;
    ଫେରନ୍ତୁ true;
  }
  ଯଦି ((ch & 0xF8) == 0xF0) {
    len = 4;
    ଫେରନ୍ତୁ true;
  }
  ଫେରନ୍ତୁ false;
}

// Computes length of the utf8 string in characters
ଧାଡ଼ିଭିତରେ bool utf8_len(ଅଚଳ unsigned ଅକ୍ଷର* s, size_t bytes, size_t& len) {
  size_t result = 0;
  ଯେତେବେଳେ (bytes > 0) {
    size_t char_bytes = 0;
    bool valid = utf8_bytes(*s, char_bytes);
    ଯଦି (!valid || bytes < char_bytes) {
      ଫେରନ୍ତୁ false;
    }
    bytes -= char_bytes;
    s += char_bytes;
    ++result;
  }
  len = result;
  ଫେରନ୍ତୁ true;
}

ଧାଡ଼ିଭିତରେ bool utf8_validate(ଅଚଳ unsigned ଅକ୍ଷର* s, size_t len, size_t& utf8_chars) {
  size_t utf8_len = 0;
  size_t idx = 0;
  ଯେତେବେଳେ (idx < len) {
    size_t bytes = 0;
    auto ch = s[idx];
    ଯଦି (utf8_bytes(ch, bytes)) {
      ବାଛ (bytes) {
        ସ୍ଥିତି 1:
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 2: {
          // Reject overlong 2-byte sequences. Valid Unicode 2-byte encodings
          // start at U+0080, so lead bytes 0xC0 and 0xC1 are invalid.
          ଯଦି (ch < 0xC2u) {
            ଫେରନ୍ତୁ false;
          }
          ଯଦି (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
            ଫେରନ୍ତୁ false;
          }
        } ଭାଙ୍ଗ;  // 2
        ସ୍ଥିତି 3: {
          auto ch1 = s[idx];
          ବାଛ (ch1) {
            ସ୍ଥିତି 0xE0u:
              ଯଦି (++idx >= len || s[idx] < 0xA0u || s[idx] > 0xBFu) {
                ଫେରନ୍ତୁ false;
              }
              ଭାଙ୍ଗ;
            ସ୍ଥିତି 0xEDu:
              ଯଦି (++idx >= len || s[idx] < 0x80u || s[idx] > 0x9Fu) {
                ଫେରନ୍ତୁ false;
              }
              ଭାଙ୍ଗ;
            ପୂର୍ବନିର୍ଦ୍ଧାରିତ: {
              ଯଦି ((ch1 >= 0xE1u && ch1 <= 0xECu) ||
                  (ch1 >= 0xEEu && ch1 <= 0xEFu)) {
                ଯଦି (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
                  ଫେରନ୍ତୁ false;
                }
              } ଅନ୍ୟଥା {
                ଫେରନ୍ତୁ false;
              }
            } ଭାଙ୍ଗ;
          }
          // validate byte 3
          ଯଦି (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
            ଫେରନ୍ତୁ false;
          }
        } ଭାଙ୍ଗ;  // 3
        ସ୍ଥିତି 4: {
          auto ch1 = s[idx];
          ବାଛ (ch1) {
            ସ୍ଥିତି 0xF0u: {
              ଯଦି (++idx >= len || s[idx] < 0x90u || s[idx] > 0xBFu) {
                ଫେରନ୍ତୁ false;
              }
            } ଭାଙ୍ଗ;
            ସ୍ଥିତି 0xF4u: {
              ଯଦି (++idx >= len || s[idx] < 0x80u || s[idx] > 0x8Fu) {
                ଫେରନ୍ତୁ false;
              }
            } ଭାଙ୍ଗ;
            ପୂର୍ବନିର୍ଦ୍ଧାରିତ: {
              ଯଦି (ch1 >= 0xF1u && ch1 <= 0xF3u) {
                ଯଦି (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
                  ଫେରନ୍ତୁ false;
                }
              } ଅନ୍ୟଥା {
                ଫେରନ୍ତୁ false;
              }
            } ଭାଙ୍ଗ;
          }
          // validate bytes 3 and 4
          size_t stop = idx + 2;
          ଯେତେବେଳେ (idx < stop) {
            ଯଦି (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
              ଫେରନ୍ତୁ false;
            }
          }
        } ଭାଙ୍ଗ;  // 4
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          // no chars longer than 4
          ଫେରନ୍ତୁ false;
      }  // switch bytes
      ++idx;
      ++utf8_len;
    } ଅନ୍ୟଥା {
      ଫେରନ୍ତୁ false;
    }
  }
  // End index must match
  // the end of the last byte sequence.
  ଯଦି (idx != len) {
    ଫେରନ୍ତୁ false;
  }
  utf8_chars = utf8_len;
  ଫେରନ୍ତୁ true;
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

static_assert(ଆକାର(wchar_t) >= 4,
              "Non-Windows UTF-8/wchar_t conversion helpers require wchar_t to be at least 32 bits.");

/// Compute the number of UTF-8 bytes required to encode a wide string.
ଧାଡ଼ିଭିତରେ size_t WideToUtf8RequiredSize(ଅଚଳ std::wstring& wstr) {
  size_t result = 0;
  ପାଇଁ (wchar_t wc : wstr) {
    char32_t cp = static_cast<char32_t>(wc);
    ଯଦି (cp >= 0xD800 && cp <= 0xDFFF) {
      ORT_THROW("Invalid Unicode surrogate codepoint U+", std::hex, static_cast<uint32_t>(cp));
    }
    ଯଦି (cp <= 0x7F) {
      result += 1;
    } ଅନ୍ୟଥା ଯଦି (cp <= 0x7FF) {
      result += 2;
    } ଅନ୍ୟଥା ଯଦି (cp <= 0xFFFF) {
      result += 3;
    } ଅନ୍ୟଥା ଯଦି (cp <= 0x10FFFF) {
      result += 4;
    } ଅନ୍ୟଥା {
      ORT_THROW("Invalid Unicode codepoint U+", std::hex, static_cast<uint32_t>(cp));
    }
  }
  ଫେରନ୍ତୁ result;
}

/// Convert a wide string to UTF-8, writing into a pre-allocated std::string.
/// The string is resized to the actual number of bytes written.
ଧାଡ଼ିଭିତରେ Status WideToUtf8(ଅଚଳ std::wstring& wstr, std::string& str) {
  ଯଦି (wstr.empty()) {
    str.clear();
    ଫେରନ୍ତୁ Status::OK();
  }

  ଅକ୍ଷର* dest = str.data();
  ଅକ୍ଷର* dest_end = dest + str.size();

  ପାଇଁ (wchar_t wc : wstr) {
    char32_t cp = static_cast<char32_t>(wc);
    ଯଦି (cp >= 0xD800 && cp <= 0xDFFF) {
      ଫେରନ୍ତୁ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL,
                             "Invalid Unicode surrogate codepoint during UTF-8 conversion");
    }
    ଯଦି (cp <= 0x7F) {
      ଅଚଳ size_t remaining = static_cast<size_t>(dest_end - dest);
      ଯଦି (remaining < 1) {
        ଫେରନ୍ତୁ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<ଅକ୍ଷର>(cp);
    } ଅନ୍ୟଥା ଯଦି (cp <= 0x7FF) {
      ଅଚଳ size_t remaining = static_cast<size_t>(dest_end - dest);
      ଯଦି (remaining < 2) {
        ଫେରନ୍ତୁ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<ଅକ୍ଷର>(0xC0 | (cp >> 6));
      *dest++ = static_cast<ଅକ୍ଷର>(0x80 | (cp & 0x3F));
    } ଅନ୍ୟଥା ଯଦି (cp <= 0xFFFF) {
      ଅଚଳ size_t remaining = static_cast<size_t>(dest_end - dest);
      ଯଦି (remaining < 3) {
        ଫେରନ୍ତୁ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<ଅକ୍ଷର>(0xE0 | (cp >> 12));
      *dest++ = static_cast<ଅକ୍ଷର>(0x80 | ((cp >> 6) & 0x3F));
      *dest++ = static_cast<ଅକ୍ଷର>(0x80 | (cp & 0x3F));
    } ଅନ୍ୟଥା ଯଦି (cp <= 0x10FFFF) {
      ଅଚଳ size_t remaining = static_cast<size_t>(dest_end - dest);
      ଯଦି (remaining < 4) {
        ଫେରନ୍ତୁ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<ଅକ୍ଷର>(0xF0 | (cp >> 18));
      *dest++ = static_cast<ଅକ୍ଷର>(0x80 | ((cp >> 12) & 0x3F));
      *dest++ = static_cast<ଅକ୍ଷର>(0x80 | ((cp >> 6) & 0x3F));
      *dest++ = static_cast<ଅକ୍ଷର>(0x80 | (cp & 0x3F));
    } ଅନ୍ୟଥା {
      ଫେରନ୍ତୁ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL,
                             "Invalid Unicode codepoint during UTF-8 conversion");
    }
  }

  str.resize(static_cast<size_t>(dest - str.data()));
  ଫେରନ୍ତୁ Status::OK();
}

/// Convert a UTF-8 string to a wide string, writing into a pre-allocated std::wstring.
/// The wstring is resized to the actual number of characters written.
/// Validates continuation bytes and rejects overlong encodings and surrogates.
ଧାଡ଼ିଭିତରେ Status Utf8ToWide(ଅଚଳ std::string& str, std::wstring& wstr) {
  ଯଦି (str.empty()) {
    wstr.clear();
    ଫେରନ୍ତୁ Status::OK();
  }

  ଯଦି (wstr.size() < str.size()) {
    wstr.resize(str.size());
  }

  ଅଚଳ auto* src = reinterpret_cast<ଅଚଳ unsigned ଅକ୍ଷର*>(str.data());
  ଅଚଳ auto* src_end = src + str.size();
  wchar_t* dest = wstr.data();

  ଯେତେବେଳେ (src < src_end) {
    char32_t cp = 0;
    size_t byte_len = 0;

    ଯଦି ((*src & 0x80) == 0) {
      cp = *src;
      byte_len = 1;
    } ଅନ୍ୟଥା ଯଦି ((*src & 0xE0) == 0xC0) {
      byte_len = 2;
      ଯଦି (static_cast<size_t>(src_end - src) < 2) {
        ଫେରନ୍ତୁ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      ଯଦି ((src[1] & 0xC0) != 0x80) {
        ଫେରନ୍ତୁ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x1F) << 6) |
           static_cast<char32_t>(src[1] & 0x3F);
      // Reject overlong encoding (must be >= 0x80 for 2-byte)
      ଯଦି (cp < 0x80) {
        ଫେରନ୍ତୁ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
    } ଅନ୍ୟଥା ଯଦି ((*src & 0xF0) == 0xE0) {
      byte_len = 3;
      ଯଦି (static_cast<size_t>(src_end - src) < 3) {
        ଫେରନ୍ତୁ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      ଯଦି ((src[1] & 0xC0) != 0x80 || (src[2] & 0xC0) != 0x80) {
        ଫେରନ୍ତୁ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x0F) << 12) |
           (static_cast<char32_t>(src[1] & 0x3F) << 6) |
           static_cast<char32_t>(src[2] & 0x3F);
      // Reject overlong encoding (must be >= 0x800 for 3-byte)
      ଯଦି (cp < 0x800) {
        ଫେରନ୍ତୁ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
      // Reject UTF-16 surrogates (U+D800..U+DFFF)
      ଯଦି (cp >= 0xD800 && cp <= 0xDFFF) {
        ଫେରନ୍ତୁ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8: surrogate codepoint");
      }
    } ଅନ୍ୟଥା ଯଦି ((*src & 0xF8) == 0xF0) {
      byte_len = 4;
      ଯଦି (static_cast<size_t>(src_end - src) < 4) {
        ଫେରନ୍ତୁ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      ଯଦି ((src[1] & 0xC0) != 0x80 || (src[2] & 0xC0) != 0x80 || (src[3] & 0xC0) != 0x80) {
        ଫେରନ୍ତୁ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x07) << 18) |
           (static_cast<char32_t>(src[1] & 0x3F) << 12) |
           (static_cast<char32_t>(src[2] & 0x3F) << 6) |
           static_cast<char32_t>(src[3] & 0x3F);
      // Reject overlong encoding (must be >= 0x10000 for 4-byte)
      ଯଦି (cp < 0x10000) {
        ଫେରନ୍ତୁ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
      // Reject codepoints beyond Unicode range
      ଯଦି (cp > 0x10FFFF) {
        ଫେରନ୍ତୁ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8: codepoint beyond U+10FFFF");
      }
    } ଅନ୍ୟଥା {
      ଫେରନ୍ତୁ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8 lead byte");
    }

    *dest++ = static_cast<wchar_t>(cp);
    src += byte_len;
  }

  wstr.resize(static_cast<size_t>(dest - wstr.data()));
  ଫେରନ୍ତୁ Status::OK();
}

/// Convenience: convert UTF-8 string to wstring (throws on error).
ଧାଡ଼ିଭିତରେ std::wstring Utf8ToWideString(ଅଚଳ std::string& s) {
  // UTF-8 byte count is an upper bound on wchar_t count
  std::wstring result;
  result.resize(s.size());
  ORT_THROW_IF_ERROR(Utf8ToWide(s, result));
  ଫେରନ୍ତୁ result;
}

#endif  // !_WIN32

}  // namespace utf8_util
}  // namespace onnxruntime
