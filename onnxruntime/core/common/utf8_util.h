// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ਹਦਾਇਤ once

#ਅਨ੍ਤਰ੍ਭਾਵ "core/common/common.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <string>

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
ਸਤਰਅੰਦਰ bool utf8_ਬਾਈਟਾਂ(ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ch, ਮਾਪ_t& len) {
  ਜੇ ((ch & 0x80) == 0) {
    len = 1;
    ਵਾਪਸ true;
  }
  ਜੇ ((ch & 0xE0) == 0xC0) {
    len = 2;
    ਵਾਪਸ true;
  }
  ਜੇ ((ch & 0xF0) == 0xE0) {
    len = 3;
    ਵਾਪਸ true;
  }
  ਜੇ ((ch & 0xF8) == 0xF0) {
    len = 4;
    ਵਾਪਸ true;
  }
  ਵਾਪਸ false;
}

// Computes length of the utf8 string in characters
ਸਤਰਅੰਦਰ bool utf8_len(ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ* s, ਮਾਪ_t ਬਾਈਟਾਂ, ਮਾਪ_t& len) {
  ਮਾਪ_t ਨਤੀਜਾ = 0;
  ਜਦੋਂ (ਬਾਈਟਾਂ > 0) {
    ਮਾਪ_t char_ਬਾਈਟਾਂ = 0;
    bool valid = utf8_ਬਾਈਟਾਂ(*s, char_ਬਾਈਟਾਂ);
    ਜੇ (!valid || ਬਾਈਟਾਂ < char_ਬਾਈਟਾਂ) {
      ਵਾਪਸ false;
    }
    ਬਾਈਟਾਂ -= char_ਬਾਈਟਾਂ;
    s += char_ਬਾਈਟਾਂ;
    ++ਨਤੀਜਾ;
  }
  len = ਨਤੀਜਾ;
  ਵਾਪਸ true;
}

