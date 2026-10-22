// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#هدايت once

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
مضمن bool utf8_بائيٽون(بنانشان حرف ch, سائيز_t& len) {
  إذا ((ch & 0x80) == 0) {
    len = 1;
    أرجع true;
  }
  إذا ((ch & 0xE0) == 0xC0) {
    len = 2;
    أرجع true;
  }
  إذا ((ch & 0xF0) == 0xE0) {
    len = 3;
    أرجع true;
  }
  إذا ((ch & 0xF8) == 0xF0) {
    len = 4;
    أرجع true;
  }
  أرجع false;
}

// Computes length of the utf8 string in characters
مضمن bool utf8_len(ثابت بنانشان حرف* s, سائيز_t بائيٽون, سائيز_t& len) {
  سائيز_t نتيجو = 0;
  طالما (بائيٽون > 0) {
    سائيز_t char_بائيٽون = 0;
    bool valid = utf8_بائيٽون(*s, char_بائيٽون);
    إذا (!valid || بائيٽون < char_بائيٽون) {
      أرجع false;
    }
    بائيٽون -= char_بائيٽون;
    s += char_بائيٽون;
    ++نتيجو;
  }
  len = نتيجو;
  أرجع true;
}

مضمن bool utf8_validate(ثابت بنانشان حرف* s, سائيز_t len, سائيز_t& utf8_chars) {
  سائيز_t utf8_len = 0;
  سائيز_t idx = 0;
  طالما (idx < len) {
    سائيز_t بائيٽون = 0;
    خودڪار ch = s[idx];
    إذا (utf8_بائيٽون(ch, بائيٽون)) {
      بدل (بائيٽون) {
        حالة 1:
          اكسر;
        حالة 2: {
          // Reject overlong 2-byte sequences. Valid Unicode 2-byte encodings
          // start at U+0080, so lead bytes 0xC0 and 0xC1 are invalid.
          إذا (ch < 0xC2u) {
            أرجع false;
          }
          إذا (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
            أرجع false;
          }
        } اكسر;  // 2
        حالة 3: {
          خودڪار ch1 = s[idx];
          بدل (ch1) {
            حالة 0xE0u:
              إذا (++idx >= len || s[idx] < 0xA0u || s[idx] > 0xBFu) {
                أرجع false;
              }
              اكسر;
            حالة 0xEDu:
              إذا (++idx >= len || s[idx] < 0x80u || s[idx] > 0x9Fu) {
                أرجع false;
              }
              اكسر;
            افتراضي: {
              إذا ((ch1 >= 0xE1u && ch1 <= 0xECu) ||
                  (ch1 >= 0xEEu && ch1 <= 0xEFu)) {
                إذا (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
                  أرجع false;
                }
              } وإلا {
                أرجع false;
              }
            } اكسر;
          }
          // validate byte 3
          إذا (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
            أرجع false;
          }
        } اكسر;  // 3
        حالة 4: {
          خودڪار ch1 = s[idx];
          بدل (ch1) {
            حالة 0xF0u: {
              إذا (++idx >= len || s[idx] < 0x90u || s[idx] > 0xBFu) {
                أرجع false;
              }
            } اكسر;
            حالة 0xF4u: {
              إذا (++idx >= len || s[idx] < 0x80u || s[idx] > 0x8Fu) {
                أرجع false;
              }
            } اكسر;
            افتراضي: {
              إذا (ch1 >= 0xF1u && ch1 <= 0xF3u) {
                إذا (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
                  أرجع false;
                }
              } وإلا {
                أرجع false;
              }
            } اكسر;
          }
          // validate bytes 3 and 4
          سائيز_t stop = idx + 2;
          طالما (idx < stop) {
            إذا (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
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
  إذا (idx != len) {
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
#جيڪڏهن_وصف_نه_ٿيل _WIN32

static_assert(حجم(wchar_t) >= 4,
              "Non-Windows UTF-8/wchar_t conversion helpers require wchar_t to be at least 32 bits.");

/// Compute the number of UTF-8 bytes required to encode a wide string.
مضمن سائيز_t Wide_منزل_Utf8_Required_سائيز(ثابت std::wstring& wstr) {
  سائيز_t نتيجو = 0;
  لكل (wchar_t wc : wstr) {
    char32_t cp = static_cast<char32_t>(wc);
    إذا (cp >= 0xD800 && cp <= 0xDFFF) {
      ORT_THROW("Invalid Unicode surrogate codepoint U+", std::hex, static_cast<uint32_t>(cp));
    }
    إذا (cp <= 0x7F) {
      نتيجو += 1;
    } وإلا إذا (cp <= 0x7FF) {
      نتيجو += 2;
    } وإلا إذا (cp <= 0xFFFF) {
      نتيجو += 3;
    } وإلا إذا (cp <= 0x10FFFF) {
      نتيجو += 4;
    } وإلا {
      ORT_THROW("Invalid Unicode codepoint U+", std::hex, static_cast<uint32_t>(cp));
    }
  }
  أرجع نتيجو;
}

/// Convert a wide string to UTF-8, writing into a pre-allocated std::string.
/// The string is resized to the actual number of bytes written.
مضمن حالت_2 Wide_منزل_Utf8(ثابت std::wstring& wstr, std::string& str) {
  إذا (wstr.خالي_جايون()) {
    str.clear();
    أرجع حالت_2::OK();
  }

  حرف* dest = str.معلومات();
  حرف* dest_پڇاڙي = dest + str.سائيز();

  لكل (wchar_t wc : wstr) {
    char32_t cp = static_cast<char32_t>(wc);
    إذا (cp >= 0xD800 && cp <= 0xDFFF) {
      أرجع ORT_MAKE_حالت(ONNXRUNTIME, FAIL,
                             "Invalid Unicode surrogate codepoint during UTF-8 conversion");
    }
    إذا (cp <= 0x7F) {
      ثابت سائيز_t بچيل = static_cast<سائيز_t>(dest_پڇاڙي - dest);
      إذا (بچيل < 1) {
        أرجع ORT_MAKE_حالت(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<حرف>(cp);
    } وإلا إذا (cp <= 0x7FF) {
      ثابت سائيز_t بچيل = static_cast<سائيز_t>(dest_پڇاڙي - dest);
      إذا (بچيل < 2) {
        أرجع ORT_MAKE_حالت(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<حرف>(0xC0 | (cp >> 6));
      *dest++ = static_cast<حرف>(0x80 | (cp & 0x3F));
    } وإلا إذا (cp <= 0xFFFF) {
      ثابت سائيز_t بچيل = static_cast<سائيز_t>(dest_پڇاڙي - dest);
      إذا (بچيل < 3) {
        أرجع ORT_MAKE_حالت(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<حرف>(0xE0 | (cp >> 12));
      *dest++ = static_cast<حرف>(0x80 | ((cp >> 6) & 0x3F));
      *dest++ = static_cast<حرف>(0x80 | (cp & 0x3F));
    } وإلا إذا (cp <= 0x10FFFF) {
      ثابت سائيز_t بچيل = static_cast<سائيز_t>(dest_پڇاڙي - dest);
      إذا (بچيل < 4) {
        أرجع ORT_MAKE_حالت(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<حرف>(0xF0 | (cp >> 18));
      *dest++ = static_cast<حرف>(0x80 | ((cp >> 12) & 0x3F));
      *dest++ = static_cast<حرف>(0x80 | ((cp >> 6) & 0x3F));
      *dest++ = static_cast<حرف>(0x80 | (cp & 0x3F));
    } وإلا {
      أرجع ORT_MAKE_حالت(ONNXRUNTIME, FAIL,
                             "Invalid Unicode codepoint during UTF-8 conversion");
    }
  }

  str.resize(static_cast<سائيز_t>(dest - str.معلومات()));
  أرجع حالت_2::OK();
}

/// Convert a UTF-8 string to a wide string, writing into a pre-allocated std::wstring.
/// The wstring is resized to the actual number of characters written.
/// Validates continuation bytes and rejects overlong encodings and surrogates.
مضمن حالت_2 Utf8_منزل_Wide(ثابت std::string& str, std::wstring& wstr) {
  إذا (str.خالي_جايون()) {
    wstr.clear();
    أرجع حالت_2::OK();
  }

  إذا (wstr.سائيز() < str.سائيز()) {
    wstr.resize(str.سائيز());
  }

  ثابت خودڪار* src = reinterpret_cast<ثابت بنانشان حرف*>(str.معلومات());
  ثابت خودڪار* src_پڇاڙي = src + str.سائيز();
  wchar_t* dest = wstr.معلومات();

  طالما (src < src_پڇاڙي) {
    char32_t cp = 0;
    سائيز_t byte_len = 0;

    إذا ((*src & 0x80) == 0) {
      cp = *src;
      byte_len = 1;
    } وإلا إذا ((*src & 0xE0) == 0xC0) {
      byte_len = 2;
      إذا (static_cast<سائيز_t>(src_پڇاڙي - src) < 2) {
        أرجع ORT_MAKE_حالت(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      إذا ((src[1] & 0xC0) != 0x80) {
        أرجع ORT_MAKE_حالت(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x1F) << 6) |
           static_cast<char32_t>(src[1] & 0x3F);
      // Reject overlong encoding (must be >= 0x80 for 2-byte)
      إذا (cp < 0x80) {
        أرجع ORT_MAKE_حالت(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
    } وإلا إذا ((*src & 0xF0) == 0xE0) {
      byte_len = 3;
      إذا (static_cast<سائيز_t>(src_پڇاڙي - src) < 3) {
        أرجع ORT_MAKE_حالت(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      إذا ((src[1] & 0xC0) != 0x80 || (src[2] & 0xC0) != 0x80) {
        أرجع ORT_MAKE_حالت(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x0F) << 12) |
           (static_cast<char32_t>(src[1] & 0x3F) << 6) |
           static_cast<char32_t>(src[2] & 0x3F);
      // Reject overlong encoding (must be >= 0x800 for 3-byte)
      إذا (cp < 0x800) {
        أرجع ORT_MAKE_حالت(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
      // Reject UTF-16 surrogates (U+D800..U+DFFF)
      إذا (cp >= 0xD800 && cp <= 0xDFFF) {
        أرجع ORT_MAKE_حالت(ONNXRUNTIME, FAIL, "Invalid UTF-8: surrogate codepoint");
      }
    } وإلا إذا ((*src & 0xF8) == 0xF0) {
      byte_len = 4;
      إذا (static_cast<سائيز_t>(src_پڇاڙي - src) < 4) {
        أرجع ORT_MAKE_حالت(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      إذا ((src[1] & 0xC0) != 0x80 || (src[2] & 0xC0) != 0x80 || (src[3] & 0xC0) != 0x80) {
        أرجع ORT_MAKE_حالت(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x07) << 18) |
           (static_cast<char32_t>(src[1] & 0x3F) << 12) |
           (static_cast<char32_t>(src[2] & 0x3F) << 6) |
           static_cast<char32_t>(src[3] & 0x3F);
      // Reject overlong encoding (must be >= 0x10000 for 4-byte)
      إذا (cp < 0x10000) {
        أرجع ORT_MAKE_حالت(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
      // Reject codepoints beyond Unicode range
      إذا (cp > 0x10FFFF) {
        أرجع ORT_MAKE_حالت(ONNXRUNTIME, FAIL, "Invalid UTF-8: codepoint beyond U+10FFFF");
      }
    } وإلا {
      أرجع ORT_MAKE_حالت(ONNXRUNTIME, FAIL, "Invalid UTF-8 lead byte");
    }

    *dest++ = static_cast<wchar_t>(cp);
    src += byte_len;
  }

  wstr.resize(static_cast<سائيز_t>(dest - wstr.معلومات()));
  أرجع حالت_2::OK();
}

/// Convenience: convert UTF-8 string to wstring (throws on error).
مضمن std::wstring Utf8_منزل_Wide_String(ثابت std::string& s) {
  // UTF-8 byte count is an upper bound on wchar_t count
  std::wstring نتيجو;
  نتيجو.resize(s.سائيز());
  ORT_THROW_IF_ERROR(Utf8_منزل_Wide(s, نتيجو));
  أرجع نتيجو;
}

#جيڪڏهن_پورو  // !_WIN32

}  // namespace utf8_util
}  // namespace onnxruntime
