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

#அந்தர்பாவ "core/common/status.h"
#அந்தர்பாவ "core/common/common.h"

நாமாகாஶ onnxruntime {
நாமாகாஶ common {
Status::Status(StatusCategory category, முழு code, மாறிலி std::string& செய்தி) {
  // state_ will be allocated here causing the status to be treated as a failure
  ORT_ENFORCE(code != static_cast<முழு>(common::OK));

  state_ = std::make_unique<State>(category, code, செய்தி);
}

Status::Status(StatusCategory category, முழு code, மாறிலி எழுத்து* செய்தி) {
  // state_ will be allocated here causing the status to be treated as a failure
  ORT_ENFORCE(code != static_cast<முழு>(common::OK));

  state_ = std::make_unique<State>(category, code, செய்தி);
}

Status::Status(StatusCategory category, முழு code)
    : Status(category, code, "") {
}

StatusCategory Status::Category() மாறிலி noexcept {
  திரும்பு IsOK() ? common::NONE : state_->category;
}

முழு Status::Code() மாறிலி noexcept {
  திரும்பு IsOK() ? static_cast<முழு>(common::OK) : state_->code;
}

மாறிலி std::string& Status::ErrorMessage() மாறிலி noexcept {
  திரும்பு IsOK() ? EmptyString() : state_->செய்தி;
}

std::string Status::ToString() மாறிலி {
  எனில் (state_ == nullptr) {
    திரும்பு std::string("OK");
  }

  std::string result;

  எனில் (common::SYSTEM == state_->category) {
    result += "SystemError";
    result += " : ";
    result += std::to_string(errno);
  } இல்லையெனில் எனில் (common::ONNXRUNTIME == state_->category) {
    result += "[ONNXRuntimeError]";
    result += " : ";
    result += std::to_string(Code());
    result += " : ";
    result += StatusCodeToString(static_cast<StatusCode>(Code()));
    result += " : ";
    result += state_->செய்தி;
  }

  திரும்பு result;
}

// GSL_SUPPRESS(i.22) is broken. Ignore the warnings for the static local variables that are trivial
// and should not have any destruction order issues via pragmas instead.
// https://developercommunity.visualstudio.com/content/problem/249706/gslsuppress-does-not-work-for-i22-c-core-guideline.html
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 26426)
#endif

மாறிலி std::string& Status::EmptyString() noexcept {
  நிலையான std::string s_empty;
  திரும்பு s_empty;
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif

}  // namespace common
}  // namespace onnxruntime
