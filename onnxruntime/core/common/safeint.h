// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#অন্তর্ভাৱ "core/common/exceptions.h"

// Setup the infrastructure to throw ORT exceptions so they're caught by existing handlers.

template <typename E>
class SafeIntExceptionHandler;

template <>
class SafeIntExceptionHandler<onnxruntime::OnnxRuntimeException> {
 সার্ৱজনিক:
  [[noreturn]] স্থির শূন্য SafeIntOnOverflow() {
    ORT_THROW("Integer overflow");
  }

  [[noreturn]] স্থির শূন্য SafeIntOnDivZero() {
    ORT_THROW("Divide by zero");
  }
};

#define SAFEINT_EXCEPTION_HANDLER_CPP 1
#define SafeIntDefaultExceptionHandler SafeIntExceptionHandler<onnxruntime::OnnxRuntimeException>

#যদি defined(__GNUC__)
#অন্তর্ভাৱ "onnxruntime_config.h"
#pragma GCC diagnostic ঠেলো
#ifdef HAS_UNUSED_BUT_SET_PARAMETER
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#endif
#endif
#অন্তর্ভাৱ "SafeInt.hpp"
#যদি defined(__GNUC__)
#pragma GCC diagnostic বেৰ_কৰো
#endif

#অন্তর্ভাৱ <type_traits>

namespace onnxruntime {

template <typename T>
using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<T>>;

template <typename T>
অন্তঃস্থ constexpr bool is_supported_integer_v =
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
  যদি (!SafeCast(a, cast_a)) {
    SafeIntDefaultExceptionHandler::SafeIntOnOverflow();
  }

  R result{};
  যদি (!SafeMultiply(cast_a, b, result)) {
    SafeIntDefaultExceptionHandler::SafeIntOnOverflow();
  }

  যদি constexpr (পরিমাণ...(rest) > 0) {
    ঘূৰাই SafeMul<R>(result, rest...);
  } অন্যথা {
    ঘূৰাই result;
  }
}

}  // namespace onnxruntime
