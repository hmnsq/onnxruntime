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

#ಅನ್ತರ್ಭಾವ "core/common/status.h"
#ಅನ್ತರ್ಭಾವ "core/common/common.h"

ನಾಮಾಕಾಶ onnxruntime {
ನಾಮಾಕಾಶ common {
Status::Status(StatusCategory category, ಪೂರ್ಣಾಙ್ಕ code, ಧ್ರುವ std::string& ಸಂದೇಶ) {
  // state_ will be allocated here causing the status to be treated as a failure
  ORT_ENFORCE(code != static_cast<ಪೂರ್ಣಾಙ್ಕ>(common::OK));

  state_ = std::make_unique<State>(category, code, ಸಂದೇಶ);
}

Status::Status(StatusCategory category, ಪೂರ್ಣಾಙ್ಕ code, ಧ್ರುವ ವರ್ಣ* ಸಂದೇಶ) {
  // state_ will be allocated here causing the status to be treated as a failure
  ORT_ENFORCE(code != static_cast<ಪೂರ್ಣಾಙ್ಕ>(common::OK));

  state_ = std::make_unique<State>(category, code, ಸಂದೇಶ);
}

Status::Status(StatusCategory category, ಪೂರ್ಣಾಙ್ಕ code)
    : Status(category, code, "") {
}

StatusCategory Status::Category() ಧ್ರುವ noexcept {
  ಪ್ರತ್ಯಾಗಮ IsOK() ? common::NONE : state_->category;
}

ಪೂರ್ಣಾಙ್ಕ Status::Code() ಧ್ರುವ noexcept {
  ಪ್ರತ್ಯಾಗಮ IsOK() ? static_cast<ಪೂರ್ಣಾಙ್ಕ>(common::OK) : state_->code;
}

ಧ್ರುವ std::string& Status::ErrorMessage() ಧ್ರುವ noexcept {
  ಪ್ರತ್ಯಾಗಮ IsOK() ? EmptyString() : state_->ಸಂದೇಶ;
}

std::string Status::ToString() ಧ್ರುವ {
  ಯದಿ (state_ == nullptr) {
    ಪ್ರತ್ಯಾಗಮ std::string("OK");
  }

  std::string result;

  ಯದಿ (common::SYSTEM == state_->category) {
    result += "SystemError";
    result += " : ";
    result += std::to_string(errno);
  } ಅನ್ಯಥಾ ಯದಿ (common::ONNXRUNTIME == state_->category) {
    result += "[ONNXRuntimeError]";
    result += " : ";
    result += std::to_string(Code());
    result += " : ";
    result += StatusCodeToString(static_cast<StatusCode>(Code()));
    result += " : ";
    result += state_->ಸಂದೇಶ;
  }

  ಪ್ರತ್ಯಾಗಮ result;
}

// GSL_SUPPRESS(i.22) is broken. Ignore the warnings for the static local variables that are trivial
// and should not have any destruction order issues via pragmas instead.
// https://developercommunity.visualstudio.com/content/problem/249706/gslsuppress-does-not-work-for-i22-c-core-guideline.html
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 26426)
#endif

ಧ್ರುವ std::string& Status::EmptyString() noexcept {
  ಸ್ಥಿರ std::string s_empty;
  ಪ್ರತ್ಯಾಗಮ s_empty;
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif

}  // namespace common
}  // namespace onnxruntime
