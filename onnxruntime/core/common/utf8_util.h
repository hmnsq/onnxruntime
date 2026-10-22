// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ಸೂಚನೆ once

#ಅನ್ತರ್ಭಾವ "core/common/common.h"

#ಅನ್ತರ್ಭಾವ <string>

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
ಸಾಲೊಳಗೆ bool utf8_ಬೈಟುಗಳು(ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ ch, ಗಾತ್ರ_t& len) {
  ಆದರೆ ((ch & 0x80) == 0) {
    len = 1;
    ಹಿಂತಿರುಗಿ true;
  }
  ಆದರೆ ((ch & 0xE0) == 0xC0) {
    len = 2;
    ಹಿಂತಿರುಗಿ true;
  }
  ಆದರೆ ((ch & 0xF0) == 0xE0) {
    len = 3;
    ಹಿಂತಿರುಗಿ true;
  }
  ಆದರೆ ((ch & 0xF8) == 0xF0) {
    len = 4;
    ಹಿಂತಿರುಗಿ true;
  }
  ಹಿಂತಿರುಗಿ false;
}

// Computes length of the utf8 string in characters
ಸಾಲೊಳಗೆ bool utf8_len(ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ* s, ಗಾತ್ರ_t ಬೈಟುಗಳು, ಗಾತ್ರ_t& len) {
  ಗಾತ್ರ_t ಫಲಿತಾಂಶ = 0;
  ಆಗ (ಬೈಟುಗಳು > 0) {
    ಗಾತ್ರ_t char_ಬೈಟುಗಳು = 0;
    bool valid = utf8_ಬೈಟುಗಳು(*s, char_ಬೈಟುಗಳು);
    ಆದರೆ (!valid || ಬೈಟುಗಳು < char_ಬೈಟುಗಳು) {
      ಹಿಂತಿರುಗಿ false;
    }
    ಬೈಟುಗಳು -= char_ಬೈಟುಗಳು;
    s += char_ಬೈಟುಗಳು;
    ++ಫಲಿತಾಂಶ;
  }
  len = ಫಲಿತಾಂಶ;
  ಹಿಂತಿರುಗಿ true;
}

