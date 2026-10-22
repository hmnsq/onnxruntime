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

#شامل "core/common/status.h"
#شامل "core/common/common.h"

فضاینام onnxruntime {
فضاینام common {
Status::Status(StatusCategory category, صحیح code, ثابت std::string& پیام) {
  // state_ will be allocated here causing the status to be treated as a failure
  ORT_ENFORCE(code != static_cast<صحیح>(common::OK));

  state_ = std::make_unique<State>(category, code, پیام);
}

Status::Status(StatusCategory category, صحیح code, ثابت نویسه* پیام) {
  // state_ will be allocated here causing the status to be treated as a failure
  ORT_ENFORCE(code != static_cast<صحیح>(common::OK));

  state_ = std::make_unique<State>(category, code, پیام);
}

Status::Status(StatusCategory category, صحیح code)
    : Status(category, code, "") {
}

StatusCategory Status::Category() ثابت noexcept {
  بازگشت IsOK() ? common::NONE : state_->category;
}

صحیح Status::Code() ثابت noexcept {
  بازگشت IsOK() ? static_cast<صحیح>(common::OK) : state_->code;
}

ثابت std::string& Status::ErrorMessage() ثابت noexcept {
  بازگشت IsOK() ? EmptyString() : state_->پیام;
}

std::string Status::ToString() ثابت {
  اگر (state_ == nullptr) {
    بازگشت std::string("OK");
  }

  std::string result;

  اگر (common::SYSTEM == state_->category) {
    result += "SystemError";
    result += " : ";
    result += std::to_string(errno);
  } وگرنه اگر (common::ONNXRUNTIME == state_->category) {
    result += "[ONNXRuntimeError]";
    result += " : ";
    result += std::to_string(Code());
    result += " : ";
    result += StatusCodeToString(static_cast<StatusCode>(Code()));
    result += " : ";
    result += state_->پیام;
  }

  بازگشت result;
}

// GSL_SUPPRESS(i.22) is broken. Ignore the warnings for the static local variables that are trivial
// and should not have any destruction order issues via pragmas instead.
// https://developercommunity.visualstudio.com/content/problem/249706/gslsuppress-does-not-work-for-i22-c-core-guideline.html
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 26426)
#endif

ثابت std::string& Status::EmptyString() noexcept {
  ایستا std::string s_empty;
  بازگشت s_empty;
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif

}  // namespace common
}  // namespace onnxruntime
