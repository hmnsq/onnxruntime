// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#شامل "core/common/exceptions.h"

// Setup the infrastructure to throw ORT exceptions so they're caught by existing handlers.

template <typename E>
class SafeIntExceptionHandler;

template <>
class SafeIntExceptionHandler<onnxruntime::OnnxRuntimeException> {
 عمومی:
  [[noreturn]] ایستا تهی SafeIntOnOverflow() {
    ORT_THROW("Integer overflow");
  }

  [[noreturn]] ایستا تهی SafeIntOnDivZero() {
    ORT_THROW("Divide by zero");
  }
};

#define SAFEINT_EXCEPTION_HANDLER_CPP 1
#define SafeIntDefaultExceptionHandler SafeIntExceptionHandler<onnxruntime::OnnxRuntimeException>

#اگر defined(__GNUC__)
#شامل "onnxruntime_config.h"
#pragma GCC diagnostic افزودن
#ifdef HAS_UNUSED_BUT_SET_PARAMETER
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#endif
#endif
#شامل "SafeInt.hpp"
#اگر defined(__GNUC__)
#pragma GCC diagnostic برداشت
#endif

#شامل <type_traits>

namespace onnxruntime {

template <typename T>
using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<T>>;

template <typename T>
درونخطی constexpr bool is_supported_integer_v =
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
  اگر (!SafeCast(a, cast_a)) {
    SafeIntDefaultExceptionHandler::SafeIntOnOverflow();
  }

  R result{};
  اگر (!SafeMultiply(cast_a, b, result)) {
    SafeIntDefaultExceptionHandler::SafeIntOnOverflow();
  }

  اگر constexpr (اندازه...(rest) > 0) {
    بازگشت SafeMul<R>(result, rest...);
  } وگرنه {
    بازگشت result;
  }
}

}  // namespace onnxruntime