ಸಾಲೊಳಗೆ bool utf8_validate(ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ* s, ಗಾತ್ರ_t len, ಗಾತ್ರ_t& utf8_chars) {
  ಗಾತ್ರ_t utf8_len = 0;
  ಗಾತ್ರ_t idx = 0;
  ಆಗ (idx < len) {
    ಗಾತ್ರ_t ಬೈಟುಗಳು = 0;
    ಸ್ವಯಂಚಾಲಿತ ch = s[idx];
    ಆದರೆ (utf8_ಬೈಟುಗಳು(ch, ಬೈಟುಗಳು)) {
      ಆಯ್ಕೆ (ಬೈಟುಗಳು) {
        ಸ್ಥಿತಿ 1:
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 2: {
          // Reject overlong 2-byte sequences. Valid Unicode 2-byte encodings
          // start at U+0080, so lead bytes 0xC0 and 0xC1 are invalid.
          ಆದರೆ (ch < 0xC2u) {
            ಹಿಂತಿರುಗಿ false;
          }
          ಆದರೆ (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
            ಹಿಂತಿರುಗಿ false;
          }
        } ನಿಲ್ಲಿಸು;  // 2
        ಸ್ಥಿತಿ 3: {
          ಸ್ವಯಂಚಾಲಿತ ch1 = s[idx];
          ಆಯ್ಕೆ (ch1) {
            ಸ್ಥಿತಿ 0xE0u:
              ಆದರೆ (++idx >= len || s[idx] < 0xA0u || s[idx] > 0xBFu) {
                ಹಿಂತಿರುಗಿ false;
              }
              ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ 0xEDu:
              ಆದರೆ (++idx >= len || s[idx] < 0x80u || s[idx] > 0x9Fu) {
                ಹಿಂತಿರುಗಿ false;
              }
              ನಿಲ್ಲಿಸು;
            ಪೂರ್ವನಿಯೋಜಿತ: {
              ಆದರೆ ((ch1 >= 0xE1u && ch1 <= 0xECu) ||
                  (ch1 >= 0xEEu && ch1 <= 0xEFu)) {
                ಆದರೆ (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
                  ಹಿಂತಿರುಗಿ false;
                }
              } ಇಲ್ಲದಿದ್ದರೆ {
                ಹಿಂತಿರುಗಿ false;
              }
            } ನಿಲ್ಲಿಸು;
          }
          // validate byte 3
          ಆದರೆ (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
            ಹಿಂತಿರುಗಿ false;
          }
        } ನಿಲ್ಲಿಸು;  // 3
        ಸ್ಥಿತಿ 4: {
          ಸ್ವಯಂಚಾಲಿತ ch1 = s[idx];
          ಆಯ್ಕೆ (ch1) {
            ಸ್ಥಿತಿ 0xF0u: {
              ಆದರೆ (++idx >= len || s[idx] < 0x90u || s[idx] > 0xBFu) {
                ಹಿಂತಿರುಗಿ false;
              }
            } ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ 0xF4u: {
              ಆದರೆ (++idx >= len || s[idx] < 0x80u || s[idx] > 0x8Fu) {
                ಹಿಂತಿರುಗಿ false;
              }
            } ನಿಲ್ಲಿಸು;
            ಪೂರ್ವನಿಯೋಜಿತ: {
              ಆದರೆ (ch1 >= 0xF1u && ch1 <= 0xF3u) {
                ಆದರೆ (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
                  ಹಿಂತಿರುಗಿ false;
                }
              } ಇಲ್ಲದಿದ್ದರೆ {
                ಹಿಂತಿರುಗಿ false;
              }
            } ನಿಲ್ಲಿಸು;
          }
          // validate bytes 3 and 4
          ಗಾತ್ರ_t stop = idx + 2;
          ಆಗ (idx < stop) {
            ಆದರೆ (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
              ಹಿಂತಿರುಗಿ false;
            }
          }
        } ನಿಲ್ಲಿಸು;  // 4
        ಪೂರ್ವನಿಯೋಜಿತ:
          // no chars longer than 4
          ಹಿಂತಿರುಗಿ false;
      }  // switch bytes
      ++idx;
      ++utf8_len;
    } ಇಲ್ಲದಿದ್ದರೆ {
      ಹಿಂತಿರುಗಿ false;
    }
  }
  // End index must match
  // the end of the last byte sequence.
  ಆದರೆ (idx != len) {
    ಹಿಂತಿರುಗಿ false;
  }
  utf8_chars = utf8_len;
  ಹಿಂತಿರುಗಿ true;
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
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ _WIN32

static_assert(ಗಾತ್ರ(wchar_t) >= 4,
              "Non-Windows UTF-8/wchar_t conversion helpers require wchar_t to be at least 32 bits.");

/// Compute the number of UTF-8 bytes required to encode a wide string.
ಸಾಲೊಳಗೆ ಗಾತ್ರ_t Wide_ಗಮ್ಯ_Utf8_Required_ಗಾತ್ರ(ಸ್ಥಿರಾಂಕ std::wstring& wstr) {
  ಗಾತ್ರ_t ಫಲಿತಾಂಶ = 0;
  ಗಾಗಿ (wchar_t wc : wstr) {
    char32_t cp = static_cast<char32_t>(wc);
    ಆದರೆ (cp >= 0xD800 && cp <= 0xDFFF) {
      ORT_THROW("Invalid Unicode surrogate codepoint U+", std::hex, static_cast<uint32_t>(cp));
    }
    ಆದರೆ (cp <= 0x7F) {
      ಫಲಿತಾಂಶ += 1;
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (cp <= 0x7FF) {
      ಫಲಿತಾಂಶ += 2;
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (cp <= 0xFFFF) {
      ಫಲಿತಾಂಶ += 3;
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (cp <= 0x10FFFF) {
      ಫಲಿತಾಂಶ += 4;
    } ಇಲ್ಲದಿದ್ದರೆ {
      ORT_THROW("Invalid Unicode codepoint U+", std::hex, static_cast<uint32_t>(cp));
    }
  }
  ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

/// Convert a wide string to UTF-8, writing into a pre-allocated std::string.
/// The string is resized to the actual number of bytes written.
ಸಾಲೊಳಗೆ ಸ್ಥಿತಿ_3 Wide_ಗಮ್ಯ_Utf8(ಸ್ಥಿರಾಂಕ std::wstring& wstr, std::string& str) {
  ಆದರೆ (wstr.ಖಾಲಿಗಳು()) {
    str.clear();
    ಹಿಂತಿರುಗಿ ಸ್ಥಿತಿ_3::OK();
  }

  ಅಕ್ಷರ* dest = str.ದತ್ತಾಂಶ();
  ಅಕ್ಷರ* dest_ಕೊನೆ = dest + str.ಗಾತ್ರ_2();

  ಗಾಗಿ (wchar_t wc : wstr) {
    char32_t cp = static_cast<char32_t>(wc);
    ಆದರೆ (cp >= 0xD800 && cp <= 0xDFFF) {
      ಹಿಂತಿರುಗಿ ORT_MAKE_ಸ್ಥಿತಿ(ONNXRUNTIME, FAIL,
                             "Invalid Unicode surrogate codepoint during UTF-8 conversion");
    }
    ಆದರೆ (cp <= 0x7F) {
      ಸ್ಥಿರಾಂಕ ಗಾತ್ರ_t ಉಳಿದ = static_cast<ಗಾತ್ರ_t>(dest_ಕೊನೆ - dest);
      ಆದರೆ (ಉಳಿದ < 1) {
        ಹಿಂತಿರುಗಿ ORT_MAKE_ಸ್ಥಿತಿ(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<ಅಕ್ಷರ>(cp);
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (cp <= 0x7FF) {
      ಸ್ಥಿರಾಂಕ ಗಾತ್ರ_t ಉಳಿದ = static_cast<ಗಾತ್ರ_t>(dest_ಕೊನೆ - dest);
      ಆದರೆ (ಉಳಿದ < 2) {
        ಹಿಂತಿರುಗಿ ORT_MAKE_ಸ್ಥಿತಿ(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<ಅಕ್ಷರ>(0xC0 | (cp >> 6));
      *dest++ = static_cast<ಅಕ್ಷರ>(0x80 | (cp & 0x3F));
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (cp <= 0xFFFF) {
      ಸ್ಥಿರಾಂಕ ಗಾತ್ರ_t ಉಳಿದ = static_cast<ಗಾತ್ರ_t>(dest_ಕೊನೆ - dest);
      ಆದರೆ (ಉಳಿದ < 3) {
        ಹಿಂತಿರುಗಿ ORT_MAKE_ಸ್ಥಿತಿ(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<ಅಕ್ಷರ>(0xE0 | (cp >> 12));
      *dest++ = static_cast<ಅಕ್ಷರ>(0x80 | ((cp >> 6) & 0x3F));
      *dest++ = static_cast<ಅಕ್ಷರ>(0x80 | (cp & 0x3F));
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (cp <= 0x10FFFF) {
      ಸ್ಥಿರಾಂಕ ಗಾತ್ರ_t ಉಳಿದ = static_cast<ಗಾತ್ರ_t>(dest_ಕೊನೆ - dest);
      ಆದರೆ (ಉಳಿದ < 4) {
        ಹಿಂತಿರುಗಿ ORT_MAKE_ಸ್ಥಿತಿ(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<ಅಕ್ಷರ>(0xF0 | (cp >> 18));
      *dest++ = static_cast<ಅಕ್ಷರ>(0x80 | ((cp >> 12) & 0x3F));
      *dest++ = static_cast<ಅಕ್ಷರ>(0x80 | ((cp >> 6) & 0x3F));
      *dest++ = static_cast<ಅಕ್ಷರ>(0x80 | (cp & 0x3F));
    } ಇಲ್ಲದಿದ್ದರೆ {
      ಹಿಂತಿರುಗಿ ORT_MAKE_ಸ್ಥಿತಿ(ONNXRUNTIME, FAIL,
                             "Invalid Unicode codepoint during UTF-8 conversion");
    }
  }

  str.resize(static_cast<ಗಾತ್ರ_t>(dest - str.ದತ್ತಾಂಶ()));
  ಹಿಂತಿರುಗಿ ಸ್ಥಿತಿ_3::OK();
}

/// Convert a UTF-8 string to a wide string, writing into a pre-allocated std::wstring.
/// The wstring is resized to the actual number of characters written.
/// Validates continuation bytes and rejects overlong encodings and surrogates.
ಸಾಲೊಳಗೆ ಸ್ಥಿತಿ_3 Utf8_ಗಮ್ಯ_Wide(ಸ್ಥಿರಾಂಕ std::string& str, std::wstring& wstr) {
  ಆದರೆ (str.ಖಾಲಿಗಳು()) {
    wstr.clear();
    ಹಿಂತಿರುಗಿ ಸ್ಥಿತಿ_3::OK();
  }

  ಆದರೆ (wstr.ಗಾತ್ರ_2() < str.ಗಾತ್ರ_2()) {
    wstr.resize(str.ಗಾತ್ರ_2());
  }

  ಸ್ಥಿರಾಂಕ ಸ್ವಯಂಚಾಲಿತ* src = reinterpret_cast<ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ*>(str.ದತ್ತಾಂಶ());
  ಸ್ಥಿರಾಂಕ ಸ್ವಯಂಚಾಲಿತ* src_ಕೊನೆ = src + str.ಗಾತ್ರ_2();
  wchar_t* dest = wstr.ದತ್ತಾಂಶ();

  ಆಗ (src < src_ಕೊನೆ) {
    char32_t cp = 0;
    ಗಾತ್ರ_t byte_len = 0;

    ಆದರೆ ((*src & 0x80) == 0) {
      cp = *src;
      byte_len = 1;
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ((*src & 0xE0) == 0xC0) {
      byte_len = 2;
      ಆದರೆ (static_cast<ಗಾತ್ರ_t>(src_ಕೊನೆ - src) < 2) {
        ಹಿಂತಿರುಗಿ ORT_MAKE_ಸ್ಥಿತಿ(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      ಆದರೆ ((src[1] & 0xC0) != 0x80) {
        ಹಿಂತಿರುಗಿ ORT_MAKE_ಸ್ಥಿತಿ(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x1F) << 6) |
           static_cast<char32_t>(src[1] & 0x3F);
      // Reject overlong encoding (must be >= 0x80 for 2-byte)
      ಆದರೆ (cp < 0x80) {
        ಹಿಂತಿರುಗಿ ORT_MAKE_ಸ್ಥಿತಿ(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ((*src & 0xF0) == 0xE0) {
      byte_len = 3;
      ಆದರೆ (static_cast<ಗಾತ್ರ_t>(src_ಕೊನೆ - src) < 3) {
        ಹಿಂತಿರುಗಿ ORT_MAKE_ಸ್ಥಿತಿ(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      ಆದರೆ ((src[1] & 0xC0) != 0x80 || (src[2] & 0xC0) != 0x80) {
        ಹಿಂತಿರುಗಿ ORT_MAKE_ಸ್ಥಿತಿ(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x0F) << 12) |
           (static_cast<char32_t>(src[1] & 0x3F) << 6) |
           static_cast<char32_t>(src[2] & 0x3F);
      // Reject overlong encoding (must be >= 0x800 for 3-byte)
      ಆದರೆ (cp < 0x800) {
        ಹಿಂತಿರುಗಿ ORT_MAKE_ಸ್ಥಿತಿ(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
      // Reject UTF-16 surrogates (U+D800..U+DFFF)
      ಆದರೆ (cp >= 0xD800 && cp <= 0xDFFF) {
        ಹಿಂತಿರುಗಿ ORT_MAKE_ಸ್ಥಿತಿ(ONNXRUNTIME, FAIL, "Invalid UTF-8: surrogate codepoint");
      }
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ((*src & 0xF8) == 0xF0) {
      byte_len = 4;
      ಆದರೆ (static_cast<ಗಾತ್ರ_t>(src_ಕೊನೆ - src) < 4) {
        ಹಿಂತಿರುಗಿ ORT_MAKE_ಸ್ಥಿತಿ(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      ಆದರೆ ((src[1] & 0xC0) != 0x80 || (src[2] & 0xC0) != 0x80 || (src[3] & 0xC0) != 0x80) {
        ಹಿಂತಿರುಗಿ ORT_MAKE_ಸ್ಥಿತಿ(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x07) << 18) |
           (static_cast<char32_t>(src[1] & 0x3F) << 12) |
           (static_cast<char32_t>(src[2] & 0x3F) << 6) |
           static_cast<char32_t>(src[3] & 0x3F);
      // Reject overlong encoding (must be >= 0x10000 for 4-byte)
      ಆದರೆ (cp < 0x10000) {
        ಹಿಂತಿರುಗಿ ORT_MAKE_ಸ್ಥಿತಿ(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
      // Reject codepoints beyond Unicode range
      ಆದರೆ (cp > 0x10FFFF) {
        ಹಿಂತಿರುಗಿ ORT_MAKE_ಸ್ಥಿತಿ(ONNXRUNTIME, FAIL, "Invalid UTF-8: codepoint beyond U+10FFFF");
      }
    } ಇಲ್ಲದಿದ್ದರೆ {
      ಹಿಂತಿರುಗಿ ORT_MAKE_ಸ್ಥಿತಿ(ONNXRUNTIME, FAIL, "Invalid UTF-8 lead byte");
    }

    *dest++ = static_cast<wchar_t>(cp);
    src += byte_len;
  }

  wstr.resize(static_cast<ಗಾತ್ರ_t>(dest - wstr.ದತ್ತಾಂಶ()));
  ಹಿಂತಿರುಗಿ ಸ್ಥಿತಿ_3::OK();
}

/// Convenience: convert UTF-8 string to wstring (throws on error).
ಸಾಲೊಳಗೆ std::wstring Utf8_ಗಮ್ಯ_Wide_String(ಸ್ಥಿರಾಂಕ std::string& s) {
  // UTF-8 byte count is an upper bound on wchar_t count
  std::wstring ಫಲಿತಾಂಶ;
  ಫಲಿತಾಂಶ.resize(s.ಗಾತ್ರ_2());
  ORT_THROW_IF_ERROR(Utf8_ಗಮ್ಯ_Wide(s, ಫಲಿತಾಂಶ));
  ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

#ಆದರೆ_ಅಂತ್ಯ  // !_WIN32

}  // namespace utf8_util
}  // namespace onnxruntime
