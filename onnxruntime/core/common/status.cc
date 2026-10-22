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

#अन्तर्भाव "core/common/status.h"
#अन्तर्भाव "core/common/common.h"

नामाकाश onnxruntime {
नामाकाश common {
Status::Status(StatusCategory category, पूर्णांक code, स्थिरांक std::string& संदेश) {
  // state_ will be allocated here causing the status to be treated as a failure
  ORT_ENFORCE(code != static_cast<पूर्णांक>(common::OK));

  state_ = std::make_unique<State>(category, code, संदेश);
}

Status::Status(StatusCategory category, पूर्णांक code, स्थिरांक अक्षर* संदेश) {
  // state_ will be allocated here causing the status to be treated as a failure
  ORT_ENFORCE(code != static_cast<पूर्णांक>(common::OK));

  state_ = std::make_unique<State>(category, code, संदेश);
}

Status::Status(StatusCategory category, पूर्णांक code)
    : Status(category, code, "") {
}

StatusCategory Status::Category() स्थिरांक noexcept {
  परत IsOK() ? common::NONE : state_->category;
}

पूर्णांक Status::Code() स्थिरांक noexcept {
  परत IsOK() ? static_cast<पूर्णांक>(common::OK) : state_->code;
}

स्थिरांक std::string& Status::ErrorMessage() स्थिरांक noexcept {
  परत IsOK() ? EmptyString() : state_->संदेश;
}

std::string Status::ToString() स्थिरांक {
  जर (state_ == nullptr) {
    परत std::string("OK");
  }

  std::string result;

  जर (common::SYSTEM == state_->category) {
    result += "SystemError";
    result += " : ";
    result += std::to_string(त्रुटि_सं);
  } अन्यथा जर (common::ONNXRUNTIME == state_->category) {
    result += "[ONNXRuntimeError]";
    result += " : ";
    result += std::to_string(Code());
    result += " : ";
    result += StatusCodeToString(static_cast<StatusCode>(Code()));
    result += " : ";
    result += state_->संदेश;
  }

  परत result;
}

// GSL_SUPPRESS(i.22) is broken. Ignore the warnings for the static local variables that are trivial
// and should not have any destruction order issues via pragmas instead.
// https://developercommunity.visualstudio.com/content/problem/249706/gslsuppress-does-not-work-for-i22-c-core-guideline.html
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 26426)
#endif

स्थिरांक std::string& Status::EmptyString() noexcept {
  स्थिर std::string s_empty;
  परत s_empty;
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif

}  // namespace common
}  // namespace onnxruntime
