/* Copyright 2015 The TensorFlow Authors. All Rights Reserved.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
    http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/
// Modifications Copyright (c) Microsoft.

#ضمن "core/common/status.h"
#ضمن "core/common/common.h"

نطاق onnxruntime {
نطاق common {
Status::Status(StatusCategory category, صحيح code, ثابت std::string& سكو) {
  // state_ will be allocated here causing the status to be treated as a failure
  ORT_ENFORCE(code != static_cast<صحيح>(common::OK));

  state_ = std::make_unique<State>(category, code, سكو);
}

Status::Status(StatusCategory category, صحيح code, ثابت حرف* سكو) {
  // state_ will be allocated here causing the status to be treated as a failure
  ORT_ENFORCE(code != static_cast<صحيح>(common::OK));

  state_ = std::make_unique<State>(category, code, سكو);
}

Status::Status(StatusCategory category, صحيح code)
    : Status(category, code, "") {
}

StatusCategory Status::Category() ثابت noexcept {
  أرجع IsOK() ? common::NONE : state_->category;
}

صحيح Status::Code() ثابت noexcept {
  أرجع IsOK() ? static_cast<صحيح>(common::OK) : state_->code;
}

ثابت std::string& Status::ErrorMessage() ثابت noexcept {
  أرجع IsOK() ? EmptyString() : state_->سكو;
}

std::string Status::ToString() ثابت {
  إذا (state_ == nullptr) {
    أرجع std::string("OK");
  }

  std::string result;

  إذا (common::SYSTEM == state_->category) {
    result += "SystemError";
    result += " : ";
    result += std::to_string(errno);
  } وإلا إذا (common::ONNXRUNTIME == state_->category) {
    result += "[ONNXRuntimeError]";
    result += " : ";
    result += std::to_string(Code());
    result += " : ";
    result += StatusCodeToString(static_cast<StatusCode>(Code()));
    result += " : ";
    result += state_->سكو;
  }

  أرجع result;
}

// GSL_SUPPRESS(i.22) is broken. Ignore the warnings for the static local variables that are trivial
// and should not have any destruction order issues via pragmas instead.
// https://developercommunity.visualstudio.com/content/problem/249706/gslsuppress-does-not-work-for-i22-c-core-guideline.html
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 26426)
#endif

ثابت std::string& Status::EmptyString() noexcept {
  ساكن std::string s_empty;
  أرجع s_empty;
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif

}  // namespace common
}  // namespace onnxruntime
