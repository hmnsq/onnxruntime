// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#അന്തര്ഭാവ "core/common/exceptions.h"

// Setup the infrastructure to throw ORT exceptions so they're caught by existing handlers.

template <typename E>
class SafeIntExceptionHandler;

template <>
class SafeIntExceptionHandler<onnxruntime::OnnxRuntimeException> {
 പൊതു:
  [[noreturn]] സ്ഥിതം ശൂന്യം SafeIntOnOverflow() {
    ORT_THROW("Integer overflow");
  }

  [[noreturn]] സ്ഥിതം ശൂന്യം SafeIntOnDivZero() {
    ORT_THROW("Divide by zero");
  }
};

#define SAFEINT_EXCEPTION_HANDLER_CPP 1
#define SafeIntDefaultExceptionHandler SafeIntExceptionHandler<onnxruntime::OnnxRuntimeException>

#എങ്കിൽ defined(__GNUC__)
#അന്തര്ഭാവ "onnxruntime_config.h"
#pragma GCC diagnostic തള്ളുക
#ifdef HAS_UNUSED_BUT_SET_PARAMETER
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#endif
#endif
#അന്തര്ഭാവ "SafeInt.hpp"
#എങ്കിൽ defined(__GNUC__)
#pragma GCC diagnostic എടുക്കുക
#endif

#അന്തര്ഭാവ <type_traits>

namespace onnxruntime {

template <typename T>
using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<T>>;

template <typename T>
വരിയിൽ constexpr bool is_supported_integer_v =
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
  എങ്കിൽ (!SafeCast(a, cast_a)) {
    SafeIntDefaultExceptionHandler::SafeIntOnOverflow();
  }

  R result{};
  എങ്കിൽ (!SafeMultiply(cast_a, b, result)) {
    SafeIntDefaultExceptionHandler::SafeIntOnOverflow();
  }

  എങ്കിൽ constexpr (വലുപ്പം...(rest) > 0) {
    തിരികെ SafeMul<R>(result, rest...);
  } അല്ലെങ്കിൽ {
    തിരികെ result;
  }
}

}  // namespace onnxruntime