ਸਤਰਅੰਦਰ bool utf8_validate(ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ* s, ਮਾਪ_t len, ਮਾਪ_t& utf8_chars) {
  ਮਾਪ_t utf8_len = 0;
  ਮਾਪ_t idx = 0;
  ਜਦੋਂ (idx < len) {
    ਮਾਪ_t ਬਾਈਟਾਂ = 0;
    ਸਵੈਚਾਲਿਤ ch = s[idx];
    ਜੇ (utf8_ਬਾਈਟਾਂ(ch, ਬਾਈਟਾਂ)) {
      ਚੋਣ (ਬਾਈਟਾਂ) {
        ਹਾਲਤ 1:
          ਰੋਕੋ;
        ਹਾਲਤ 2: {
          // Reject overlong 2-byte sequences. Valid Unicode 2-byte encodings
          // start at U+0080, so lead bytes 0xC0 and 0xC1 are invalid.
          ਜੇ (ch < 0xC2u) {
            ਵਾਪਸ false;
          }
          ਜੇ (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
            ਵਾਪਸ false;
          }
        } ਰੋਕੋ;  // 2
        ਹਾਲਤ 3: {
          ਸਵੈਚਾਲਿਤ ch1 = s[idx];
          ਚੋਣ (ch1) {
            ਹਾਲਤ 0xE0u:
              ਜੇ (++idx >= len || s[idx] < 0xA0u || s[idx] > 0xBFu) {
                ਵਾਪਸ false;
              }
              ਰੋਕੋ;
            ਹਾਲਤ 0xEDu:
              ਜੇ (++idx >= len || s[idx] < 0x80u || s[idx] > 0x9Fu) {
                ਵਾਪਸ false;
              }
              ਰੋਕੋ;
            ਮੂਲ: {
              ਜੇ ((ch1 >= 0xE1u && ch1 <= 0xECu) ||
                  (ch1 >= 0xEEu && ch1 <= 0xEFu)) {
                ਜੇ (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
                  ਵਾਪਸ false;
                }
              } ਨਹੀਂਤਾਂ {
                ਵਾਪਸ false;
              }
            } ਰੋਕੋ;
          }
          // validate byte 3
          ਜੇ (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
            ਵਾਪਸ false;
          }
        } ਰੋਕੋ;  // 3
        ਹਾਲਤ 4: {
          ਸਵੈਚਾਲਿਤ ch1 = s[idx];
          ਚੋਣ (ch1) {
            ਹਾਲਤ 0xF0u: {
              ਜੇ (++idx >= len || s[idx] < 0x90u || s[idx] > 0xBFu) {
                ਵਾਪਸ false;
              }
            } ਰੋਕੋ;
            ਹਾਲਤ 0xF4u: {
              ਜੇ (++idx >= len || s[idx] < 0x80u || s[idx] > 0x8Fu) {
                ਵਾਪਸ false;
              }
            } ਰੋਕੋ;
            ਮੂਲ: {
              ਜੇ (ch1 >= 0xF1u && ch1 <= 0xF3u) {
                ਜੇ (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
                  ਵਾਪਸ false;
                }
              } ਨਹੀਂਤਾਂ {
                ਵਾਪਸ false;
              }
            } ਰੋਕੋ;
          }
          // validate bytes 3 and 4
          ਮਾਪ_t stop = idx + 2;
          ਜਦੋਂ (idx < stop) {
            ਜੇ (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
              ਵਾਪਸ false;
            }
          }
        } ਰੋਕੋ;  // 4
        ਮੂਲ:
          // no chars longer than 4
          ਵਾਪਸ false;
      }  // switch bytes
      ++idx;
      ++utf8_len;
    } ਨਹੀਂਤਾਂ {
      ਵਾਪਸ false;
    }
  }
  // End index must match
  // the end of the last byte sequence.
  ਜੇ (idx != len) {
    ਵਾਪਸ false;
  }
  utf8_chars = utf8_len;
  ਵਾਪਸ true;
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
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _WIN32

static_assert(ਆਕਾਰ(wchar_t) >= 4,
              "Non-Windows UTF-8/wchar_t conversion helpers require wchar_t to be at least 32 bits.");

/// Compute the number of UTF-8 bytes required to encode a wide string.
ਸਤਰਅੰਦਰ ਮਾਪ_t Wide_ਮੰਜ਼ਿਲ_Utf8_Required_ਮਾਪ(ਅਚਲ std::wstring& wstr) {
  ਮਾਪ_t ਨਤੀਜਾ = 0;
  ਲਈ (wchar_t wc : wstr) {
    char32_t cp = static_cast<char32_t>(wc);
    ਜੇ (cp >= 0xD800 && cp <= 0xDFFF) {
      ORT_THROW("Invalid Unicode surrogate codepoint U+", std::hex, static_cast<uint32_t>(cp));
    }
    ਜੇ (cp <= 0x7F) {
      ਨਤੀਜਾ += 1;
    } ਨਹੀਂਤਾਂ ਜੇ (cp <= 0x7FF) {
      ਨਤੀਜਾ += 2;
    } ਨਹੀਂਤਾਂ ਜੇ (cp <= 0xFFFF) {
      ਨਤੀਜਾ += 3;
    } ਨਹੀਂਤਾਂ ਜੇ (cp <= 0x10FFFF) {
      ਨਤੀਜਾ += 4;
    } ਨਹੀਂਤਾਂ {
      ORT_THROW("Invalid Unicode codepoint U+", std::hex, static_cast<uint32_t>(cp));
    }
  }
  ਵਾਪਸ ਨਤੀਜਾ;
}

/// Convert a wide string to UTF-8, writing into a pre-allocated std::string.
/// The string is resized to the actual number of bytes written.
ਸਤਰਅੰਦਰ ਸਥਿਤੀ_2 Wide_ਮੰਜ਼ਿਲ_Utf8(ਅਚਲ std::wstring& wstr, std::string& str) {
  ਜੇ (wstr.ਖਾਲੀ_ਥਾਵਾਂ()) {
    str.clear();
    ਵਾਪਸ ਸਥਿਤੀ_2::OK();
  }

  ਅੱਖਰ* dest = str.ਅੰਕੜੇ();
  ਅੱਖਰ* dest_ਸਿਰਾ = dest + str.ਮਾਪ();

  ਲਈ (wchar_t wc : wstr) {
    char32_t cp = static_cast<char32_t>(wc);
    ਜੇ (cp >= 0xD800 && cp <= 0xDFFF) {
      ਵਾਪਸ ORT_MAKE_ਸਥਿਤੀ(ONNXRUNTIME, FAIL,
                             "Invalid Unicode surrogate codepoint during UTF-8 conversion");
    }
    ਜੇ (cp <= 0x7F) {
      ਅਚਲ ਮਾਪ_t ਬਚਿਆ = static_cast<ਮਾਪ_t>(dest_ਸਿਰਾ - dest);
      ਜੇ (ਬਚਿਆ < 1) {
        ਵਾਪਸ ORT_MAKE_ਸਥਿਤੀ(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<ਅੱਖਰ>(cp);
    } ਨਹੀਂਤਾਂ ਜੇ (cp <= 0x7FF) {
      ਅਚਲ ਮਾਪ_t ਬਚਿਆ = static_cast<ਮਾਪ_t>(dest_ਸਿਰਾ - dest);
      ਜੇ (ਬਚਿਆ < 2) {
        ਵਾਪਸ ORT_MAKE_ਸਥਿਤੀ(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<ਅੱਖਰ>(0xC0 | (cp >> 6));
      *dest++ = static_cast<ਅੱਖਰ>(0x80 | (cp & 0x3F));
    } ਨਹੀਂਤਾਂ ਜੇ (cp <= 0xFFFF) {
      ਅਚਲ ਮਾਪ_t ਬਚਿਆ = static_cast<ਮਾਪ_t>(dest_ਸਿਰਾ - dest);
      ਜੇ (ਬਚਿਆ < 3) {
        ਵਾਪਸ ORT_MAKE_ਸਥਿਤੀ(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<ਅੱਖਰ>(0xE0 | (cp >> 12));
      *dest++ = static_cast<ਅੱਖਰ>(0x80 | ((cp >> 6) & 0x3F));
      *dest++ = static_cast<ਅੱਖਰ>(0x80 | (cp & 0x3F));
    } ਨਹੀਂਤਾਂ ਜੇ (cp <= 0x10FFFF) {
      ਅਚਲ ਮਾਪ_t ਬਚਿਆ = static_cast<ਮਾਪ_t>(dest_ਸਿਰਾ - dest);
      ਜੇ (ਬਚਿਆ < 4) {
        ਵਾਪਸ ORT_MAKE_ਸਥਿਤੀ(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<ਅੱਖਰ>(0xF0 | (cp >> 18));
      *dest++ = static_cast<ਅੱਖਰ>(0x80 | ((cp >> 12) & 0x3F));
      *dest++ = static_cast<ਅੱਖਰ>(0x80 | ((cp >> 6) & 0x3F));
      *dest++ = static_cast<ਅੱਖਰ>(0x80 | (cp & 0x3F));
    } ਨਹੀਂਤਾਂ {
      ਵਾਪਸ ORT_MAKE_ਸਥਿਤੀ(ONNXRUNTIME, FAIL,
                             "Invalid Unicode codepoint during UTF-8 conversion");
    }
  }

  str.resize(static_cast<ਮਾਪ_t>(dest - str.ਅੰਕੜੇ()));
  ਵਾਪਸ ਸਥਿਤੀ_2::OK();
}

/// Convert a UTF-8 string to a wide string, writing into a pre-allocated std::wstring.
/// The wstring is resized to the actual number of characters written.
/// Validates continuation bytes and rejects overlong encodings and surrogates.
ਸਤਰਅੰਦਰ ਸਥਿਤੀ_2 Utf8_ਮੰਜ਼ਿਲ_Wide(ਅਚਲ std::string& str, std::wstring& wstr) {
  ਜੇ (str.ਖਾਲੀ_ਥਾਵਾਂ()) {
    wstr.clear();
    ਵਾਪਸ ਸਥਿਤੀ_2::OK();
  }

  ਜੇ (wstr.ਮਾਪ() < str.ਮਾਪ()) {
    wstr.resize(str.ਮਾਪ());
  }

  ਅਚਲ ਸਵੈਚਾਲਿਤ* src = reinterpret_cast<ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ*>(str.ਅੰਕੜੇ());
  ਅਚਲ ਸਵੈਚਾਲਿਤ* src_ਸਿਰਾ = src + str.ਮਾਪ();
  wchar_t* dest = wstr.ਅੰਕੜੇ();

  ਜਦੋਂ (src < src_ਸਿਰਾ) {
    char32_t cp = 0;
    ਮਾਪ_t byte_len = 0;

    ਜੇ ((*src & 0x80) == 0) {
      cp = *src;
      byte_len = 1;
    } ਨਹੀਂਤਾਂ ਜੇ ((*src & 0xE0) == 0xC0) {
      byte_len = 2;
      ਜੇ (static_cast<ਮਾਪ_t>(src_ਸਿਰਾ - src) < 2) {
        ਵਾਪਸ ORT_MAKE_ਸਥਿਤੀ(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      ਜੇ ((src[1] & 0xC0) != 0x80) {
        ਵਾਪਸ ORT_MAKE_ਸਥਿਤੀ(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x1F) << 6) |
           static_cast<char32_t>(src[1] & 0x3F);
      // Reject overlong encoding (must be >= 0x80 for 2-byte)
      ਜੇ (cp < 0x80) {
        ਵਾਪਸ ORT_MAKE_ਸਥਿਤੀ(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
    } ਨਹੀਂਤਾਂ ਜੇ ((*src & 0xF0) == 0xE0) {
      byte_len = 3;
      ਜੇ (static_cast<ਮਾਪ_t>(src_ਸਿਰਾ - src) < 3) {
        ਵਾਪਸ ORT_MAKE_ਸਥਿਤੀ(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      ਜੇ ((src[1] & 0xC0) != 0x80 || (src[2] & 0xC0) != 0x80) {
        ਵਾਪਸ ORT_MAKE_ਸਥਿਤੀ(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x0F) << 12) |
           (static_cast<char32_t>(src[1] & 0x3F) << 6) |
           static_cast<char32_t>(src[2] & 0x3F);
      // Reject overlong encoding (must be >= 0x800 for 3-byte)
      ਜੇ (cp < 0x800) {
        ਵਾਪਸ ORT_MAKE_ਸਥਿਤੀ(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
      // Reject UTF-16 surrogates (U+D800..U+DFFF)
      ਜੇ (cp >= 0xD800 && cp <= 0xDFFF) {
        ਵਾਪਸ ORT_MAKE_ਸਥਿਤੀ(ONNXRUNTIME, FAIL, "Invalid UTF-8: surrogate codepoint");
      }
    } ਨਹੀਂਤਾਂ ਜੇ ((*src & 0xF8) == 0xF0) {
      byte_len = 4;
      ਜੇ (static_cast<ਮਾਪ_t>(src_ਸਿਰਾ - src) < 4) {
        ਵਾਪਸ ORT_MAKE_ਸਥਿਤੀ(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      ਜੇ ((src[1] & 0xC0) != 0x80 || (src[2] & 0xC0) != 0x80 || (src[3] & 0xC0) != 0x80) {
        ਵਾਪਸ ORT_MAKE_ਸਥਿਤੀ(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x07) << 18) |
           (static_cast<char32_t>(src[1] & 0x3F) << 12) |
           (static_cast<char32_t>(src[2] & 0x3F) << 6) |
           static_cast<char32_t>(src[3] & 0x3F);
      // Reject overlong encoding (must be >= 0x10000 for 4-byte)
      ਜੇ (cp < 0x10000) {
        ਵਾਪਸ ORT_MAKE_ਸਥਿਤੀ(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
      // Reject codepoints beyond Unicode range
      ਜੇ (cp > 0x10FFFF) {
        ਵਾਪਸ ORT_MAKE_ਸਥਿਤੀ(ONNXRUNTIME, FAIL, "Invalid UTF-8: codepoint beyond U+10FFFF");
      }
    } ਨਹੀਂਤਾਂ {
      ਵਾਪਸ ORT_MAKE_ਸਥਿਤੀ(ONNXRUNTIME, FAIL, "Invalid UTF-8 lead byte");
    }

    *dest++ = static_cast<wchar_t>(cp);
    src += byte_len;
  }

  wstr.resize(static_cast<ਮਾਪ_t>(dest - wstr.ਅੰਕੜੇ()));
  ਵਾਪਸ ਸਥਿਤੀ_2::OK();
}

/// Convenience: convert UTF-8 string to wstring (throws on error).
ਸਤਰਅੰਦਰ std::wstring Utf8_ਮੰਜ਼ਿਲ_Wide_String(ਅਚਲ std::string& s) {
  // UTF-8 byte count is an upper bound on wchar_t count
  std::wstring ਨਤੀਜਾ;
  ਨਤੀਜਾ.resize(s.ਮਾਪ());
  ORT_THROW_IF_ERROR(Utf8_ਮੰਜ਼ਿਲ_Wide(s, ਨਤੀਜਾ));
  ਵਾਪਸ ਨਤੀਜਾ;
}

#ਜੇ_ਅੰਤ  // !_WIN32

}  // namespace utf8_util
}  // namespace onnxruntime
