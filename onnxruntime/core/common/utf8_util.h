// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#आशय once

#समावेश "core/common/common.h"

#समावेश <string>

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
अंतरभूत bool utf8_bytes(अचिन्हित अक्षर ch, माप_प्रकार& len) {
  अगर ((ch & 0x80) == 0) {
    len = 1;
    वापस true;
  }
  अगर ((ch & 0xE0) == 0xC0) {
    len = 2;
    वापस true;
  }
  अगर ((ch & 0xF0) == 0xE0) {
    len = 3;
    वापस true;
  }
  अगर ((ch & 0xF8) == 0xF0) {
    len = 4;
    वापस true;
  }
  वापस false;
}

// Computes length of the utf8 string in characters
अंतरभूत bool utf8_len(स्थिर अचिन्हित अक्षर* s, माप_प्रकार bytes, माप_प्रकार& len) {
  माप_प्रकार परिणाम = 0;
  जबतक (bytes > 0) {
    माप_प्रकार char_bytes = 0;
    bool valid = utf8_bytes(*s, char_bytes);
    अगर (!valid || bytes < char_bytes) {
      वापस false;
    }
    bytes -= char_bytes;
    s += char_bytes;
    ++परिणाम;
  }
  len = परिणाम;
  वापस true;
}

अंतरभूत bool utf8_validate(स्थिर अचिन्हित अक्षर* s, माप_प्रकार len, माप_प्रकार& utf8_chars) {
  माप_प्रकार utf8_len = 0;
  माप_प्रकार idx = 0;
  जबतक (idx < len) {
    माप_प्रकार bytes = 0;
    स्वतः ch = s[idx];
    अगर (utf8_bytes(ch, bytes)) {
      चयन (bytes) {
        हाल 1:
          अवरोध;
        हाल 2: {
          // Reject overlong 2-byte sequences. Valid Unicode 2-byte encodings
          // start at U+0080, so lead bytes 0xC0 and 0xC1 are invalid.
          अगर (ch < 0xC2u) {
            वापस false;
          }
          अगर (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
            वापस false;
          }
        } अवरोध;  // 2
        हाल 3: {
          स्वतः ch1 = s[idx];
          चयन (ch1) {
            हाल 0xE0u:
              अगर (++idx >= len || s[idx] < 0xA0u || s[idx] > 0xBFu) {
                वापस false;
              }
              अवरोध;
            हाल 0xEDu:
              अगर (++idx >= len || s[idx] < 0x80u || s[idx] > 0x9Fu) {
                वापस false;
              }
              अवरोध;
            शेष: {
              अगर ((ch1 >= 0xE1u && ch1 <= 0xECu) ||
                  (ch1 >= 0xEEu && ch1 <= 0xEFu)) {
                अगर (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
                  वापस false;
                }
              } अन्यथा {
                वापस false;
              }
            } अवरोध;
          }
          // validate byte 3
          अगर (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
            वापस false;
          }
        } अवरोध;  // 3
        हाल 4: {
          स्वतः ch1 = s[idx];
          चयन (ch1) {
            हाल 0xF0u: {
              अगर (++idx >= len || s[idx] < 0x90u || s[idx] > 0xBFu) {
                वापस false;
              }
            } अवरोध;
            हाल 0xF4u: {
              अगर (++idx >= len || s[idx] < 0x80u || s[idx] > 0x8Fu) {
                वापस false;
              }
            } अवरोध;
            शेष: {
              अगर (ch1 >= 0xF1u && ch1 <= 0xF3u) {
                अगर (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
                  वापस false;
                }
              } अन्यथा {
                वापस false;
              }
            } अवरोध;
          }
          // validate bytes 3 and 4
          माप_प्रकार stop = idx + 2;
          जबतक (idx < stop) {
            अगर (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
              वापस false;
            }
          }
        } अवरोध;  // 4
        शेष:
          // no chars longer than 4
          वापस false;
      }  // switch bytes
      ++idx;
      ++utf8_len;
    } अन्यथा {
      वापस false;
    }
  }
  // End index must match
  // the end of the last byte sequence.
  अगर (idx != len) {
    वापस false;
  }
  utf8_chars = utf8_len;
  वापस true;
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
#अगर_अघोषित _WIN32

static_assert(माप(ब_अक्षर_प्रकार) >= 4,
              "Non-Windows UTF-8/wchar_t conversion helpers require wchar_t to be at least 32 bits.");

/// Compute the number of UTF-8 bytes required to encode a wide string.
अंतरभूत माप_प्रकार Wide_To_Utf8_Required_आकार(स्थिर std::wstring& wstr) {
  माप_प्रकार परिणाम = 0;
  क्रम (ब_अक्षर_प्रकार wc : wstr) {
    char32_t cp = static_cast<char32_t>(wc);
    अगर (cp >= 0xD800 && cp <= 0xDFFF) {
      ORT_THROW("Invalid Unicode surrogate codepoint U+", std::hex, static_cast<uint32_t>(cp));
    }
    अगर (cp <= 0x7F) {
      परिणाम += 1;
    } अन्यथा अगर (cp <= 0x7FF) {
      परिणाम += 2;
    } अन्यथा अगर (cp <= 0xFFFF) {
      परिणाम += 3;
    } अन्यथा अगर (cp <= 0x10FFFF) {
      परिणाम += 4;
    } अन्यथा {
      ORT_THROW("Invalid Unicode codepoint U+", std::hex, static_cast<uint32_t>(cp));
    }
  }
  वापस परिणाम;
}

/// Convert a wide string to UTF-8, writing into a pre-allocated std::string.
/// The string is resized to the actual number of bytes written.
अंतरभूत Status WideToUtf8(स्थिर std::wstring& wstr, std::string& str) {
  अगर (wstr.empty()) {
    str.clear();
    वापस Status::OK();
  }

  अक्षर* dest = str.तथ्य();
  अक्षर* dest_end = dest + str.आकार();

  क्रम (ब_अक्षर_प्रकार wc : wstr) {
    char32_t cp = static_cast<char32_t>(wc);
    अगर (cp >= 0xD800 && cp <= 0xDFFF) {
      वापस ORT_MAKE_STATUS(ONNXRUNTIME, FAIL,
                             "Invalid Unicode surrogate codepoint during UTF-8 conversion");
    }
    अगर (cp <= 0x7F) {
      स्थिर माप_प्रकार remaining = static_cast<माप_प्रकार>(dest_end - dest);
      अगर (remaining < 1) {
        वापस ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<अक्षर>(cp);
    } अन्यथा अगर (cp <= 0x7FF) {
      स्थिर माप_प्रकार remaining = static_cast<माप_प्रकार>(dest_end - dest);
      अगर (remaining < 2) {
        वापस ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<अक्षर>(0xC0 | (cp >> 6));
      *dest++ = static_cast<अक्षर>(0x80 | (cp & 0x3F));
    } अन्यथा अगर (cp <= 0xFFFF) {
      स्थिर माप_प्रकार remaining = static_cast<माप_प्रकार>(dest_end - dest);
      अगर (remaining < 3) {
        वापस ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<अक्षर>(0xE0 | (cp >> 12));
      *dest++ = static_cast<अक्षर>(0x80 | ((cp >> 6) & 0x3F));
      *dest++ = static_cast<अक्षर>(0x80 | (cp & 0x3F));
    } अन्यथा अगर (cp <= 0x10FFFF) {
      स्थिर माप_प्रकार remaining = static_cast<माप_प्रकार>(dest_end - dest);
      अगर (remaining < 4) {
        वापस ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<अक्षर>(0xF0 | (cp >> 18));
      *dest++ = static_cast<अक्षर>(0x80 | ((cp >> 12) & 0x3F));
      *dest++ = static_cast<अक्षर>(0x80 | ((cp >> 6) & 0x3F));
      *dest++ = static_cast<अक्षर>(0x80 | (cp & 0x3F));
    } अन्यथा {
      वापस ORT_MAKE_STATUS(ONNXRUNTIME, FAIL,
                             "Invalid Unicode codepoint during UTF-8 conversion");
    }
  }

  str.resize(static_cast<माप_प्रकार>(dest - str.तथ्य()));
  वापस Status::OK();
}

/// Convert a UTF-8 string to a wide string, writing into a pre-allocated std::wstring.
/// The wstring is resized to the actual number of characters written.
/// Validates continuation bytes and rejects overlong encodings and surrogates.
अंतरभूत Status Utf8ToWide(स्थिर std::string& str, std::wstring& wstr) {
  अगर (str.empty()) {
    wstr.clear();
    वापस Status::OK();
  }

  अगर (wstr.आकार() < str.आकार()) {
    wstr.resize(str.आकार());
  }

  स्थिर स्वतः* src = reinterpret_cast<स्थिर अचिन्हित अक्षर*>(str.तथ्य());
  स्थिर स्वतः* src_end = src + str.आकार();
  ब_अक्षर_प्रकार* dest = wstr.तथ्य();

  जबतक (src < src_end) {
    char32_t cp = 0;
    माप_प्रकार byte_len = 0;

    अगर ((*src & 0x80) == 0) {
      cp = *src;
      byte_len = 1;
    } अन्यथा अगर ((*src & 0xE0) == 0xC0) {
      byte_len = 2;
      अगर (static_cast<माप_प्रकार>(src_end - src) < 2) {
        वापस ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      अगर ((src[1] & 0xC0) != 0x80) {
        वापस ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x1F) << 6) |
           static_cast<char32_t>(src[1] & 0x3F);
      // Reject overlong encoding (must be >= 0x80 for 2-byte)
      अगर (cp < 0x80) {
        वापस ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
    } अन्यथा अगर ((*src & 0xF0) == 0xE0) {
      byte_len = 3;
      अगर (static_cast<माप_प्रकार>(src_end - src) < 3) {
        वापस ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      अगर ((src[1] & 0xC0) != 0x80 || (src[2] & 0xC0) != 0x80) {
        वापस ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x0F) << 12) |
           (static_cast<char32_t>(src[1] & 0x3F) << 6) |
           static_cast<char32_t>(src[2] & 0x3F);
      // Reject overlong encoding (must be >= 0x800 for 3-byte)
      अगर (cp < 0x800) {
        वापस ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
      // Reject UTF-16 surrogates (U+D800..U+DFFF)
      अगर (cp >= 0xD800 && cp <= 0xDFFF) {
        वापस ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8: surrogate codepoint");
      }
    } अन्यथा अगर ((*src & 0xF8) == 0xF0) {
      byte_len = 4;
      अगर (static_cast<माप_प्रकार>(src_end - src) < 4) {
        वापस ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      अगर ((src[1] & 0xC0) != 0x80 || (src[2] & 0xC0) != 0x80 || (src[3] & 0xC0) != 0x80) {
        वापस ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x07) << 18) |
           (static_cast<char32_t>(src[1] & 0x3F) << 12) |
           (static_cast<char32_t>(src[2] & 0x3F) << 6) |
           static_cast<char32_t>(src[3] & 0x3F);
      // Reject overlong encoding (must be >= 0x10000 for 4-byte)
      अगर (cp < 0x10000) {
        वापस ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
      // Reject codepoints beyond Unicode range
      अगर (cp > 0x10FFFF) {
        वापस ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8: codepoint beyond U+10FFFF");
      }
    } अन्यथा {
      वापस ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8 lead byte");
    }

    *dest++ = static_cast<ब_अक्षर_प्रकार>(cp);
    src += byte_len;
  }

  wstr.resize(static_cast<माप_प्रकार>(dest - wstr.तथ्य()));
  वापस Status::OK();
}

/// Convenience: convert UTF-8 string to wstring (throws on error).
अंतरभूत std::wstring Utf8ToWideString(स्थिर std::string& s) {
  // UTF-8 byte count is an upper bound on wchar_t count
  std::wstring परिणाम;
  परिणाम.resize(s.आकार());
  ORT_THROW_IF_ERROR(Utf8ToWide(s, परिणाम));
  वापस परिणाम;
}

#पूर्ण_अगर  // !_WIN32

}  // namespace utf8_util
}  // namespace onnxruntime
