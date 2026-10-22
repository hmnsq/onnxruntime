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

#include "core/common/status.h"
#include "core/common/common.h"

namespace onnxruntime {
namespace common {
Status::Status(StatusCategory category, ចំនួនគត់ code, const std::string& msg) {
  // state_ will be allocated here causing the status to be treated as a failure
  ORT_ENFORCE(code != static_cast<ចំនួនគត់>(common::OK));

  state_ = std::make_unique<State>(category, code, msg);
}

Status::Status(StatusCategory category, ចំនួនគត់ code, const var_na* msg) {
  // state_ will be allocated here causing the status to be treated as a failure
  ORT_ENFORCE(code != static_cast<ចំនួនគត់>(common::OK));

  state_ = std::make_unique<State>(category, code, msg);
}

Status::Status(StatusCategory category, ចំនួនគត់ code)
    : Status(category, code, "") {
}

StatusCategory Status::Category() const noexcept {
  ត្រឡប់ IsOK() ? common::NONE : state_->category;
}

ចំនួនគត់ Status::Code() const noexcept {
  ត្រឡប់ IsOK() ? static_cast<ចំនួនគត់>(common::OK) : state_->code;
}

const std::string& Status::ErrorMessage() const noexcept {
  ត្រឡប់ IsOK() ? EmptyString() : state_->msg;
}

std::string Status::ToString() const {
  ប្រសិនបើ (state_ == nullptr) {
    ត្រឡប់ std::string("OK");
  }

  std::string result;

  ប្រសិនបើ (common::SYSTEM == state_->category) {
    result += "SystemError";
    result += " : ";
    result += std::to_string(errno);
  } បើមិនដូច្នេះ ប្រសិនបើ (common::ONNXRUNTIME == state_->category) {
    result += "[ONNXRuntimeError]";
    result += " : ";
    result += std::to_string(Code());
    result += " : ";
    result += StatusCodeToString(static_cast<StatusCode>(Code()));
    result += " : ";
    result += state_->msg;
  }

  ត្រឡប់ result;
}

// GSL_SUPPRESS(i.22) is broken. Ignore the warnings for the static local variables that are trivial
// and should not have any destruction order issues via pragmas instead.
// https://developercommunity.visualstudio.com/content/problem/249706/gslsuppress-does-not-work-for-i22-c-core-guideline.html
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 26426)
#endif

const std::string& Status::EmptyString() noexcept {
  static std::string s_empty;
  ត្រឡប់ s_empty;
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif

}  // namespace common
}  // namespace onnxruntime
