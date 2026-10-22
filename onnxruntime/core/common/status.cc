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

#અન્તર્ભાવ "core/common/status.h"
#અન્તર્ભાવ "core/common/common.h"

નામાકાશ onnxruntime {
નામાકાશ common {
Status::Status(StatusCategory category, પૂર્ણાંક code, અચલ std::string& સંદેશ) {
  // state_ will be allocated here causing the status to be treated as a failure
  ORT_ENFORCE(code != static_cast<પૂર્ણાંક>(common::OK));

  state_ = std::make_unique<State>(category, code, સંદેશ);
}

Status::Status(StatusCategory category, પૂર્ણાંક code, અચલ અક્ષર* સંદેશ) {
  // state_ will be allocated here causing the status to be treated as a failure
  ORT_ENFORCE(code != static_cast<પૂર્ણાંક>(common::OK));

  state_ = std::make_unique<State>(category, code, સંદેશ);
}

Status::Status(StatusCategory category, પૂર્ણાંક code)
    : Status(category, code, "") {
}

StatusCategory Status::Category() અચલ noexcept {
  પાછા IsOK() ? common::NONE : state_->category;
}

પૂર્ણાંક Status::Code() અચલ noexcept {
  પાછા IsOK() ? static_cast<પૂર્ણાંક>(common::OK) : state_->code;
}

અચલ std::string& Status::ErrorMessage() અચલ noexcept {
  પાછા IsOK() ? EmptyString() : state_->સંદેશ;
}

std::string Status::ToString() અચલ {
  જો (state_ == nullptr) {
    પાછા std::string("OK");
  }

  std::string result;

  જો (common::SYSTEM == state_->category) {
    result += "SystemError";
    result += " : ";
    result += std::to_string(errno);
  } નહીંતર જો (common::ONNXRUNTIME == state_->category) {
    result += "[ONNXRuntimeError]";
    result += " : ";
    result += std::to_string(Code());
    result += " : ";
    result += StatusCodeToString(static_cast<StatusCode>(Code()));
    result += " : ";
    result += state_->સંદેશ;
  }

  પાછા result;
}

// GSL_SUPPRESS(i.22) is broken. Ignore the warnings for the static local variables that are trivial
// and should not have any destruction order issues via pragmas instead.
// https://developercommunity.visualstudio.com/content/problem/249706/gslsuppress-does-not-work-for-i22-c-core-guideline.html
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 26426)
#endif

અચલ std::string& Status::EmptyString() noexcept {
  સ્થિર std::string s_empty;
  પાછા s_empty;
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif

}  // namespace common
}  // namespace onnxruntime
