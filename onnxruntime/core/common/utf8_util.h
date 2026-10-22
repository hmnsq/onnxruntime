// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#निर्देशः once

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
अन्तःस्थ bool utf8_अष्टकानि(अचिह्नित वर्ण ch, आकार_t& len) {
  यदि ((ch & 0x80) == 0) {
    len = 1;
    प्रत्यागम true;
  }
  यदि ((ch & 0xE0) == 0xC0) {
    len = 2;
    प्रत्यागम true;
  }
  यदि ((ch & 0xF0) == 0xE0) {
    len = 3;
    प्रत्यागम true;
  }
  यदि ((ch & 0xF8) == 0xF0) {
    len = 4;
    प्रत्यागम true;
  }
  प्रत्यागम false;
}

// Computes length of the utf8 string in characters
अन्तःस्थ bool utf8_len(ध्रुव अचिह्नित वर्ण* s, आकार_t अष्टकानि, आकार_t& len) {
  आकार_t परिणाम = 0;
  यावत् (अष्टकानि > 0) {
    आकार_t char_अष्टकानि = 0;
    bool valid = utf8_अष्टकानि(*s, char_अष्टकानि);
    यदि (!valid || अष्टकानि < char_अष्टकानि) {
      प्रत्यागम false;
    }
    अष्टकानि -= char_अष्टकानि;
    s += char_अष्टकानि;
    ++परिणाम;
  }
  len = परिणाम;
  प्रत्यागम true;
}

