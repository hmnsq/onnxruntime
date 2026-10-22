// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#आशय once

#समावेश "core/common/exceptions.h"

// Setup the infrastructure to throw ORT exceptions so they're caught by existing handlers.

ढाँचा <typename E>
class SafeIntExceptionHandler;

ढाँचा <>
class SafeIntExceptionHandler<onnxruntime::OnnxRuntimeException> {
 खुला:
  [[noreturn]] अटल व्योम SafeIntOnOverflow() {
    ORT_THROW("Integer overflow");
  }

  [[noreturn]] अटल व्योम SafeIntOnDivZero() {
    ORT_THROW("Divide by zero");
  }
};

#घोषणा SAFEINT_EXCEPTION_HANDLER_CPP 1
#घोषणा SafeIntDefaultExceptionHandler SafeIntExceptionHandler<onnxruntime::OnnxRuntimeException>

#अगर defined(__GNUC__)
#समावेश "onnxruntime_config.h"
#आशय GCC diagnostic धकेलो
#अगर_घोषित HAS_UNUSED_BUT_SET_PARAMETER
#आशय GCC diagnostic ignored "-Wunused-but-set-parameter"
#पूर्ण_अगर
#पूर्ण_अगर
#समावेश "SafeInt.hpp"
#अगर defined(__GNUC__)
#आशय GCC diagnostic निकालो
#पूर्ण_अगर

#समावेश <type_traits>

namespace onnxruntime {

ढाँचा <typename T>
using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<T>>;

ढाँचा <typename T>
अंतरभूत constexpr bool is_supported_integer_v =
    std::is_integral_v<remove_cvref_t<T>> && !std::is_same_v<remove_cvref_t<T>, bool>;

//------------------------------------------------------------------------------
// Safe multiplication of two or more integer values into an explicit result type R.
// Throws OnnxRuntimeException on overflow.
//------------------------------------------------------------------------------
ढाँचा <typename R, typename T, typename U, typename... Rest>
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
  अगर (!SafeCast(a, cast_a)) {
    SafeIntDefaultExceptionHandler::SafeIntOnOverflow();
  }

  R परिणाम{};
  अगर (!SafeMultiply(cast_a, b, परिणाम)) {
    SafeIntDefaultExceptionHandler::SafeIntOnOverflow();
  }

  अगर constexpr (माप...(rest) > 0) {
    वापस SafeMul<R>(परिणाम, rest...);
  } अन्यथा {
    वापस परिणाम;
  }
}

}  // namespace onnxruntime
