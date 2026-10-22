// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ہدایت once

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
اندرونی bool utf8_بائٹیں(غیرعلامتی حرف ch, سائز_t& len) {
  اگر ((ch & 0x80) == 0) {
    len = 1;
    واپس true;
  }
  اگر ((ch & 0xE0) == 0xC0) {
    len = 2;
    واپس true;
  }
  اگر ((ch & 0xF0) == 0xE0) {
    len = 3;
    واپس true;
  }
  اگر ((ch & 0xF8) == 0xF0) {
    len = 4;
    واپس true;
  }
  واپس false;
}

// Computes length of the utf8 string in characters
اندرونی bool utf8_len(ثابت غیرعلامتی حرف* s, سائز_t بائٹیں, سائز_t& len) {
  سائز_t نتیجہ = 0;
  جب (بائٹیں > 0) {
    سائز_t char_بائٹیں = 0;
    bool valid = utf8_بائٹیں(*s, char_بائٹیں);
    اگر (!valid || بائٹیں < char_بائٹیں) {
      واپس false;
    }
    بائٹیں -= char_بائٹیں;
    s += char_بائٹیں;
    ++نتیجہ;
  }
  len = نتیجہ;
  واپس true;
}

اندرونی bool utf8_validate(ثابت غیرعلامتی حرف* s, سائز_t len, سائز_t& utf8_chars) {
  سائز_t utf8_len = 0;
  سائز_t idx = 0;
  جب (idx < len) {
    سائز_t بائٹیں = 0;
    خودکار ch = s[idx];
    اگر (utf8_بائٹیں(ch, بائٹیں)) {
      انتخاب (بائٹیں) {
        حالت 1:
          توڑو;
        حالت 2: {
          // Reject overlong 2-byte sequences. Valid Unicode 2-byte encodings
          // start at U+0080, so lead bytes 0xC0 and 0xC1 are invalid.
          اگر (ch < 0xC2u) {
            واپس false;
          }
          اگر (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
            واپس false;
          }
        } توڑو;  // 2
        حالت 3: {
          خودکار ch1 = s[idx];
          انتخاب (ch1) {
            حالت 0xE0u:
              اگر (++idx >= len || s[idx] < 0xA0u || s[idx] > 0xBFu) {
                واپس false;
              }
              توڑو;
            حالت 0xEDu:
              اگر (++idx >= len || s[idx] < 0x80u || s[idx] > 0x9Fu) {
                واپس false;
              }
              توڑو;
            طےشدہ: {
              اگر ((ch1 >= 0xE1u && ch1 <= 0xECu) ||
                  (ch1 >= 0xEEu && ch1 <= 0xEFu)) {
                اگر (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
                  واپس false;
                }
              } ورنہ {
                واپس false;
              }
            } توڑو;
          }
          // validate byte 3
          اگر (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
            واپس false;
          }
        } توڑو;  // 3
        حالت 4: {
          خودکار ch1 = s[idx];
          انتخاب (ch1) {
            حالت 0xF0u: {
              اگر (++idx >= len || s[idx] < 0x90u || s[idx] > 0xBFu) {
                واپس false;
              }
            } توڑو;
            حالت 0xF4u: {
              اگر (++idx >= len || s[idx] < 0x80u || s[idx] > 0x8Fu) {
                واپس false;
              }
            } توڑو;
            طےشدہ: {
              اگر (ch1 >= 0xF1u && ch1 <= 0xF3u) {
                اگر (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
                  واپس false;
                }
              } ورنہ {
                واپس false;
              }
            } توڑو;
          }
          // validate bytes 3 and 4
          سائز_t stop = idx + 2;
          جب (idx < stop) {
            اگر (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
              واپس false;
            }
          }
        } توڑو;  // 4
        طےشدہ:
          // no chars longer than 4
          واپس false;
      }  // switch bytes
      ++idx;
      ++utf8_len;
    } ورنہ {
      واپس false;
    }
  }
  // End index must match
  // the end of the last byte sequence.
  اگر (idx != len) {
    واپس false;
  }
  utf8_chars = utf8_len;
  واپس true;
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
#اگر_غیر_تعریف_شدہ _WIN32

static_assert(حجم(wchar_t) >= 4,
              "Non-Windows UTF-8/wchar_t conversion helpers require wchar_t to be at least 32 bits.");

/// Compute the number of UTF-8 bytes required to encode a wide string.
اندرونی سائز_t Wide_مقصد_Utf8_Required_سائز(ثابت std::wstring& wstr) {
  سائز_t نتیجہ = 0;
  برائے (wchar_t wc : wstr) {
    char32_t cp = static_cast<char32_t>(wc);
    اگر (cp >= 0xD800 && cp <= 0xDFFF) {
      ORT_THROW("Invalid Unicode surrogate codepoint U+", std::hex, static_cast<uint32_t>(cp));
    }
    اگر (cp <= 0x7F) {
      نتیجہ += 1;
    } ورنہ اگر (cp <= 0x7FF) {
      نتیجہ += 2;
    } ورنہ اگر (cp <= 0xFFFF) {
      نتیجہ += 3;
    } ورنہ اگر (cp <= 0x10FFFF) {
      نتیجہ += 4;
    } ورنہ {
      ORT_THROW("Invalid Unicode codepoint U+", std::hex, static_cast<uint32_t>(cp));
    }
  }
  واپس نتیجہ;
}

/// Convert a wide string to UTF-8, writing into a pre-allocated std::string.
/// The string is resized to the actual number of bytes written.
اندرونی حالت_3 Wide_مقصد_Utf8(ثابت std::wstring& wstr, std::string& str) {
  اگر (wstr.خالی_خانے()) {
    str.clear();
    واپس حالت_3::OK();
  }

  حرف* dest = str.ڈیٹا();
  حرف* dest_انتہا = dest + str.سائز();

  برائے (wchar_t wc : wstr) {
    char32_t cp = static_cast<char32_t>(wc);
    اگر (cp >= 0xD800 && cp <= 0xDFFF) {
      واپس ORT_MAKE_حالت(ONNXRUNTIME, FAIL,
                             "Invalid Unicode surrogate codepoint during UTF-8 conversion");
    }
    اگر (cp <= 0x7F) {
      ثابت سائز_t بقایا = static_cast<سائز_t>(dest_انتہا - dest);
      اگر (بقایا < 1) {
        واپس ORT_MAKE_حالت(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<حرف>(cp);
    } ورنہ اگر (cp <= 0x7FF) {
      ثابت سائز_t بقایا = static_cast<سائز_t>(dest_انتہا - dest);
      اگر (بقایا < 2) {
        واپس ORT_MAKE_حالت(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<حرف>(0xC0 | (cp >> 6));
      *dest++ = static_cast<حرف>(0x80 | (cp & 0x3F));
    } ورنہ اگر (cp <= 0xFFFF) {
      ثابت سائز_t بقایا = static_cast<سائز_t>(dest_انتہا - dest);
      اگر (بقایا < 3) {
        واپس ORT_MAKE_حالت(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<حرف>(0xE0 | (cp >> 12));
      *dest++ = static_cast<حرف>(0x80 | ((cp >> 6) & 0x3F));
      *dest++ = static_cast<حرف>(0x80 | (cp & 0x3F));
    } ورنہ اگر (cp <= 0x10FFFF) {
      ثابت سائز_t بقایا = static_cast<سائز_t>(dest_انتہا - dest);
      اگر (بقایا < 4) {
        واپس ORT_MAKE_حالت(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<حرف>(0xF0 | (cp >> 18));
      *dest++ = static_cast<حرف>(0x80 | ((cp >> 12) & 0x3F));
      *dest++ = static_cast<حرف>(0x80 | ((cp >> 6) & 0x3F));
      *dest++ = static_cast<حرف>(0x80 | (cp & 0x3F));
    } ورنہ {
      واپس ORT_MAKE_حالت(ONNXRUNTIME, FAIL,
                             "Invalid Unicode codepoint during UTF-8 conversion");
    }
  }

  str.resize(static_cast<سائز_t>(dest - str.ڈیٹا()));
  واپس حالت_3::OK();
}

/// Convert a UTF-8 string to a wide string, writing into a pre-allocated std::wstring.
/// The wstring is resized to the actual number of characters written.
/// Validates continuation bytes and rejects overlong encodings and surrogates.
اندرونی حالت_3 Utf8_مقصد_Wide(ثابت std::string& str, std::wstring& wstr) {
  اگر (str.خالی_خانے()) {
    wstr.clear();
    واپس حالت_3::OK();
  }

  اگر (wstr.سائز() < str.سائز()) {
    wstr.resize(str.سائز());
  }

  ثابت خودکار* src = reinterpret_cast<ثابت غیرعلامتی حرف*>(str.ڈیٹا());
  ثابت خودکار* src_انتہا = src + str.سائز();
  wchar_t* dest = wstr.ڈیٹا();

  جب (src < src_انتہا) {
    char32_t cp = 0;
    سائز_t byte_len = 0;

    اگر ((*src & 0x80) == 0) {
      cp = *src;
      byte_len = 1;
    } ورنہ اگر ((*src & 0xE0) == 0xC0) {
      byte_len = 2;
      اگر (static_cast<سائز_t>(src_انتہا - src) < 2) {
        واپس ORT_MAKE_حالت(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      اگر ((src[1] & 0xC0) != 0x80) {
        واپس ORT_MAKE_حالت(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x1F) << 6) |
           static_cast<char32_t>(src[1] & 0x3F);
      // Reject overlong encoding (must be >= 0x80 for 2-byte)
      اگر (cp < 0x80) {
        واپس ORT_MAKE_حالت(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
    } ورنہ اگر ((*src & 0xF0) == 0xE0) {
      byte_len = 3;
      اگر (static_cast<سائز_t>(src_انتہا - src) < 3) {
        واپس ORT_MAKE_حالت(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      اگر ((src[1] & 0xC0) != 0x80 || (src[2] & 0xC0) != 0x80) {
        واپس ORT_MAKE_حالت(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x0F) << 12) |
           (static_cast<char32_t>(src[1] & 0x3F) << 6) |
           static_cast<char32_t>(src[2] & 0x3F);
      // Reject overlong encoding (must be >= 0x800 for 3-byte)
      اگر (cp < 0x800) {
        واپس ORT_MAKE_حالت(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
      // Reject UTF-16 surrogates (U+D800..U+DFFF)
      اگر (cp >= 0xD800 && cp <= 0xDFFF) {
        واپس ORT_MAKE_حالت(ONNXRUNTIME, FAIL, "Invalid UTF-8: surrogate codepoint");
      }
    } ورنہ اگر ((*src & 0xF8) == 0xF0) {
      byte_len = 4;
      اگر (static_cast<سائز_t>(src_انتہا - src) < 4) {
        واپس ORT_MAKE_حالت(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      اگر ((src[1] & 0xC0) != 0x80 || (src[2] & 0xC0) != 0x80 || (src[3] & 0xC0) != 0x80) {
        واپس ORT_MAKE_حالت(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x07) << 18) |
           (static_cast<char32_t>(src[1] & 0x3F) << 12) |
           (static_cast<char32_t>(src[2] & 0x3F) << 6) |
           static_cast<char32_t>(src[3] & 0x3F);
      // Reject overlong encoding (must be >= 0x10000 for 4-byte)
      اگر (cp < 0x10000) {
        واپس ORT_MAKE_حالت(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
      // Reject codepoints beyond Unicode range
      اگر (cp > 0x10FFFF) {
        واپس ORT_MAKE_حالت(ONNXRUNTIME, FAIL, "Invalid UTF-8: codepoint beyond U+10FFFF");
      }
    } ورنہ {
      واپس ORT_MAKE_حالت(ONNXRUNTIME, FAIL, "Invalid UTF-8 lead byte");
    }

    *dest++ = static_cast<wchar_t>(cp);
    src += byte_len;
  }

  wstr.resize(static_cast<سائز_t>(dest - wstr.ڈیٹا()));
  واپس حالت_3::OK();
}

/// Convenience: convert UTF-8 string to wstring (throws on error).
اندرونی std::wstring Utf8_مقصد_Wide_String(ثابت std::string& s) {
  // UTF-8 byte count is an upper bound on wchar_t count
  std::wstring نتیجہ;
  نتیجہ.resize(s.سائز());
  ORT_THROW_IF_ERROR(Utf8_مقصد_Wide(s, نتیجہ));
  واپس نتیجہ;
}

#اگر_ختم  // !_WIN32

}  // namespace utf8_util
}  // namespace onnxruntime
