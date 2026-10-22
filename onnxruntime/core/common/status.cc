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

#অন্তর্ভাৱ "core/common/status.h"
#অন্তর্ভাৱ "core/common/common.h"

নামাকাশ onnxruntime {
নামাকাশ common {
Status::Status(StatusCategory category, পূর্ণাঙ্ক code, ধ্রুৱ std::string& বাৰ্তা) {
  // state_ will be allocated here causing the status to be treated as a failure
  ORT_ENFORCE(code != static_cast<পূর্ণাঙ্ক>(common::OK));

  state_ = std::make_unique<State>(category, code, বাৰ্তা);
}

Status::Status(StatusCategory category, পূর্ণাঙ্ক code, ধ্রুৱ ৱর্ণ* বাৰ্তা) {
  // state_ will be allocated here causing the status to be treated as a failure
  ORT_ENFORCE(code != static_cast<পূর্ণাঙ্ক>(common::OK));

  state_ = std::make_unique<State>(category, code, বাৰ্তা);
}

Status::Status(StatusCategory category, পূর্ণাঙ্ক code)
    : Status(category, code, "") {
}

StatusCategory Status::Category() ধ্রুৱ noexcept {
  ঘূৰাই IsOK() ? common::NONE : state_->category;
}

পূর্ণাঙ্ক Status::Code() ধ্রুৱ noexcept {
  ঘূৰাই IsOK() ? static_cast<পূর্ণাঙ্ক>(common::OK) : state_->code;
}

ধ্রুৱ std::string& Status::ErrorMessage() ধ্রুৱ noexcept {
  ঘূৰাই IsOK() ? EmptyString() : state_->বাৰ্তা;
}

std::string Status::ToString() ধ্রুৱ {
  যদি (state_ == nullptr) {
    ঘূৰাই std::string("OK");
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
    result += state_->বাৰ্তা;
  }

  ঘূৰাই result;
}

// GSL_SUPPRESS(i.22) is broken. Ignore the warnings for the static local variables that are trivial
// and should not have any destruction order issues via pragmas instead.
// https://developercommunity.visualstudio.com/content/problem/249706/gslsuppress-does-not-work-for-i22-c-core-guideline.html
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 26426)
#endif

ধ্রুৱ std::string& Status::EmptyString() noexcept {
  স্থির std::string s_empty;
  ঘূৰাই s_empty;
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif

}  // namespace common
}  // namespace onnxruntime
