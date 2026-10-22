// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#ಅನ್ತರ್ಭಾವ "core/common/exceptions.h"

// Setup the infrastructure to throw ORT exceptions so they're caught by existing handlers.

template <typename E>
class SafeIntExceptionHandler;

template <>
class SafeIntExceptionHandler<onnxruntime::OnnxRuntimeException> {
 ಸಾರ್ವಜನಿಕ:
  [[noreturn]] ಸ್ಥಿರ ಶೂನ್ಯ SafeIntOnOverflow() {
    ORT_THROW("Integer overflow");
  }

  [[noreturn]] ಸ್ಥಿರ ಶೂನ್ಯ SafeIntOnDivZero() {
    ORT_THROW("Divide by zero");
  }
};

#define SAFEINT_EXCEPTION_HANDLER_CPP 1
#define SafeIntDefaultExceptionHandler SafeIntExceptionHandler<onnxruntime::OnnxRuntimeException>

#ಯದಿ defined(__GNUC__)
#ಅನ್ತರ್ಭಾವ "onnxruntime_config.h"
#pragma GCC diagnostic ತಳ್ಳು
#ifdef HAS_UNUSED_BUT_SET_PARAMETER
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#endif
#endif
#ಅನ್ತರ್ಭಾವ "SafeInt.hpp"
#ಯದಿ defined(__GNUC__)
#pragma GCC diagnostic ತೆಗೆ
#endif

#ಅನ್ತರ್ಭಾವ <type_traits>

namespace onnxruntime {

template <typename T>
using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<T>>;

template <typename T>
ಅನ್ತಃಸ್ಥ constexpr bool is_supported_integer_v =
    std::is_integral_v<remove_cvref_t<T>> && !std::is_same_v<remove_cvref_t<T>, bool>;

//------------------------------------------------------------------------------
// Safe multiplication of two or more integer values into an explicit result type R.
// Throws OnnxRuntimeException on overflow.
//------------------------------------------------------------------------------
template <typename R, typename T, typename U, typename... Rest>
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
  ಯದಿ (!SafeCast(a, cast_a)) {
    SafeIntDefaultExceptionHandler::SafeIntOnOverflow();
  }

  R result{};
  ಯದಿ (!SafeMultiply(cast_a, b, result)) {
    SafeIntDefaultExceptionHandler::SafeIntOnOverflow();
  }

  ಯದಿ constexpr (ಪರಿಮಾಣ...(rest) > 0) {
    ಪ್ರತ್ಯಾಗಮ SafeMul<R>(result, rest...);
  } ಅನ್ಯಥಾ {
    ಪ್ರತ್ಯಾಗಮ result;
  }
}

}  // namespace onnxruntime
