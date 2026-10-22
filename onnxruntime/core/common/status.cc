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

#অন্তর্ভাব "core/common/status.h"
#অন্তর্ভাব "core/common/common.h"

নামাকাশ onnxruntime {
নামাকাশ common {
Status::Status(StatusCategory category, পূর্ণসংখ্যা code, ধ্রুবক std::string& বার্তা) {
  // state_ will be allocated here causing the status to be treated as a failure
  ORT_ENFORCE(code != static_cast<পূর্ণসংখ্যা>(common::OK));

  state_ = std::make_unique<State>(category, code, বার্তা);
}

Status::Status(StatusCategory category, পূর্ণসংখ্যা code, ধ্রুবক অক্ষর* বার্তা) {
  // state_ will be allocated here causing the status to be treated as a failure
  ORT_ENFORCE(code != static_cast<পূর্ণসংখ্যা>(common::OK));

  state_ = std::make_unique<State>(category, code, বার্তা);
}

Status::Status(StatusCategory category, পূর্ণসংখ্যা code)
    : Status(category, code, "") {
}

StatusCategory Status::Category() ধ্রুবক noexcept {
  ফেরত IsOK() ? common::NONE : state_->category;
}

পূর্ণসংখ্যা Status::Code() ধ্রুবক noexcept {
  ফেরত IsOK() ? static_cast<পূর্ণসংখ্যা>(common::OK) : state_->code;
}

ধ্রুবক std::string& Status::ErrorMessage() ধ্রুবক noexcept {
  ফেরত IsOK() ? EmptyString() : state_->বার্তা;
}

std::string Status::ToString() ধ্রুবক {
  যদি (state_ == nullptr) {
    ফেরত std::string("OK");
  }

  std::string result;

  যদি (common::SYSTEM == state_->category) {
    result += "SystemError";
    result += " : ";
    result += std::to_string(errno);
  } অন্যথা যদি (common::ONNXRUNTIME == state_->category) {
    result += "[ONNXRuntimeError]";
    result += " : ";
    result += std::to_string(Code());
    result += " : ";
    result += StatusCodeToString(static_cast<StatusCode>(Code()));
    result += " : ";
    result += state_->বার্তা;
  }

  ফেরত result;
}

// GSL_SUPPRESS(i.22) is broken. Ignore the warnings for the static local variables that are trivial
// and should not have any destruction order issues via pragmas instead.
// https://developercommunity.visualstudio.com/content/problem/249706/gslsuppress-does-not-work-for-i22-c-core-guideline.html
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 26426)
#endif

ধ্রুবক std::string& Status::EmptyString() noexcept {
  স্থির std::string s_empty;
  ফেরত s_empty;
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif

}  // namespace common
}  // namespace onnxruntime
