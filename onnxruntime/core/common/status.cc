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
Status::Status(StatusCategory category, पूर्णाङ्क code, अचर std::string& सन्देश) {
  // state_ will be allocated here causing the status to be treated as a failure
  ORT_ENFORCE(code != static_cast<पूर्णाङ्क>(common::OK));

  state_ = std::make_unique<State>(category, code, सन्देश);
}

Status::Status(StatusCategory category, पूर्णाङ्क code, अचर वर्ण* सन्देश) {
  // state_ will be allocated here causing the status to be treated as a failure
  ORT_ENFORCE(code != static_cast<पूर्णाङ्क>(common::OK));

  state_ = std::make_unique<State>(category, code, सन्देश);
}

Status::Status(StatusCategory category, पूर्णाङ्क code)
    : Status(category, code, "") {
}

StatusCategory Status::Category() अचर noexcept {
  फर्काऊ IsOK() ? common::NONE : state_->category;
}

पूर्णाङ्क Status::Code() अचर noexcept {
  फर्काऊ IsOK() ? static_cast<पूर्णाङ्क>(common::OK) : state_->code;
}

अचर std::string& Status::ErrorMessage() अचर noexcept {
  फर्काऊ IsOK() ? EmptyString() : state_->सन्देश;
}

std::string Status::ToString() अचर {
  यदि (state_ == nullptr) {
    फर्काऊ std::string("OK");
  }

  std::string result;

  यदि (common::SYSTEM == state_->category) {
    result += "SystemError";
    result += " : ";
    result += std::to_string(त्रुटि_सं);
  } अन्यथा यदि (common::ONNXRUNTIME == state_->category) {
    result += "[ONNXRuntimeError]";
    result += " : ";
    result += std::to_string(Code());
    result += " : ";
    result += StatusCodeToString(static_cast<StatusCode>(Code()));
    result += " : ";
    result += state_->सन्देश;
  }

  फर्काऊ result;
}

// GSL_SUPPRESS(i.22) is broken. Ignore the warnings for the static local variables that are trivial
// and should not have any destruction order issues via pragmas instead.
// https://developercommunity.visualstudio.com/content/problem/249706/gslsuppress-does-not-work-for-i22-c-core-guideline.html
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 26426)
#endif

अचर std::string& Status::EmptyString() noexcept {
  स्थिर std::string s_empty;
  फर्काऊ s_empty;
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif

}  // namespace common
}  // namespace onnxruntime
