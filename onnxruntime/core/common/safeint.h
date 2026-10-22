// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#సూచన once

#అన్తర్భావ "core/common/exceptions.h"

// Setup the infrastructure to throw ORT exceptions so they're caught by existing handlers.

మూస <typename E>
class సురక్షిత_Int_Exception_Handler;

మూస <>
class సురక్షిత_Int_Exception_Handler<onnxruntime::OnnxRuntimeException> {
 బహిరంగ:
  [[noreturn]] స్థిర శూన్య సురక్షిత_Int_On_అధికప్రవాహం() {
    ORT_THROW("Integer overflow");
  }

  [[noreturn]] స్థిర శూన్య సురక్షిత_Int_On_Div_Zero() {
    ORT_THROW("Divide by zero");
  }
};

#నిర్వచించు SAFEINT_EXCEPTION_HANDLER_CPP 1
#నిర్వచించు సురక్షిత_Int_Default_Exception_Handler సురక్షిత_Int_Exception_Handler<onnxruntime::OnnxRuntimeException>

#ఒకవేళ defined(__GNUC__)
#అన్తర్భావ "onnxruntime_config.h"
#సూచన GCC diagnostic నెట్టు
#నిర్వచితమైతే HAS_వాడని_BUT_సమితి_PARAMETER
#సూచన GCC diagnostic ignored "-Wunused-but-set-parameter"
#అయితే_ముగింపు
#అయితే_ముగింపు
#అన్తర్భావ "SafeInt.hpp"
#ఒకవేళ defined(__GNUC__)
#సూచన GCC diagnostic తీయి
#అయితే_ముగింపు

#అన్తర్భావ <type_traits>

namespace onnxruntime {

మూస <typename T>
using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<T>>;

మూస <typename T>
అంతఃవరుస constexpr bool is_supported_integer_v =
    std::is_integral_v<remove_cvref_t<T>> && !std::is_same_v<remove_cvref_t<T>, bool>;

//------------------------------------------------------------------------------
// Safe multiplication of two or more integer values into an explicit result type R.
// Throws OnnxRuntimeException on overflow.
//------------------------------------------------------------------------------
మూస <typename R, typename T, typename U, typename... Rest>
[[nodiscard]] R సురక్షిత_Mul(T a, U b, Rest... rest) {
  static_assert(is_supported_integer_v<R>,
                "SafeMul requires an integral result type (excluding bool)");
  static_assert(is_supported_integer_v<T> && is_supported_integer_v<U>,
                "SafeMul requires integral operand types (excluding bool)");
  static_assert((is_supported_integer_v<Rest> && ...),
                "SafeMul requires integral operand types (excluding bool)");

  // SafeMultiply(T, U, T&) requires the first argument and result to share
  // the same type. Cast the first operand to R so the result is directly in R.
  R cast_a{};
  ఒకవేళ (!సురక్షిత_Cast(a, cast_a)) {
    సురక్షిత_Int_Default_Exception_Handler::సురక్షిత_Int_On_అధికప్రవాహం();
  }

  R ఫలితం{};
  ఒకవేళ (!సురక్షిత_Multiply(cast_a, b, ఫలితం)) {
    సురక్షిత_Int_Default_Exception_Handler::సురక్షిత_Int_On_అధికప్రవాహం();
  }

  ఒకవేళ constexpr (పరిమాణం...(rest) > 0) {
    తిరిగి సురక్షిత_Mul<R>(ఫలితం, rest...);
  } లేకపోతే {
    తిరిగి ఫలితం;
  }
}

}  // namespace onnxruntime
