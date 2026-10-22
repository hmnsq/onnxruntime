// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#هدايت once

#ضمن "core/common/exceptions.h"

// Setup the infrastructure to throw ORT exceptions so they're caught by existing handlers.

سانچو <typename E>
class محفوظ_Int_Exception_Handler;

سانچو <>
class محفوظ_Int_Exception_Handler<onnxruntime::OnnxRuntimeException> {
 عام:
  [[noreturn]] ساكن فراغ محفوظ_Int_On_حد_کان_مٿي() {
    ORT_THROW("Integer overflow");
  }

  [[noreturn]] ساكن فراغ محفوظ_Int_On_Div_Zero() {
    ORT_THROW("Divide by zero");
  }
};

#وصف_ڪريو SAFEINT_EXCEPTION_HANDLER_CPP 1
#وصف_ڪريو محفوظ_Int_Default_Exception_Handler محفوظ_Int_Exception_Handler<onnxruntime::OnnxRuntimeException>

#إذا defined(__GNUC__)
#ضمن "onnxruntime_config.h"
#هدايت GCC diagnostic ڌڪو
#جيڪڏهن_وصف_ٿيل HAS_اڻ_استعمال_ٿيل_BUT_سيٽ_PARAMETER
#هدايت GCC diagnostic ignored "-Wunused-but-set-parameter"
#جيڪڏهن_پورو
#جيڪڏهن_پورو
#ضمن "SafeInt.hpp"
#إذا defined(__GNUC__)
#هدايت GCC diagnostic ڪڍو
#جيڪڏهن_پورو

#ضمن <type_traits>

namespace onnxruntime {

سانچو <typename T>
using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<T>>;

سانچو <typename T>
مضمن constexpr bool is_supported_integer_v =
    std::is_integral_v<remove_cvref_t<T>> && !std::is_same_v<remove_cvref_t<T>, bool>;

//------------------------------------------------------------------------------
// Safe multiplication of two or more integer values into an explicit result type R.
// Throws OnnxRuntimeException on overflow.
//------------------------------------------------------------------------------
سانچو <typename R, typename T, typename U, typename... Rest>
[[nodiscard]] R محفوظ_Mul(T a, U b, Rest... rest) {
  static_assert(is_supported_integer_v<R>,
                "SafeMul requires an integral result type (excluding bool)");
  static_assert(is_supported_integer_v<T> && is_supported_integer_v<U>,
                "SafeMul requires integral operand types (excluding bool)");
  static_assert((is_supported_integer_v<Rest> && ...),
                "SafeMul requires integral operand types (excluding bool)");

  // SafeMultiply(T, U, T&) requires the first argument and result to share
  // the same type. Cast the first operand to R so the result is directly in R.
  R cast_a{};
  إذا (!محفوظ_Cast(a, cast_a)) {
    محفوظ_Int_Default_Exception_Handler::محفوظ_Int_On_حد_کان_مٿي();
  }

  R نتيجو{};
  إذا (!محفوظ_Multiply(cast_a, b, نتيجو)) {
    محفوظ_Int_Default_Exception_Handler::محفوظ_Int_On_حد_کان_مٿي();
  }

  إذا constexpr (حجم...(rest) > 0) {
    أرجع محفوظ_Mul<R>(نتيجو, rest...);
  } وإلا {
    أرجع نتيجو;
  }
}

}  // namespace onnxruntime
