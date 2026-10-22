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
ओळीत bool utf8_bytes(अचिन्हित अक्षर ch, माप_प्रकार& len) {
  जर ((ch & 0x80) == 0) {
    len = 1;
    परत true;
  }
  जर ((ch & 0xE0) == 0xC0) {
    len = 2;
    परत true;
  }
  जर ((ch & 0xF0) == 0xE0) {
    len = 3;
    परत true;
  }
  जर ((ch & 0xF8) == 0xF0) {
    len = 4;
    परत true;
  }
  परत false;
}

// Computes length of the utf8 string in characters
ओळीत bool utf8_len(स्थिरांक अचिन्हित अक्षर* s, माप_प्रकार bytes, माप_प्रकार& len) {
  माप_प्रकार result = 0;
  पर्यंत (bytes > 0) {
    माप_प्रकार char_bytes = 0;
    bool valid = utf8_bytes(*s, char_bytes);
    जर (!valid || bytes < char_bytes) {
      परत false;
    }
    bytes -= char_bytes;
    s += char_bytes;
    ++result;
  }
  len = result;
  परत true;
}

ओळीत bool utf8_validate(स्थिरांक अचिन्हित अक्षर* s, माप_प्रकार len, माप_प्रकार& utf8_chars) {
  माप_प्रकार utf8_len = 0;
  माप_प्रकार idx = 0;
  पर्यंत (idx < len) {
    माप_प्रकार bytes = 0;
    स्वतः ch = s[idx];
    जर (utf8_bytes(ch, bytes)) {
      निवड (bytes) {
        स्थिती 1:
          थांबा;
        स्थिती 2: {
          // Reject overlong 2-byte sequences. Valid Unicode 2-byte encodings
          // start at U+0080, so lead bytes 0xC0 and 0xC1 are invalid.
          जर (ch < 0xC2u) {
            परत false;
          }
          जर (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
            परत false;
          }
        } थांबा;  // 2
        स्थिती 3: {
          स्वतः ch1 = s[idx];
          निवड (ch1) {
            स्थिती 0xE0u:
              जर (++idx >= len || s[idx] < 0xA0u || s[idx] > 0xBFu) {
                परत false;
              }
              थांबा;
            स्थिती 0xEDu:
              जर (++idx >= len || s[idx] < 0x80u || s[idx] > 0x9Fu) {
                परत false;
              }
              थांबा;
            पूर्वनिर्धारित: {
              जर ((ch1 >= 0xE1u && ch1 <= 0xECu) ||
                  (ch1 >= 0xEEu && ch1 <= 0xEFu)) {
                जर (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
                  परत false;
                }
              } अन्यथा {
                परत false;
              }
            } थांबा;
          }
          // validate byte 3
          जर (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
            परत false;
          }
        } थांबा;  // 3
        स्थिती 4: {
          स्वतः ch1 = s[idx];
          निवड (ch1) {
            स्थिती 0xF0u: {
              जर (++idx >= len || s[idx] < 0x90u || s[idx] > 0xBFu) {
                परत false;
              }
            } थांबा;
            स्थिती 0xF4u: {
              जर (++idx >= len || s[idx] < 0x80u || s[idx] > 0x8Fu) {
                परत false;
              }
            } थांबा;
            पूर्वनिर्धारित: {
              जर (ch1 >= 0xF1u && ch1 <= 0xF3u) {
                जर (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
                  परत false;
                }
              } अन्यथा {
                परत false;
              }
            } थांबा;
          }
          // validate bytes 3 and 4
          माप_प्रकार stop = idx + 2;
          पर्यंत (idx < stop) {
            जर (++idx >= len || s[idx] < 0x80u || s[idx] > 0xBFu) {
              परत false;
            }
          }
        } थांबा;  // 4
        पूर्वनिर्धारित:
          // no chars longer than 4
          परत false;
      }  // switch bytes
      ++idx;
      ++utf8_len;
    } अन्यथा {
      परत false;
    }
  }
  // End index must match
  // the end of the last byte sequence.
  जर (idx != len) {
    परत false;
  }
  utf8_chars = utf8_len;
  परत true;
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

static_assert(आकार(ब_अक्षर_प्रकार) >= 4,
              "Non-Windows UTF-8/wchar_t conversion helpers require wchar_t to be at least 32 bits.");

/// Compute the number of UTF-8 bytes required to encode a wide string.
ओळीत माप_प्रकार WideToUtf8RequiredSize(स्थिरांक std::wstring& wstr) {
  माप_प्रकार result = 0;
  साठी (ब_अक्षर_प्रकार wc : wstr) {
    char32_t cp = static_cast<char32_t>(wc);
    जर (cp >= 0xD800 && cp <= 0xDFFF) {
      ORT_THROW("Invalid Unicode surrogate codepoint U+", std::hex, static_cast<uint32_t>(cp));
    }
    जर (cp <= 0x7F) {
      result += 1;
    } अन्यथा जर (cp <= 0x7FF) {
      result += 2;
    } अन्यथा जर (cp <= 0xFFFF) {
      result += 3;
    } अन्यथा जर (cp <= 0x10FFFF) {
      result += 4;
    } अन्यथा {
      ORT_THROW("Invalid Unicode codepoint U+", std::hex, static_cast<uint32_t>(cp));
    }
  }
  परत result;
}

/// Convert a wide string to UTF-8, writing into a pre-allocated std::string.
/// The string is resized to the actual number of bytes written.
ओळीत Status WideToUtf8(स्थिरांक std::wstring& wstr, std::string& str) {
  जर (wstr.empty()) {
    str.clear();
    परत Status::OK();
  }

  अक्षर* dest = str.data();
  अक्षर* dest_end = dest + str.size();

  साठी (ब_अक्षर_प्रकार wc : wstr) {
    char32_t cp = static_cast<char32_t>(wc);
    जर (cp >= 0xD800 && cp <= 0xDFFF) {
      परत ORT_MAKE_STATUS(ONNXRUNTIME, FAIL,
                             "Invalid Unicode surrogate codepoint during UTF-8 conversion");
    }
    जर (cp <= 0x7F) {
      स्थिरांक माप_प्रकार remaining = static_cast<माप_प्रकार>(dest_end - dest);
      जर (remaining < 1) {
        परत ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<अक्षर>(cp);
    } अन्यथा जर (cp <= 0x7FF) {
      स्थिरांक माप_प्रकार remaining = static_cast<माप_प्रकार>(dest_end - dest);
      जर (remaining < 2) {
        परत ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<अक्षर>(0xC0 | (cp >> 6));
      *dest++ = static_cast<अक्षर>(0x80 | (cp & 0x3F));
    } अन्यथा जर (cp <= 0xFFFF) {
      स्थिरांक माप_प्रकार remaining = static_cast<माप_प्रकार>(dest_end - dest);
      जर (remaining < 3) {
        परत ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<अक्षर>(0xE0 | (cp >> 12));
      *dest++ = static_cast<अक्षर>(0x80 | ((cp >> 6) & 0x3F));
      *dest++ = static_cast<अक्षर>(0x80 | (cp & 0x3F));
    } अन्यथा जर (cp <= 0x10FFFF) {
      स्थिरांक माप_प्रकार remaining = static_cast<माप_प्रकार>(dest_end - dest);
      जर (remaining < 4) {
        परत ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Destination buffer too small for UTF-8 conversion");
      }
      *dest++ = static_cast<अक्षर>(0xF0 | (cp >> 18));
      *dest++ = static_cast<अक्षर>(0x80 | ((cp >> 12) & 0x3F));
      *dest++ = static_cast<अक्षर>(0x80 | ((cp >> 6) & 0x3F));
      *dest++ = static_cast<अक्षर>(0x80 | (cp & 0x3F));
    } अन्यथा {
      परत ORT_MAKE_STATUS(ONNXRUNTIME, FAIL,
                             "Invalid Unicode codepoint during UTF-8 conversion");
    }
  }

  str.resize(static_cast<माप_प्रकार>(dest - str.data()));
  परत Status::OK();
}

/// Convert a UTF-8 string to a wide string, writing into a pre-allocated std::wstring.
/// The wstring is resized to the actual number of characters written.
/// Validates continuation bytes and rejects overlong encodings and surrogates.
ओळीत Status Utf8ToWide(स्थिरांक std::string& str, std::wstring& wstr) {
  जर (str.empty()) {
    wstr.clear();
    परत Status::OK();
  }

  जर (wstr.size() < str.size()) {
    wstr.resize(str.size());
  }

  स्थिरांक स्वतः* src = reinterpret_cast<स्थिरांक अचिन्हित अक्षर*>(str.data());
  स्थिरांक स्वतः* src_end = src + str.size();
  ब_अक्षर_प्रकार* dest = wstr.data();

  पर्यंत (src < src_end) {
    char32_t cp = 0;
    माप_प्रकार byte_len = 0;

    जर ((*src & 0x80) == 0) {
      cp = *src;
      byte_len = 1;
    } अन्यथा जर ((*src & 0xE0) == 0xC0) {
      byte_len = 2;
      जर (static_cast<माप_प्रकार>(src_end - src) < 2) {
        परत ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      जर ((src[1] & 0xC0) != 0x80) {
        परत ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x1F) << 6) |
           static_cast<char32_t>(src[1] & 0x3F);
      // Reject overlong encoding (must be >= 0x80 for 2-byte)
      जर (cp < 0x80) {
        परत ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
    } अन्यथा जर ((*src & 0xF0) == 0xE0) {
      byte_len = 3;
      जर (static_cast<माप_प्रकार>(src_end - src) < 3) {
        परत ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      जर ((src[1] & 0xC0) != 0x80 || (src[2] & 0xC0) != 0x80) {
        परत ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x0F) << 12) |
           (static_cast<char32_t>(src[1] & 0x3F) << 6) |
           static_cast<char32_t>(src[2] & 0x3F);
      // Reject overlong encoding (must be >= 0x800 for 3-byte)
      जर (cp < 0x800) {
        परत ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
      // Reject UTF-16 surrogates (U+D800..U+DFFF)
      जर (cp >= 0xD800 && cp <= 0xDFFF) {
        परत ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8: surrogate codepoint");
      }
    } अन्यथा जर ((*src & 0xF8) == 0xF0) {
      byte_len = 4;
      जर (static_cast<माप_प्रकार>(src_end - src) < 4) {
        परत ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Truncated UTF-8 sequence");
      }
      जर ((src[1] & 0xC0) != 0x80 || (src[2] & 0xC0) != 0x80 || (src[3] & 0xC0) != 0x80) {
        परत ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8 continuation byte");
      }
      cp = (static_cast<char32_t>(src[0] & 0x07) << 18) |
           (static_cast<char32_t>(src[1] & 0x3F) << 12) |
           (static_cast<char32_t>(src[2] & 0x3F) << 6) |
           static_cast<char32_t>(src[3] & 0x3F);
      // Reject overlong encoding (must be >= 0x10000 for 4-byte)
      जर (cp < 0x10000) {
        परत ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Overlong UTF-8 encoding");
      }
      // Reject codepoints beyond Unicode range
      जर (cp > 0x10FFFF) {
        परत ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8: codepoint beyond U+10FFFF");
      }
    } अन्यथा {
      परत ORT_MAKE_STATUS(ONNXRUNTIME, FAIL, "Invalid UTF-8 lead byte");
    }

    *dest++ = static_cast<ब_अक्षर_प्रकार>(cp);
    src += byte_len;
  }

  wstr.resize(static_cast<माप_प्रकार>(dest - wstr.data()));
  परत Status::OK();
}

/// Convenience: convert UTF-8 string to wstring (throws on error).
ओळीत std::wstring Utf8ToWideString(स्थिरांक std::string& s) {
  // UTF-8 byte count is an upper bound on wchar_t count
  std::wstring result;
  result.resize(s.size());
  ORT_THROW_IF_ERROR(Utf8ToWide(s, result));
  परत result;
}

#endif  // !_WIN32

}  // namespace utf8_util
}  // namespace onnxruntime
