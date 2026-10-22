// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#निर्देशः once

#अन्तर्भाव "core/common/exceptions.h"

// Setup the infrastructure to throw ORT exceptions so they're caught by existing handlers.

प्रतिरूपम् <typename E>
class SafeIntExceptionHandler;

प्रतिरूपम् <>
class SafeIntExceptionHandler<onnxruntime::OnnxRuntimeException> {
 सार्वजनिक:
  [[noreturn]] स्थिर शून्य Safe_Int_On_अतिप्रवाहः() {
    ORT_THROW("Integer overflow");
  }

  [[noreturn]] स्थिर शून्य SafeIntOnDivZero() {
    ORT_THROW("Divide by zero");
  }
};

#परिभाषय SAFEINT_EXCEPTION_HANDLER_CPP 1
#परिभाषय SafeIntDefaultExceptionHandler SafeIntExceptionHandler<onnxruntime::OnnxRuntimeException>

#यदि defined(__GNUC__)
#अन्तर्भाव "onnxruntime_config.h"
#निर्देशः GCC diagnostic प्रक्षिप
#यदि_परिभाषितम् HAS_अप्रयुक्तम्_BUT_समुच्चयः_PARAMETER
#निर्देशः GCC diagnostic ignored "-Wunused-but-set-parameter"
#यदि_अन्तः
#यदि_अन्तः
#अन्तर्भाव "SafeInt.hpp"
#यदि defined(__GNUC__)
#निर्देशः GCC diagnostic निष्कासय
#यदि_अन्तः

#अन्तर्भाव <type_traits>

namespace onnxruntime {

प्रतिरूपम् <typename T>
using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<T>>;

प्रतिरूपम् <typename T>
अन्तःस्थ constexpr bool is_supported_integer_v =
    std::is_integral_v<remove_cvref_t<T>> && !std::is_same_v<remove_cvref_t<T>, bool>;

//------------------------------------------------------------------------------
// Safe multiplication of two or more integer values into an explicit result type R.
// Throws OnnxRuntimeException on overflow.
//------------------------------------------------------------------------------
प्रतिरूपम् <typename R, typename T, typename U, typename... Rest>
[[nodiscard]] R SafeMul(T a, U b, Rest... rest) {
  static_assert(is_supported_integer_v<R>,
                "SafeMul requires an integral result type (excluding bool)");
  static_assert(is_supported_integer_v<T> && is_supported_integer_v<U>,
                "SafeMul requires integral operand types (excluding bool)");
  static_assert((is_supported_integer_v<Rest> && ...),
                "SafeMul requires integral operand types (excluding bool)");

  // SafeMultiply(T, U, T&) requires the first argument and result to share
  // the same type. Cast the first operand to R so the result is directly in R.
  R cast_a{};
  यदि (!SafeCast(a, cast_a)) {
    SafeIntDefaultExceptionHandler::Safe_Int_On_अतिप्रवाहः();
  }

  R परिणाम{};
  यदि (!SafeMultiply(cast_a, b, परिणाम)) {
    SafeIntDefaultExceptionHandler::Safe_Int_On_अतिप्रवाहः();
  }

  यदि constexpr (परिमाण...(rest) > 0) {
    प्रत्यागम SafeMul<R>(परिणाम, rest...);
  } अन्यथा {
    प्रत्यागम परिणाम;
  }
}

}  // namespace onnxruntime
