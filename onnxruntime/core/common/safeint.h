// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ಸೂಚನೆ once

#ಅನ್ತರ್ಭಾವ "core/common/exceptions.h"

// Setup the infrastructure to throw ORT exceptions so they're caught by existing handlers.

ಮಾದರಿ <typename E>
class ಸುರಕ್ಷಿತ_Int_Exception_Handler;

ಮಾದರಿ <>
class ಸುರಕ್ಷಿತ_Int_Exception_Handler<onnxruntime::OnnxRuntimeException> {
 ಸಾರ್ವಜನಿಕ:
  [[noreturn]] ಸ್ಥಿರ ಶೂನ್ಯ ಸುರಕ್ಷಿತ_Int_On_ಅತಿಪ್ರವಾಹ() {
    ORT_THROW("Integer overflow");
  }

  [[noreturn]] ಸ್ಥಿರ ಶೂನ್ಯ ಸುರಕ್ಷಿತ_Int_On_Div_Zero() {
    ORT_THROW("Divide by zero");
  }
};

#ವ್ಯಾಖ್ಯಾನಿಸು SAFEINT_EXCEPTION_HANDLER_CPP 1
#ವ್ಯಾಖ್ಯಾನಿಸು ಸುರಕ್ಷಿತ_Int_Default_Exception_Handler ಸುರಕ್ಷಿತ_Int_Exception_Handler<onnxruntime::OnnxRuntimeException>

#ಆದರೆ defined(__GNUC__)
#ಅನ್ತರ್ಭಾವ "onnxruntime_config.h"
#ಸೂಚನೆ GCC diagnostic ತಳ್ಳು
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAS_ಬಳಸದ_BUT_ಗಣ_PARAMETER
#ಸೂಚನೆ GCC diagnostic ignored "-Wunused-but-set-parameter"
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ "SafeInt.hpp"
#ಆದರೆ defined(__GNUC__)
#ಸೂಚನೆ GCC diagnostic ತೆಗೆ
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ <type_traits>

namespace onnxruntime {

ಮಾದರಿ <typename T>
using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<T>>;

ಮಾದರಿ <typename T>
ಸಾಲೊಳಗೆ constexpr bool is_supported_integer_v =
    std::is_integral_v<remove_cvref_t<T>> && !std::is_same_v<remove_cvref_t<T>, bool>;

//------------------------------------------------------------------------------
// Safe multiplication of two or more integer values into an explicit result type R.
// Throws OnnxRuntimeException on overflow.
//------------------------------------------------------------------------------
ಮಾದರಿ <typename R, typename T, typename U, typename... Rest>
[[nodiscard]] R ಸುರಕ್ಷಿತ_Mul(T a, U b, Rest... rest) {
  static_assert(is_supported_integer_v<R>,
                "SafeMul requires an integral result type (excluding bool)");
  static_assert(is_supported_integer_v<T> && is_supported_integer_v<U>,
                "SafeMul requires integral operand types (excluding bool)");
  static_assert((is_supported_integer_v<Rest> && ...),
                "SafeMul requires integral operand types (excluding bool)");

  // SafeMultiply(T, U, T&) requires the first argument and result to share
  // the same type. Cast the first operand to R so the result is directly in R.
  R cast_a{};
  ಆದರೆ (!ಸುರಕ್ಷಿತ_Cast(a, cast_a)) {
    ಸುರಕ್ಷಿತ_Int_Default_Exception_Handler::ಸುರಕ್ಷಿತ_Int_On_ಅತಿಪ್ರವಾಹ();
  }

  R ಫಲಿತಾಂಶ{};
  ಆದರೆ (!ಸುರಕ್ಷಿತ_Multiply(cast_a, b, ಫಲಿತಾಂಶ)) {
    ಸುರಕ್ಷಿತ_Int_Default_Exception_Handler::ಸುರಕ್ಷಿತ_Int_On_ಅತಿಪ್ರವಾಹ();
  }

  ಆದರೆ constexpr (ಗಾತ್ರ...(rest) > 0) {
    ಹಿಂತಿರುಗಿ ಸುರಕ್ಷಿತ_Mul<R>(ಫಲಿತಾಂಶ, rest...);
  } ಇಲ್ಲದಿದ್ದರೆ {
    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
  }
}

}  // namespace onnxruntime