अन्तःस्थ bool utf8_validate(ध्रुव अचिह्नित वर्ण* s, आकार_t len, आकार_t& utf8_chars) {
  आकार_t utf8_len = 0;
  आकार_t idx = 0;
  यावत् (idx < len) {
    आकार_t अष्टकानि = 0;
    स्वचालित ch = s[idx];
    यदि (utf8_अष्टकानि(ch, अष्टकानि)) {
      विकल्प (अष्टकानि) {
        पक्ष 1:
          भङ्ग;
        पक्ष 2: {
          // Reject overlong 2-byte sequences. Valid Unicode 2-byte encodings
          // start at U+0080, so lead bytes 0xC0 and 0xC1 are invalid.
          यदि (ch < 0xC2u) {
            प्रत्यागम false;
          }
          यदि (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
            प्रत्यागम false;
          }
        } भङ्ग;  // 2
        पक्ष 3: {
          स्वचालित ch1 = s[idx];
          विकल्प (ch1) {
            पक्ष 0xE0u:
              यदि (++idx >= len || s[idx] < 0xA0u || s[idx] > 0xBFu) {
                प्रत्यागम false;
              }
              भङ्ग;
            पक्ष 0xEDu:
              यदि (++idx >= len || s[idx] < 0x80u || s[idx] > 0x9Fu) {
                प्रत्यागम false;
              }
              भङ्ग;
            स्वभाव: {
              यदि ((ch1 >= 0xE1u && ch1 <= 0xECu) ||
                  (ch1 >= 0xEEu && ch1 <= 0xEFu)) {
                यदि (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
                  प्रत्यागम false;
                }
              } अन्यथा {
                प्रत्यागम false;
              }
            } भङ्ग;
          }
          // validate byte 3
          यदि (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
            प्रत्यागम false;
          }
        } भङ्ग;  // 3
        पक्ष 4: {
          स्वचालित ch1 = s[idx];
          विकल्प (ch1) {
            पक्ष 0xF0u: {
              यदि (++idx >= len || s[idx] < 0x90u || s[idx] > 0xBFu) {
                प्रत्यागम false;
              }
            } भङ्ग;
            पक्ष 0xF4u: {
              यदि (++idx >= len || s[idx] < 0x80u || s[idx] > 0x8Fu) {
                प्रत्यागम false;
              }
            } भङ्ग;
            स्वभाव: {
              यदि (ch1 >= 0xF1u && ch1 <= 0xF3u) {
                यदि (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
                  प्रत्यागम false;
                }
              } अन्यथा {
                प्रत्यागम false;
              }
            } भङ्ग;
          }
          // validate bytes 3 and 4
          आकार_t stop = idx + 2;
          यावत् (idx < stop) {
            यदि (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
              प्रत्यागम false;
            }
          }
        } भङ्ग;  // 4
        स्वभाव:
          // no chars longer than 4
          प्रत्यागम false;
      }  // switch bytes
      ++idx;
      ++utf8_len;
    } अन्यथा {
      प्रत्यागम false;
    }
  }
  // End index must match
  // the end of the last byte sequence.
  यदि (idx != len) {
    प्रत्यागम false;
  }
  utf8_chars = utf8_len;
  प्रत्यागम true;
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
#यदि_अपरिभाषितम् _WIN32

static_assert(परिमाण(wchar_t) >= 4,
              "Non-Windows UTF-8/wchar_t conversion helpers require wchar_t to be at least 32 bits.");

/// Compute the number of UTF-8 bytes required to encode a wide string.
अन्तःस्थ आकार_t Wide_गन्तव्यम्_Utf8_Required_आकार(ध्रुव std::wstring& wstr) {
  आकार_t परिणाम = 0;
  प्रति (wchar_t wc : wstr) {
    char32_t cp = static_cast<char32_t>(wc);
    यदि (cp >= 0xD800 && cp <= 0xDFFF) {
      ORT_THROW("Invalid Unicode surrogate codepoint U+", std::hex, static_cast<uint32_t>(cp));
    }
    यदि (cp <= 0x7F) {
      परिणाम += 1;
    } अन्यथा यदि (cp <= 0x7FF) {
      परिणाम += 2;
    } अन्यथा यदि (cp <= 0xFFFF) {
      परिणाम += 3;
    } अन्यथा यदि (cp <= 0x10FFFF) {
      परिणाम += 4;
    } अन्यथा {
      ORT_THROW("Invalid Unicode codepoint U+", std::hex, static_cast<uint32_t>(cp));
    }
  }
  प्रत्यागम परिणाम;
}

/// Convert a wide string to UTF-8, writing into a pre-allocated std::string.
/// The string is resized to the actual number of bytes written.
अन्तःस्थ स्थितिः_2 Wide_गन्तव्यम्_Utf8(ध्रुव std::wstring& wstr, std::string& str) {
  यदि (wstr.रिक्तानि()) {
    str.clear();
    प्रत्यागम स्थितिः_2::OK();
  }

  वर्ण* dest = str.दत्त();
  वर्ण* dest_अन्तः = dest + str.आकार();

  प्रति (wchar_t wc : wstr) {
    char32_t cp = static_cast<char32_t>(wc);
    यदि (cp >= 0xD800 && cp <= 0xDFFF) {
      प्रत्यागम ORT_MAKE_स्थितिः(ONNXRUNTIME, FAIL,
                             "Invalid Unicode surrogate codepoint during UTF-8 conversion");
    }
    यदि (cp <= 0x7F) {
      ध्रुव आकार_t remaining = static_cast<आकार_t>(dest_अन्तः - dest);
      यदि (remaining < 1) {
        प्रत्यागम ORT_MAKE_स्थितिः(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<वर्ण>(cp);
    } अन्यथा यदि (cp <= 0x7FF) {
      ध्रुव आकार_t remaining = static_cast<आकार_t>(dest_अन्तः - dest);
      यदि (remaining < 2) {
        प्रत्यागम ORT_MAKE_स्थितिः(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<वर्ण>(0xC0 | (cp >> 6));
      *dest++ = static_cast<वर्ण>(0x80 | (cp & 0x3F));
    } अन्यथा यदि (cp <= 0xFFFF) {
      ध्रुव आकार_t remaining = static_cast<आकार_t>(dest_अन्तः - dest);
      यदि (remaining < 3) {
        प्रत्यागम ORT_MAKE_स्थितिः(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<वर्ण>(0xE0 | (cp >> 12));
      *dest++ = static_cast<वर्ण>(0x80 | ((cp >> 6) & 0x3F));
      *dest++ = static_cast<वर्ण>(0x80 | (cp & 0x3F));
    } अन्यथा यदि (cp <= 0x10FFFF) {
      ध्रुव आकार_t remaining = static_cast<आकार_t>(dest_अन्तः - dest);
      यदि (remaining < 4) {
        प्रत्यागम ORT_MAKE_स्थितिः(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<वर्ण>(0xF0 | (cp >> 18));
      *dest++ = static_cast<वर्ण>(0x80 | ((cp >> 12) & 0x3F));
      *dest++ = static_cast<वर्ण>(0x80 | ((cp >> 6) & 0x3F));
      *dest++ = static_cast<वर्ण>(0x80 | (cp & 0x3F));
    } अन्यथा {
      प्रत्यागम ORT_MAKE_स्थितिः(ONNXRUNTIME, FAIL,
                             "Invalid Unicode codepoint during UTF-8 conversion");
    }
  }

  str.resize(static_cast<आकार_t>(dest - str.दत्त()));
  प्रत्यागम स्थितिः_2::OK();
}

/// Convert a UTF-8 string to a wide string, writing into a pre-allocated std::wstring.
/// The wstring is resized to the actual number of characters written.
/// Validates continuation bytes and rejects overlong encodings and surrogates.
अन्तःस्थ स्थितिः_2 Utf8_गन्तव्यम्_Wide(ध्रुव std::string& str, std::wstring& wstr) {
  यदि (str.रिक्तानि()) {
    wstr.clear();
    प्रत्यागम स्थितिः_2::OK();
  }

  यदि (wstr.आकार() < str.आकार()) {
    wstr.resize(str.आकार());
  }

  ध्रुव स्वचालित* src = reinterpret_cast<ध्रुव अचिह्नित वर्ण*>(str.दत्त());
  ध्रुव स्वचालित* src_अन्तः = src + str.आकार();
  wchar_t* dest = wstr.दत्त();

  यावत् (src < src_अन्तः) {
    char32_t cp = 0;
    आकार_t byte_len = 0;

    यदि ((*src & 0x80) == 0) {
      cp = *src;
      byte_len = 1;
    } अन्यथा यदि ((*src & 0xE0) == 0xC0) {
      byte_len = 2;
      यदि (static_cast<आकार_t>(src_अन्तः - src) < 2) {
        प्रत्यागम ORT_MAKE_स्थितिः(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      यदि ((src[1] & 0xC0) != 0x80) {
        प्रत्यागम ORT_MAKE_स्थितिः(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x1F) << 6) |
           static_cast<char32_t>(src[1] & 0x3F);
      // Reject overlong encoding (must be >= 0x80 for 2-byte)
      यदि (cp < 0x80) {
        प्रत्यागम ORT_MAKE_स्थितिः(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
    } अन्यथा यदि ((*src & 0xF0) == 0xE0) {
      byte_len = 3;
      यदि (static_cast<आकार_t>(src_अन्तः - src) < 3) {
        प्रत्यागम ORT_MAKE_स्थितिः(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      यदि ((src[1] & 0xC0) != 0x80 || (src[2] & 0xC0) != 0x80) {
        प्रत्यागम ORT_MAKE_स्थितिः(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x0F) << 12) |
           (static_cast<char32_t>(src[1] & 0x3F) << 6) |
           static_cast<char32_t>(src[2] & 0x3F);
      // Reject overlong encoding (must be >= 0x800 for 3-byte)
      यदि (cp < 0x800) {
        प्रत्यागम ORT_MAKE_स्थितिः(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
      // Reject UTF-16 surrogates (U+D800..U+DFFF)
      यदि (cp >= 0xD800 && cp <= 0xDFFF) {
        प्रत्यागम ORT_MAKE_स्थितिः(ONNXRUNTIME, FAIL, "Invalid UTF-8: surrogate codepoint");
      }
    } अन्यथा यदि ((*src & 0xF8) == 0xF0) {
      byte_len = 4;
      यदि (static_cast<आकार_t>(src_अन्तः - src) < 4) {
        प्रत्यागम ORT_MAKE_स्थितिः(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      यदि ((src[1] & 0xC0) != 0x80 || (src[2] & 0xC0) != 0x80 || (src[3] & 0xC0) != 0x80) {
        प्रत्यागम ORT_MAKE_स्थितिः(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x07) << 18) |
           (static_cast<char32_t>(src[1] & 0x3F) << 12) |
           (static_cast<char32_t>(src[2] & 0x3F) << 6) |
           static_cast<char32_t>(src[3] & 0x3F);
      // Reject overlong encoding (must be >= 0x10000 for 4-byte)
      यदि (cp < 0x10000) {
        प्रत्यागम ORT_MAKE_स्थितिः(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
      // Reject codepoints beyond Unicode range
      यदि (cp > 0x10FFFF) {
        प्रत्यागम ORT_MAKE_स्थितिः(ONNXRUNTIME, FAIL, "Invalid UTF-8: codepoint beyond U+10FFFF");
      }
    } अन्यथा {
      प्रत्यागम ORT_MAKE_स्थितिः(ONNXRUNTIME, FAIL, "Invalid UTF-8 lead byte");
    }

    *dest++ = static_cast<wchar_t>(cp);
    src += byte_len;
  }

  wstr.resize(static_cast<आकार_t>(dest - wstr.दत्त()));
  प्रत्यागम स्थितिः_2::OK();
}

/// Convenience: convert UTF-8 string to wstring (throws on error).
अन्तःस्थ std::wstring Utf8_गन्तव्यम्_Wide_String(ध्रुव std::string& s) {
  // UTF-8 byte count is an upper bound on wchar_t count
  std::wstring परिणाम;
  परिणाम.resize(s.आकार());
  ORT_THROW_IF_ERROR(Utf8_गन्तव्यम्_Wide(s, परिणाम));
  प्रत्यागम परिणाम;
}

#यदि_अन्तः  // !_WIN32

}  // namespace utf8_util
}  // namespace onnxruntime
