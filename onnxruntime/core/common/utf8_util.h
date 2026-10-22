// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#شامل "core/common/common.h"

#شامل <string>

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
درونخطی bool utf8_bytes(unsigned نویسه ch, size_t& len) {
  اگر ((ch & 0x80) == 0) {
    len = 1;
    بازگشت true;
  }
  اگر ((ch & 0xE0) == 0xC0) {
    len = 2;
    بازگشت true;
  }
  اگر ((ch & 0xF0) == 0xE0) {
    len = 3;
    بازگشت true;
  }
  اگر ((ch & 0xF8) == 0xF0) {
    len = 4;
    بازگشت true;
  }
  بازگشت false;
}

// Computes length of the utf8 string in characters
درونخطی bool utf8_len(ثابت unsigned نویسه* s, size_t bytes, size_t& len) {
  size_t result = 0;
  وقتی (bytes > 0) {
    size_t char_bytes = 0;
    bool valid = utf8_bytes(*s, char_bytes);
    اگر (!valid || bytes < char_bytes) {
      بازگشت false;
    }
    bytes -= char_bytes;
    s += char_bytes;
    ++result;
  }
  len = result;
  بازگشت true;
}

درونخطی bool utf8_validate(ثابت unsigned نویسه* s, size_t len, size_t& utf8_chars) {
  size_t utf8_len = 0;
  size_t idx = 0;
  وقتی (idx < len) {
    size_t bytes = 0;
    auto ch = s[idx];
    اگر (utf8_bytes(ch, bytes)) {
      انتخاب (bytes) {
        حالت 1:
          بشکن;
        حالت 2: {
          // Reject overlong 2-byte sequences. Valid Unicode 2-byte encodings
          // start at U+0080, so lead bytes 0xC0 and 0xC1 are invalid.
          اگر (ch < 0xC2u) {
            بازگشت false;
          }
          اگر (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
            بازگشت false;
          }
        } بشکن;  // 2
        حالت 3: {
          auto ch1 = s[idx];
          انتخاب (ch1) {
            حالت 0xE0u:
              اگر (++idx >= len || s[idx] < 0xA0u || s[idx] > 0xBFu) {
                بازگشت false;
              }
              بشکن;
            حالت 0xEDu:
              اگر (++idx >= len || s[idx] < 0x80u || s[idx] > 0x9Fu) {
                بازگشت false;
              }
              بشکن;
            پیشفرض: {
              اگر ((ch1 >= 0xE1u && ch1 <= 0xECu) ||
                  (ch1 >= 0xEEu && ch1 <= 0xEFu)) {
                اگر (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
                  بازگشت false;
                }
              } وگرنه {
                بازگشت false;
              }
            } بشکن;
          }
          // validate byte 3
          اگر (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
            بازگشت false;
          }
        } بشکن;  // 3
        حالت 4: {
          auto ch1 = s[idx];
          انتخاب (ch1) {
            حالت 0xF0u: {
              اگر (++idx >= len || s[idx] < 0x90u || s[idx] > 0xBFu) {
                بازگشت false;
              }
            } بشکن;
            حالت 0xF4u: {
              اگر (++idx >= len || s[idx] < 0x80u || s[idx] > 0x8Fu) {
                بازگشت false;
              }
            } بشکن;
            پیشفرض: {
              اگر (ch1 >= 0xF1u && ch1 <= 0xF3u) {
                اگر (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
                  بازگشت false;
                }
              } وگرنه {
                بازگشت false;
              }
            } بشکن;
          }
          // validate bytes 3 and 4
          size_t stop = idx + 2;
          وقتی (idx < stop) {
            اگر (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
              بازگشت false;
            }
          }
        } بشکن;  // 4
        پیشفرض:
          // no chars longer than 4
          بازگشت false;
      }  // switch bytes
      ++idx;
      ++utf8_len;
    } وگرنه {
      بازگشت false;
    }
  }
  // End index must match
  // the end of the last byte sequence.
  اگر (idx != len) {
    بازگشت false;
  }
  utf8_chars = utf8_len;
  بازگشت true;
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

static_assert(اندازه(wchar_t) >= 4,
              "Non-Windows UTF-8/wchar_t conversion helpers require wchar_t to be at least 32 bits.");

/// Compute the number of UTF-8 bytes required to encode a wide string.
درونخطی size_t WideToUtf8RequiredSize(ثابت std::wstring& wstr) {
  size_t result = 0;
  برای (wchar_t wc : wstr) {
    char32_t cp = static_cast<char32_t>(wc);
    اگر (cp >= 0xD800 && cp <= 0xDFFF) {
      ORT_THROW("Invalid Unicode surrogate codepoint U+", std::hex, static_cast<uint32_t>(cp));
    }
    اگر (cp <= 0x7F) {
      result += 1;
    } وگرنه اگر (cp <= 0x7FF) {
      result += 2;
    } وگرنه اگر (cp <= 0xFFFF) {
      result += 3;
    } وگرنه اگر (cp <= 0x10FFFF) {
      result += 4;
    } وگرنه {
      ORT_THROW("Invalid Unicode codepoint U+", std::hex, static_cast<uint32_t>(cp));
    }
  }
  بازگشت result;
}

/// Convert a wide string to UTF-8, writing into a pre-allocated std::string.
/// The string is resized to the actual number of bytes written.
درونخطی Status WideToUtf8(ثابت std::wstring& wstr, std::string& str) {
  اگر (wstr.empty()) {
    str.clear();
    بازگشت Status::OK();
  }

  نویسه* dest = str.data();
  نویسه* dest_end = dest + str.size();

  برای (wchar_t wc : wstr) {
    char32_t cp = static_cast<char32_t>(wc);
    اگر (cp >= 0xD800 && cp <= 0xDFFF) {
      بازگشت ORT_MAKE_STATUS(ONNXRUNTIME, FAIL,
                             "Invalid Unicode surrogate codepoint during UTF-8 conversion");
    }
    اگر (cp <= 0x7F) {
      ثابت size_t remaining = static_cast<size_t>(dest_end - dest);
      اگر (remaining < 1) {
        بازگشت ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<نویسه>(cp);
    } وگرنه اگر (cp <= 0x7FF) {
      ثابت size_t remaining = static_cast<size_t>(dest_end - dest);
      اگر (remaining < 2) {
        بازگشت ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<نویسه>(0xC0 | (cp >> 6));
      *dest++ = static_cast<نویسه>(0x80 | (cp & 0x3F));
    } وگرنه اگر (cp <= 0xFFFF) {
      ثابت size_t remaining = static_cast<size_t>(dest_end - dest);
      اگر (remaining < 3) {
        بازگشت ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<نویسه>(0xE0 | (cp >> 12));
      *dest++ = static_cast<نویسه>(0x80 | ((cp >> 6) & 0x3F));
      *dest++ = static_cast<نویسه>(0x80 | (cp & 0x3F));
    } وگرنه اگر (cp <= 0x10FFFF) {
      ثابت size_t remaining = static_cast<size_t>(dest_end - dest);
      اگر (remaining < 4) {
        بازگشت ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<نویسه>(0xF0 | (cp >> 18));
      *dest++ = static_cast<نویسه>(0x80 | ((cp >> 12) & 0x3F));
      *dest++ = static_cast<نویسه>(0x80 | ((cp >> 6) & 0x3F));
      *dest++ = static_cast<نویسه>(0x80 | (cp & 0x3F));
    } وگرنه {
      بازگشت ORT_MAKE_STATUS(ONNXRUNTIME, FAIL,
                             "Invalid Unicode codepoint during UTF-8 conversion");
    }
  }

  str.resize(static_cast<size_t>(dest - str.data()));
  بازگشت Status::OK();
}

/// Convert a UTF-8 string to a wide string, writing into a pre-allocated std::wstring.
/// The wstring is resized to the actual number of characters written.
/// Validates continuation bytes and rejects overlong encodings and surrogates.
درونخطی Status Utf8ToWide(ثابت std::string& str, std::wstring& wstr) {
  اگر (str.empty()) {
    wstr.clear();
    بازگشت Status::OK();
  }

  اگر (wstr.size() < str.size()) {
    wstr.resize(str.size());
  }

  ثابت auto* src = reinterpret_cast<ثابت unsigned نویسه*>(str.data());
  ثابت auto* src_end = src + str.size();
  wchar_t* dest = wstr.data();

  وقتی (src < src_end) {
    char32_t cp = 0;
    size_t byte_len = 0;

    اگر ((*src & 0x80) == 0) {
      cp = *src;
      byte_len = 1;
    } وگرنه اگر ((*src & 0xE0) == 0xC0) {
      byte_len = 2;
      اگر (static_cast<size_t>(src_end - src) < 2) {
        بازگشت ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      اگر ((src[1] & 0xC0) != 0x80) {
        بازگشت ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x1F) << 6) |
           static_cast<char32_t>(src[1] & 0x3F);
      // Reject overlong encoding (must be >= 0x80 for 2-byte)
      اگر (cp < 0x80) {
        بازگشت ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
    } وگرنه اگر ((*src & 0xF0) == 0xE0) {
      byte_len = 3;
      اگر (static_cast<size_t>(src_end - src) < 3) {
        بازگشت ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      اگر ((src[1] & 0xC0) != 0x80 || (src[2] & 0xC0) != 0x80) {
        بازگشت ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x0F) << 12) |
           (static_cast<char32_t>(src[1] & 0x3F) << 6) |
           static_cast<char32_t>(src[2] & 0x3F);
      // Reject overlong encoding (must be >= 0x800 for 3-byte)
      اگر (cp < 0x800) {
        بازگشت ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
      // Reject UTF-16 surrogates (U+D800..U+DFFF)
      اگر (cp >= 0xD800 && cp <= 0xDFFF) {
        بازگشت ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8: surrogate codepoint");
      }
    } وگرنه اگر ((*src & 0xF8) == 0xF0) {
      byte_len = 4;
      اگر (static_cast<size_t>(src_end - src) < 4) {
        بازگشت ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      اگر ((src[1] & 0xC0) != 0x80 || (src[2] & 0xC0) != 0x80 || (src[3] & 0xC0) != 0x80) {
        بازگشت ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x07) << 18) |
           (static_cast<char32_t>(src[1] & 0x3F) << 12) |
           (static_cast<char32_t>(src[2] & 0x3F) << 6) |
           static_cast<char32_t>(src[3] & 0x3F);
      // Reject overlong encoding (must be >= 0x10000 for 4-byte)
      اگر (cp < 0x10000) {
        بازگشت ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
      // Reject codepoints beyond Unicode range
      اگر (cp > 0x10FFFF) {
        بازگشت ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8: codepoint beyond U+10FFFF");
      }
    } وگرنه {
      بازگشت ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8 lead byte");
    }

    *dest++ = static_cast<wchar_t>(cp);
    src += byte_len;
  }

  wstr.resize(static_cast<size_t>(dest - wstr.data()));
  بازگشت Status::OK();
}

/// Convenience: convert UTF-8 string to wstring (throws on error).
درونخطی std::wstring Utf8ToWideString(ثابت std::string& s) {
  // UTF-8 byte count is an upper bound on wchar_t count
  std::wstring result;
  result.resize(s.size());
  ORT_THROW_IF_ERROR(Utf8ToWide(s, result));
  بازگشت result;
}

#endif  // !_WIN32

}  // namespace utf8_util
}  // namespace onnxruntime
