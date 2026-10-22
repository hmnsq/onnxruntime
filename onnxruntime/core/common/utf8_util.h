// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#رهنمود once

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
مضمن bool utf8_بایتها(بیعلامت حرف ch, اندازه_t& len) {
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
مضمن bool utf8_len(ثابت بیعلامت حرف* s, اندازه_t بایتها, اندازه_t& len) {
  اندازه_t نتیجه = 0;
  طالما (بایتها > 0) {
    اندازه_t char_بایتها = 0;
    bool valid = utf8_بایتها(*s, char_بایتها);
    إذا (!valid || بایتها < char_بایتها) {
      أرجع false;
    }
    بایتها -= char_بایتها;
    s += char_بایتها;
    ++نتیجه;
  }
  len = نتیجه;
  أرجع true;
}

مضمن bool utf8_validate(ثابت بیعلامت حرف* s, اندازه_t len, اندازه_t& utf8_chars) {
  اندازه_t utf8_len = 0;
  اندازه_t idx = 0;
  طالما (idx < len) {
    اندازه_t بایتها = 0;
    خودکار ch = s[idx];
    إذا (utf8_بایتها(ch, بایتها)) {
      بدل (بایتها) {
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
          خودکار ch1 = s[idx];
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
          خودکار ch1 = s[idx];
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
          اندازه_t stop = idx + 2;
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
#اگر_تعریف_نشده _WIN32

static_assert(حجم(wchar_t) >= 4,
              "Non-Windows UTF-8/wchar_t conversion helpers require wchar_t to be at least 32 bits.");

/// Compute the number of UTF-8 bytes required to encode a wide string.
مضمن اندازه_t Wide_مقصد_Utf8_Required_اندازه(ثابت std::wstring& wstr) {
  اندازه_t نتیجه = 0;
  لكل (wchar_t wc : wstr) {
    char32_t cp = static_cast<char32_t>(wc);
    إذا (cp >= 0xD800 && cp <= 0xDFFF) {
      ORT_THROW("Invalid Unicode surrogate codepoint U+", std::hex, static_cast<uint32_t>(cp));
    }
    إذا (cp <= 0x7F) {
      نتیجه += 1;
    } وإلا إذا (cp <= 0x7FF) {
      نتیجه += 2;
    } وإلا إذا (cp <= 0xFFFF) {
      نتیجه += 3;
    } وإلا إذا (cp <= 0x10FFFF) {
      نتیجه += 4;
    } وإلا {
      ORT_THROW("Invalid Unicode codepoint U+", std::hex, static_cast<uint32_t>(cp));
    }
  }
  أرجع نتیجه;
}

/// Convert a wide string to UTF-8, writing into a pre-allocated std::string.
/// The string is resized to the actual number of bytes written.
مضمن وضعیت_2 Wide_مقصد_Utf8(ثابت std::wstring& wstr, std::string& str) {
  إذا (wstr.خالی()) {
    str.clear();
    أرجع وضعیت_2::OK();
  }

  حرف* dest = str.داده();
  حرف* dest_انتها = dest + str.اندازه();

  لكل (wchar_t wc : wstr) {
    char32_t cp = static_cast<char32_t>(wc);
    إذا (cp >= 0xD800 && cp <= 0xDFFF) {
      أرجع ORT_MAKE_وضعیت(ONNXRUNTIME, FAIL,
                             "Invalid Unicode surrogate codepoint during UTF-8 conversion");
    }
    إذا (cp <= 0x7F) {
      ثابت اندازه_t مانده = static_cast<اندازه_t>(dest_انتها - dest);
      إذا (مانده < 1) {
        أرجع ORT_MAKE_وضعیت(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<حرف>(cp);
    } وإلا إذا (cp <= 0x7FF) {
      ثابت اندازه_t مانده = static_cast<اندازه_t>(dest_انتها - dest);
      إذا (مانده < 2) {
        أرجع ORT_MAKE_وضعیت(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<حرف>(0xC0 | (cp >> 6));
      *dest++ = static_cast<حرف>(0x80 | (cp & 0x3F));
    } وإلا إذا (cp <= 0xFFFF) {
      ثابت اندازه_t مانده = static_cast<اندازه_t>(dest_انتها - dest);
      إذا (مانده < 3) {
        أرجع ORT_MAKE_وضعیت(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<حرف>(0xE0 | (cp >> 12));
      *dest++ = static_cast<حرف>(0x80 | ((cp >> 6) & 0x3F));
      *dest++ = static_cast<حرف>(0x80 | (cp & 0x3F));
    } وإلا إذا (cp <= 0x10FFFF) {
      ثابت اندازه_t مانده = static_cast<اندازه_t>(dest_انتها - dest);
      إذا (مانده < 4) {
        أرجع ORT_MAKE_وضعیت(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<حرف>(0xF0 | (cp >> 18));
      *dest++ = static_cast<حرف>(0x80 | ((cp >> 12) & 0x3F));
      *dest++ = static_cast<حرف>(0x80 | ((cp >> 6) & 0x3F));
      *dest++ = static_cast<حرف>(0x80 | (cp & 0x3F));
    } وإلا {
      أرجع ORT_MAKE_وضعیت(ONNXRUNTIME, FAIL,
                             "Invalid Unicode codepoint during UTF-8 conversion");
    }
  }

  str.resize(static_cast<اندازه_t>(dest - str.داده()));
  أرجع وضعیت_2::OK();
}

/// Convert a UTF-8 string to a wide string, writing into a pre-allocated std::wstring.
/// The wstring is resized to the actual number of characters written.
/// Validates continuation bytes and rejects overlong encodings and surrogates.
مضمن وضعیت_2 Utf8_مقصد_Wide(ثابت std::string& str, std::wstring& wstr) {
  إذا (str.خالی()) {
    wstr.clear();
    أرجع وضعیت_2::OK();
  }

  إذا (wstr.اندازه() < str.اندازه()) {
    wstr.resize(str.اندازه());
  }

  ثابت خودکار* src = reinterpret_cast<ثابت بیعلامت حرف*>(str.داده());
  ثابت خودکار* src_انتها = src + str.اندازه();
  wchar_t* dest = wstr.داده();

  طالما (src < src_انتها) {
    char32_t cp = 0;
    اندازه_t byte_len = 0;

    إذا ((*src & 0x80) == 0) {
      cp = *src;
      byte_len = 1;
    } وإلا إذا ((*src & 0xE0) == 0xC0) {
      byte_len = 2;
      إذا (static_cast<اندازه_t>(src_انتها - src) < 2) {
        أرجع ORT_MAKE_وضعیت(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      إذا ((src[1] & 0xC0) != 0x80) {
        أرجع ORT_MAKE_وضعیت(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x1F) << 6) |
           static_cast<char32_t>(src[1] & 0x3F);
      // Reject overlong encoding (must be >= 0x80 for 2-byte)
      إذا (cp < 0x80) {
        أرجع ORT_MAKE_وضعیت(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
    } وإلا إذا ((*src & 0xF0) == 0xE0) {
      byte_len = 3;
      إذا (static_cast<اندازه_t>(src_انتها - src) < 3) {
        أرجع ORT_MAKE_وضعیت(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      إذا ((src[1] & 0xC0) != 0x80 || (src[2] & 0xC0) != 0x80) {
        أرجع ORT_MAKE_وضعیت(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x0F) << 12) |
           (static_cast<char32_t>(src[1] & 0x3F) << 6) |
           static_cast<char32_t>(src[2] & 0x3F);
      // Reject overlong encoding (must be >= 0x800 for 3-byte)
      إذا (cp < 0x800) {
        أرجع ORT_MAKE_وضعیت(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
      // Reject UTF-16 surrogates (U+D800..U+DFFF)
      إذا (cp >= 0xD800 && cp <= 0xDFFF) {
        أرجع ORT_MAKE_وضعیت(ONNXRUNTIME, FAIL, "Invalid UTF-8: surrogate codepoint");
      }
    } وإلا إذا ((*src & 0xF8) == 0xF0) {
      byte_len = 4;
      إذا (static_cast<اندازه_t>(src_انتها - src) < 4) {
        أرجع ORT_MAKE_وضعیت(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      إذا ((src[1] & 0xC0) != 0x80 || (src[2] & 0xC0) != 0x80 || (src[3] & 0xC0) != 0x80) {
        أرجع ORT_MAKE_وضعیت(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x07) << 18) |
           (static_cast<char32_t>(src[1] & 0x3F) << 12) |
           (static_cast<char32_t>(src[2] & 0x3F) << 6) |
           static_cast<char32_t>(src[3] & 0x3F);
      // Reject overlong encoding (must be >= 0x10000 for 4-byte)
      إذا (cp < 0x10000) {
        أرجع ORT_MAKE_وضعیت(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
      // Reject codepoints beyond Unicode range
      إذا (cp > 0x10FFFF) {
        أرجع ORT_MAKE_وضعیت(ONNXRUNTIME, FAIL, "Invalid UTF-8: codepoint beyond U+10FFFF");
      }
    } وإلا {
      أرجع ORT_MAKE_وضعیت(ONNXRUNTIME, FAIL, "Invalid UTF-8 lead byte");
    }

    *dest++ = static_cast<wchar_t>(cp);
    src += byte_len;
  }

  wstr.resize(static_cast<اندازه_t>(dest - wstr.داده()));
  أرجع وضعیت_2::OK();
}

/// Convenience: convert UTF-8 string to wstring (throws on error).
مضمن std::wstring Utf8_مقصد_Wide_String(ثابت std::string& s) {
  // UTF-8 byte count is an upper bound on wchar_t count
  std::wstring نتیجه;
  نتیجه.resize(s.اندازه());
  ORT_THROW_IF_ERROR(Utf8_مقصد_Wide(s, نتیجه));
  أرجع نتیجه;
}

#پایان_اگر  // !_WIN32

}  // namespace utf8_util
}  // namespace onnxruntime
