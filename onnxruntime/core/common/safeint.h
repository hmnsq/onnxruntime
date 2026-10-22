// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ਹਦਾਇਤ once

#ਅਨ੍ਤਰ੍ਭਾਵ "core/common/exceptions.h"

// Setup the infrastructure to throw ORT exceptions so they're caught by existing handlers.

ਸਾਂਚਾ <typename E>
class ਸੁਰੱਖਿਅਤ_Int_Exception_Handler;

ਸਾਂਚਾ <>
class ਸੁਰੱਖਿਅਤ_Int_Exception_Handler<onnxruntime::OnnxRuntimeException> {
 ਜਨਤਕ:
  [[noreturn]] ਸਥਿਰ ਖਾਲੀ ਸੁਰੱਖਿਅਤ_Int_On_ਉਛਾਲ() {
    ORT_THROW("Integer overflow");
  }

  [[noreturn]] ਸਥਿਰ ਖਾਲੀ ਸੁਰੱਖਿਅਤ_Int_On_Div_Zero() {
    ORT_THROW("Divide by zero");
  }
};

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SAFEINT_EXCEPTION_HANDLER_CPP 1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਸੁਰੱਖਿਅਤ_Int_Default_Exception_Handler ਸੁਰੱਖਿਅਤ_Int_Exception_Handler<onnxruntime::OnnxRuntimeException>

#ਜੇ defined(__GNUC__)
#ਅਨ੍ਤਰ੍ਭਾਵ "onnxruntime_config.h"
#ਹਦਾਇਤ GCC diagnostic ਧੱਕੋ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAS_ਅਣਵਰਤਿਆ_BUT_ਸੈੱਟ_PARAMETER
#ਹਦਾਇਤ GCC diagnostic ignored "-Wunused-but-set-parameter"
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ "SafeInt.hpp"
#ਜੇ defined(__GNUC__)
#ਹਦਾਇਤ GCC diagnostic ਕੱਢੋ
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ <type_traits>

namespace onnxruntime {

ਸਾਂਚਾ <typename T>
using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<T>>;

ਸਾਂਚਾ <typename T>
ਸਤਰਅੰਦਰ constexpr bool is_supported_integer_v =
    std::is_integral_v<remove_cvref_t<T>> && !std::is_same_v<remove_cvref_t<T>, bool>;

//------------------------------------------------------------------------------
// Safe multiplication of two or more integer values into an explicit result type R.
// Throws OnnxRuntimeException on overflow.
//------------------------------------------------------------------------------
ਸਾਂਚਾ <typename R, typename T, typename U, typename... Rest>
[[nodiscard]] R ਸੁਰੱਖਿਅਤ_Mul(T a, U b, Rest... rest) {
  static_assert(is_supported_integer_v<R>,
                "SafeMul requires an integral result type (excluding bool)");
  static_assert(is_supported_integer_v<T> && is_supported_integer_v<U>,
                "SafeMul requires integral operand types (excluding bool)");
  static_assert((is_supported_integer_v<Rest> && ...),
                "SafeMul requires integral operand types (excluding bool)");

  // SafeMultiply(T, U, T&) requires the first argument and result to share
  // the same type. Cast the first operand to R so the result is directly in R.
  R cast_a{};
  ਜੇ (!ਸੁਰੱਖਿਅਤ_Cast(a, cast_a)) {
    ਸੁਰੱਖਿਅਤ_Int_Default_Exception_Handler::ਸੁਰੱਖਿਅਤ_Int_On_ਉਛਾਲ();
  }

  R ਨਤੀਜਾ{};
  ਜੇ (!ਸੁਰੱਖਿਅਤ_Multiply(cast_a, b, ਨਤੀਜਾ)) {
    ਸੁਰੱਖਿਅਤ_Int_Default_Exception_Handler::ਸੁਰੱਖਿਅਤ_Int_On_ਉਛਾਲ();
  }

  ਜੇ constexpr (ਆਕਾਰ...(rest) > 0) {
    ਵਾਪਸ ਸੁਰੱਖਿਅਤ_Mul<R>(ਨਤੀਜਾ, rest...);
  } ਨਹੀਂਤਾਂ {
    ਵਾਪਸ ਨਤੀਜਾ;
  }
}

}  // namespace onnxruntime
