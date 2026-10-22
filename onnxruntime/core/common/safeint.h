// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#رهنمود once

#ضمن "core/common/exceptions.h"

// Setup the infrastructure to throw ORT exceptions so they're caught by existing handlers.

قالب <typename E>
class امن_Int_Exception_Handler;

قالب <>
class امن_Int_Exception_Handler<onnxruntime::OnnxRuntimeException> {
 عام:
  [[noreturn]] ساكن فراغ امن_Int_On_سرریز() {
    ORT_THROW("Integer overflow");
  }

  [[noreturn]] ساكن فراغ امن_Int_On_Div_Zero() {
    ORT_THROW("Divide by zero");
  }
};

#تعریف_کن SAFEINT_EXCEPTION_HANDLER_CPP 1
#تعریف_کن امن_Int_Default_Exception_Handler امن_Int_Exception_Handler<onnxruntime::OnnxRuntimeException>

#إذا defined(__GNUC__)
#ضمن "onnxruntime_config.h"
#رهنمود GCC diagnostic افزودن
#اگر_تعریف_شده HAS_استفادهنشده_BUT_مجموعه_PARAMETER
#رهنمود GCC diagnostic ignored "-Wunused-but-set-parameter"
#پایان_اگر
#پایان_اگر
#ضمن "SafeInt.hpp"
#إذا defined(__GNUC__)
#رهنمود GCC diagnostic برداشت
#پایان_اگر

#ضمن <type_traits>

namespace onnxruntime {

قالب <typename T>
using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<T>>;

قالب <typename T>
مضمن constexpr bool is_supported_integer_v =
    std::is_integral_v<remove_cvref_t<T>> && !std::is_same_v<remove_cvref_t<T>, bool>;

//------------------------------------------------------------------------------
// Safe multiplication of two or more integer values into an explicit result type R.
// Throws OnnxRuntimeException on overflow.
//------------------------------------------------------------------------------
قالب <typename R, typename T, typename U, typename... Rest>
[[nodiscard]] R امن_Mul(T a, U b, Rest... rest) {
  static_assert(is_supported_integer_v<R>,
                "SafeMul requires an integral result type (excluding bool)");
  static_assert(is_supported_integer_v<T> && is_supported_integer_v<U>,
                "SafeMul requires integral operand types (excluding bool)");
  static_assert((is_supported_integer_v<Rest> && ...),
                "SafeMul requires integral operand types (excluding bool)");

  // SafeMultiply(T, U, T&) requires the first argument and result to share
  // the same type. Cast the first operand to R so the result is directly in R.
  R cast_a{};
  إذا (!امن_Cast(a, cast_a)) {
    امن_Int_Default_Exception_Handler::امن_Int_On_سرریز();
  }

  R نتیجه{};
  إذا (!امن_Multiply(cast_a, b, نتیجه)) {
    امن_Int_Default_Exception_Handler::امن_Int_On_سرریز();
  }

  إذا constexpr (حجم...(rest) > 0) {
    أرجع امن_Mul<R>(نتیجه, rest...);
  } وإلا {
    أرجع نتیجه;
  }
}

}  // namespace onnxruntime
