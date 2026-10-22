// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ہدایت once

#ضمن "core/common/exceptions.h"

// Setup the infrastructure to throw ORT exceptions so they're caught by existing handlers.

سانچا <typename E>
class محفوظ_Int_Exception_Handler;

سانچا <>
class محفوظ_Int_Exception_Handler<onnxruntime::OnnxRuntimeException> {
 عام:
  [[noreturn]] ساكن فراغ محفوظ_Int_On_حد_توں_ودھ() {
    ORT_THROW("Integer overflow");
  }

  [[noreturn]] ساكن فراغ محفوظ_Int_On_Div_Zero() {
    ORT_THROW("Divide by zero");
  }
};

#تعریف_کرو SAFEINT_EXCEPTION_HANDLER_CPP 1
#تعریف_کرو محفوظ_Int_Default_Exception_Handler محفوظ_Int_Exception_Handler<onnxruntime::OnnxRuntimeException>

#إذا defined(__GNUC__)
#ضمن "onnxruntime_config.h"
#ہدایت GCC diagnostic ڈالو
#جے_تعریف_شدہ HAS_بن_ورتیا_BUT_سیٹ_PARAMETER
#ہدایت GCC diagnostic ignored "-Wunused-but-set-parameter"
#جے_انت
#جے_انت
#ضمن "SafeInt.hpp"
#إذا defined(__GNUC__)
#ہدایت GCC diagnostic نکالو
#جے_انت

#ضمن <type_traits>

namespace onnxruntime {

سانچا <typename T>
using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<T>>;

سانچا <typename T>
مضمن constexpr bool is_supported_integer_v =
    std::is_integral_v<remove_cvref_t<T>> && !std::is_same_v<remove_cvref_t<T>, bool>;

//------------------------------------------------------------------------------
// Safe multiplication of two or more integer values into an explicit result type R.
// Throws OnnxRuntimeException on overflow.
//------------------------------------------------------------------------------
سانچا <typename R, typename T, typename U, typename... Rest>
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
    محفوظ_Int_Default_Exception_Handler::محفوظ_Int_On_حد_توں_ودھ();
  }

  R نتیجہ{};
  إذا (!محفوظ_Multiply(cast_a, b, نتیجہ)) {
    محفوظ_Int_Default_Exception_Handler::محفوظ_Int_On_حد_توں_ودھ();
  }

  إذا constexpr (حجم...(rest) > 0) {
    أرجع محفوظ_Mul<R>(نتیجہ, rest...);
  } وإلا {
    أرجع نتیجہ;
  }
}

}  // namespace onnxruntime
