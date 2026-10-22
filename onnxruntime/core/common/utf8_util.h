// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

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
अन्तःस्थ bool utf8_bytes(अचिन्हित वर्ण ch, माप_प्रकार& len) {
  यदि ((ch & 0x80) == 0) {
    len = 1;
    फर्काऊ true;
  }
  यदि ((ch & 0xE0) == 0xC0) {
    len = 2;
    फर्काऊ true;
  }
  यदि ((ch & 0xF0) == 0xE0) {
    len = 3;
    फर्काऊ true;
  }
  यदि ((ch & 0xF8) == 0xF0) {
    len = 4;
    फर्काऊ true;
  }
  फर्काऊ false;
}

// Computes length of the utf8 string in characters
अन्तःस्थ bool utf8_len(अचर अचिन्हित वर्ण* s, माप_प्रकार bytes, माप_प्रकार& len) {
  माप_प्रकार result = 0;
  जबसम्म (bytes > 0) {
    माप_प्रकार char_bytes = 0;
    bool valid = utf8_bytes(*s, char_bytes);
    यदि (!valid || bytes < char_bytes) {
      फर्काऊ false;
    }
    bytes -= char_bytes;
    s += char_bytes;
    ++result;
  }
  len = result;
  फर्काऊ true;
}

अन्तःस्थ bool utf8_validate(अचर अचिन्हित वर्ण* s, माप_प्रकार len, माप_प्रकार& utf8_chars) {
  माप_प्रकार utf8_len = 0;
  माप_प्रकार idx = 0;
  जबसम्म (idx < len) {
    माप_प्रकार bytes = 0;
    स्वतः ch = s[idx];
    यदि (utf8_bytes(ch, bytes)) {
      विकल्प (bytes) {
        पक्ष 1:
          रोक;
        पक्ष 2: {
          // Reject overlong 2-byte sequences. Valid Unicode 2-byte encodings
          // start at U+0080, so lead bytes 0xC0 and 0xC1 are invalid.
          यदि (ch < 0xC2u) {
            फर्काऊ false;
          }
          यदि (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
            फर्काऊ false;
          }
        } रोक;  // 2
        पक्ष 3: {
          स्वतः ch1 = s[idx];
          विकल्प (ch1) {
            पक्ष 0xE0u:
              यदि (++idx >= len || s[idx] < 0xA0u || s[idx] > 0xBFu) {
                फर्काऊ false;
              }
              रोक;
            पक्ष 0xEDu:
              यदि (++idx >= len || s[idx] < 0x80u || s[idx] > 0x9Fu) {
                फर्काऊ false;
              }
              रोक;
            स्वभाव: {
              यदि ((ch1 >= 0xE1u && ch1 <= 0xECu) ||
                  (ch1 >= 0xEEu && ch1 <= 0xEFu)) {
                यदि (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
                  फर्काऊ false;
                }
              } अन्यथा {
                फर्काऊ false;
              }
            } रोक;
          }
          // validate byte 3
          यदि (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
            फर्काऊ false;
          }
        } रोक;  // 3
        पक्ष 4: {
          स्वतः ch1 = s[idx];
          विकल्प (ch1) {
            पक्ष 0xF0u: {
              यदि (++idx >= len || s[idx] < 0x90u || s[idx] > 0xBFu) {
                फर्काऊ false;
              }
            } रोक;
            पक्ष 0xF4u: {
              यदि (++idx >= len || s[idx] < 0x80u || s[idx] > 0x8Fu) {
                फर्काऊ false;
              }
            } रोक;
            स्वभाव: {
              यदि (ch1 >= 0xF1u && ch1 <= 0xF3u) {
                यदि (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
                  फर्काऊ false;
                }
              } अन्यथा {
                फर्काऊ false;
              }
            } रोक;
          }
          // validate bytes 3 and 4
          माप_प्रकार stop = idx + 2;
          जबसम्म (idx < stop) {
            यदि (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
              फर्काऊ false;
            }
          }
        } रोक;  // 4
        स्वभाव:
          // no chars longer than 4
          फर्काऊ false;
      }  // switch bytes
      ++idx;
      ++utf8_len;
    } अन्यथा {
      फर्काऊ false;
    }
  }
  // End index must match
  // the end of the last byte sequence.
  यदि (idx != len) {
    फर्काऊ false;
  }
  utf8_chars = utf8_len;
  फर्काऊ true;
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

static_assert(परिमाण(ब_अक्षर_प्रकार) >= 4,
              "Non-Windows UTF-8/wchar_t conversion helpers require wchar_t to be at least 32 bits.");

/// Compute the number of UTF-8 bytes required to encode a wide string.
अन्तःस्थ माप_प्रकार WideToUtf8RequiredSize(अचर std::wstring& wstr) {
  माप_प्रकार result = 0;
  लागि (ब_अक्षर_प्रकार wc : wstr) {
    char32_t cp = static_cast<char32_t>(wc);
    यदि (cp >= 0xD800 && cp <= 0xDFFF) {
      ORT_THROW("Invalid Unicode surrogate codepoint U+", std::hex, static_cast<uint32_t>(cp));
    }
    यदि (cp <= 0x7F) {
      result += 1;
    } अन्यथा यदि (cp <= 0x7FF) {
      result += 2;
    } अन्यथा यदि (cp <= 0xFFFF) {
      result += 3;
    } अन्यथा यदि (cp <= 0x10FFFF) {
      result += 4;
    } अन्यथा {
      ORT_THROW("Invalid Unicode codepoint U+", std::hex, static_cast<uint32_t>(cp));
    }
  }
  फर्काऊ result;
}

/// Convert a wide string to UTF-8, writing into a pre-allocated std::string.
/// The string is resized to the actual number of bytes written.
अन्तःस्थ Status WideToUtf8(अचर std::wstring& wstr, std::string& str) {
  यदि (wstr.empty()) {
    str.clear();
    फर्काऊ Status::OK();
  }

  वर्ण* dest = str.data();
  वर्ण* dest_end = dest + str.size();

  लागि (ब_अक्षर_प्रकार wc : wstr) {
    char32_t cp = static_cast<char32_t>(wc);
    यदि (cp >= 0xD800 && cp <= 0xDFFF) {
      फर्काऊ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL,
                             "Invalid Unicode surrogate codepoint during UTF-8 conversion");
    }
    यदि (cp <= 0x7F) {
      अचर माप_प्रकार remaining = static_cast<माप_प्रकार>(dest_end - dest);
      यदि (remaining < 1) {
        फर्काऊ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<वर्ण>(cp);
    } अन्यथा यदि (cp <= 0x7FF) {
      अचर माप_प्रकार remaining = static_cast<माप_प्रकार>(dest_end - dest);
      यदि (remaining < 2) {
        फर्काऊ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<वर्ण>(0xC0 | (cp >> 6));
      *dest++ = static_cast<वर्ण>(0x80 | (cp & 0x3F));
    } अन्यथा यदि (cp <= 0xFFFF) {
      अचर माप_प्रकार remaining = static_cast<माप_प्रकार>(dest_end - dest);
      यदि (remaining < 3) {
        फर्काऊ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<वर्ण>(0xE0 | (cp >> 12));
      *dest++ = static_cast<वर्ण>(0x80 | ((cp >> 6) & 0x3F));
      *dest++ = static_cast<वर्ण>(0x80 | (cp & 0x3F));
    } अन्यथा यदि (cp <= 0x10FFFF) {
      अचर माप_प्रकार remaining = static_cast<माप_प्रकार>(dest_end - dest);
      यदि (remaining < 4) {
        फर्काऊ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<वर्ण>(0xF0 | (cp >> 18));
      *dest++ = static_cast<वर्ण>(0x80 | ((cp >> 12) & 0x3F));
      *dest++ = static_cast<वर्ण>(0x80 | ((cp >> 6) & 0x3F));
      *dest++ = static_cast<वर्ण>(0x80 | (cp & 0x3F));
    } अन्यथा {
      फर्काऊ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL,
                             "Invalid Unicode codepoint during UTF-8 conversion");
    }
  }

  str.resize(static_cast<माप_प्रकार>(dest - str.data()));
  फर्काऊ Status::OK();
}

/// Convert a UTF-8 string to a wide string, writing into a pre-allocated std::wstring.
/// The wstring is resized to the actual number of characters written.
/// Validates continuation bytes and rejects overlong encodings and surrogates.
अन्तःस्थ Status Utf8ToWide(अचर std::string& str, std::wstring& wstr) {
  यदि (str.empty()) {
    wstr.clear();
    फर्काऊ Status::OK();
  }

  यदि (wstr.size() < str.size()) {
    wstr.resize(str.size());
  }

  अचर स्वतः* src = reinterpret_cast<अचर अचिन्हित वर्ण*>(str.data());
  अचर स्वतः* src_end = src + str.size();
  ब_अक्षर_प्रकार* dest = wstr.data();

  जबसम्म (src < src_end) {
    char32_t cp = 0;
    माप_प्रकार byte_len = 0;

    यदि ((*src & 0x80) == 0) {
      cp = *src;
      byte_len = 1;
    } अन्यथा यदि ((*src & 0xE0) == 0xC0) {
      byte_len = 2;
      यदि (static_cast<माप_प्रकार>(src_end - src) < 2) {
        फर्काऊ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      यदि ((src[1] & 0xC0) != 0x80) {
        फर्काऊ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x1F) << 6) |
           static_cast<char32_t>(src[1] & 0x3F);
      // Reject overlong encoding (must be >= 0x80 for 2-byte)
      यदि (cp < 0x80) {
        फर्काऊ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
    } अन्यथा यदि ((*src & 0xF0) == 0xE0) {
      byte_len = 3;
      यदि (static_cast<माप_प्रकार>(src_end - src) < 3) {
        फर्काऊ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      यदि ((src[1] & 0xC0) != 0x80 || (src[2] & 0xC0) != 0x80) {
        फर्काऊ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x0F) << 12) |
           (static_cast<char32_t>(src[1] & 0x3F) << 6) |
           static_cast<char32_t>(src[2] & 0x3F);
      // Reject overlong encoding (must be >= 0x800 for 3-byte)
      यदि (cp < 0x800) {
        फर्काऊ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
      // Reject UTF-16 surrogates (U+D800..U+DFFF)
      यदि (cp >= 0xD800 && cp <= 0xDFFF) {
        फर्काऊ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8: surrogate codepoint");
      }
    } अन्यथा यदि ((*src & 0xF8) == 0xF0) {
      byte_len = 4;
      यदि (static_cast<माप_प्रकार>(src_end - src) < 4) {
        फर्काऊ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      यदि ((src[1] & 0xC0) != 0x80 || (src[2] & 0xC0) != 0x80 || (src[3] & 0xC0) != 0x80) {
        फर्काऊ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x07) << 18) |
           (static_cast<char32_t>(src[1] & 0x3F) << 12) |
           (static_cast<char32_t>(src[2] & 0x3F) << 6) |
           static_cast<char32_t>(src[3] & 0x3F);
      // Reject overlong encoding (must be >= 0x10000 for 4-byte)
      यदि (cp < 0x10000) {
        फर्काऊ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
      // Reject codepoints beyond Unicode range
      यदि (cp > 0x10FFFF) {
        फर्काऊ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8: codepoint beyond U+10FFFF");
      }
    } अन्यथा {
      फर्काऊ ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8 lead byte");
    }

    *dest++ = static_cast<ब_अक्षर_प्रकार>(cp);
    src += byte_len;
  }

  wstr.resize(static_cast<माप_प्रकार>(dest - wstr.data()));
  फर्काऊ Status::OK();
}

/// Convenience: convert UTF-8 string to wstring (throws on error).
अन्तःस्थ std::wstring Utf8ToWideString(अचर std::string& s) {
  // UTF-8 byte count is an upper bound on wchar_t count
  std::wstring result;
  result.resize(s.size());
  ORT_THROW_IF_ERROR(Utf8ToWide(s, result));
  फर्काऊ result;
}

#endif  // !_WIN32

}  // namespace utf8_util
}  // namespace onnxruntime
