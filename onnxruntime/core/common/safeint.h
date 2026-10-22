// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#لارښوونه once

#ضمن "core/common/exceptions.h"

// Setup the infrastructure to throw ORT exceptions so they're caught by existing handlers.

کينډۍ <typename E>
class خوندي_Int_Exception_Handler;

کينډۍ <>
class خوندي_Int_Exception_Handler<onnxruntime::OnnxRuntimeException> {
 عام:
  [[noreturn]] ساكن فراغ خوندي_Int_On_ډکېدنه() {
    ORT_THROW("Integer overflow");
  }

  [[noreturn]] ساكن فراغ خوندي_Int_On_Div_Zero() {
    ORT_THROW("Divide by zero");
  }
};

#تعریف_کړه SAFEINT_EXCEPTION_HANDLER_CPP 1
#تعریف_کړه خوندي_Int_Default_Exception_Handler خوندي_Int_Exception_Handler<onnxruntime::OnnxRuntimeException>

#إذا defined(__GNUC__)
#ضمن "onnxruntime_config.h"
#لارښوونه GCC diagnostic ټېل
#که_تعریف_شوی HAS_ناکارول_شوی_BUT_سېټ_PARAMETER
#لارښوونه GCC diagnostic ignored "-Wunused-but-set-parameter"
#د_که_پای
#د_که_پای
#ضمن "SafeInt.hpp"
#إذا defined(__GNUC__)
#لارښوونه GCC diagnostic راوباسه
#د_که_پای

#ضمن <type_traits>

namespace onnxruntime {

کينډۍ <typename T>
using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<T>>;

کينډۍ <typename T>
مضمن constexpr bool is_supported_integer_v =
    std::is_integral_v<remove_cvref_t<T>> && !std::is_same_v<remove_cvref_t<T>, bool>;

//------------------------------------------------------------------------------
// Safe multiplication of two or more integer values into an explicit result type R.
// Throws OnnxRuntimeException on overflow.
//------------------------------------------------------------------------------
کينډۍ <typename R, typename T, typename U, typename... Rest>
[[nodiscard]] R خوندي_Mul(T a, U b, Rest... rest) {
  static_assert(is_supported_integer_v<R>,
                "SafeMul requires an integral result type (excluding bool)");
  static_assert(is_supported_integer_v<T> && is_supported_integer_v<U>,
                "SafeMul requires integral operand types (excluding bool)");
  static_assert((is_supported_integer_v<Rest> && ...),
                "SafeMul requires integral operand types (excluding bool)");

  // SafeMultiply(T, U, T&) requires the first argument and result to share
  // the same type. Cast the first operand to R so the result is directly in R.
  R cast_a{};
  إذا (!خوندي_Cast(a, cast_a)) {
    خوندي_Int_Default_Exception_Handler::خوندي_Int_On_ډکېدنه();
  }

  R پایله{};
  إذا (!خوندي_Multiply(cast_a, b, پایله)) {
    خوندي_Int_Default_Exception_Handler::خوندي_Int_On_ډکېدنه();
  }

  إذا constexpr (حجم...(rest) > 0) {
    أرجع خوندي_Mul<R>(پایله, rest...);
  } وإلا {
    أرجع پایله;
  }
}

}  // namespace onnxruntime
