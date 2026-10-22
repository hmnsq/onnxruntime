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

#समावेश "core/common/status.h"
#समावेश "core/common/common.h"

namespace onnxruntime {
namespace common {
Status::Status(StatusCategory category, पूर्णांक code, स्थिर std::string& संदेश) {
  // state_ will be allocated here causing the status to be treated as a failure
  ORT_ENFORCE(code != static_cast<पूर्णांक>(common::OK));

  state_ = std::make_unique<State>(category, code, संदेश);
}

Status::Status(StatusCategory category, पूर्णांक code, स्थिर अक्षर* संदेश) {
  // state_ will be allocated here causing the status to be treated as a failure
  ORT_ENFORCE(code != static_cast<पूर्णांक>(common::OK));

  state_ = std::make_unique<State>(category, code, संदेश);
}

Status::Status(StatusCategory category, पूर्णांक code)
    : Status(category, code, "") {
}

StatusCategory Status::Category() स्थिर noexcept {
  वापस IsOK() ? common::NONE : state_->category;
}

पूर्णांक Status::Code() स्थिर noexcept {
  वापस IsOK() ? static_cast<पूर्णांक>(common::OK) : state_->code;
}

स्थिर std::string& Status::ErrorMessage() स्थिर noexcept {
  वापस IsOK() ? EmptyString() : state_->संदेश;
}

std::string Status::ToString() स्थिर {
  अगर (state_ == nullptr) {
    वापस std::string("OK");
  }

  std::string result;

  अगर (common::SYSTEM == state_->category) {
    result += "SystemError";
    result += " : ";
    result += std::to_string(त्रुटि_सं);
  } अन्यथा अगर (common::ONNXRUNTIME == state_->category) {
    result += "[ONNXRuntimeError]";
    result += " : ";
    result += std::to_string(Code());
    result += " : ";
    result += StatusCodeToString(static_cast<StatusCode>(Code()));
    result += " : ";
    result += state_->संदेश;
  }

  वापस result;
}

// GSL_SUPPRESS(i.22) is broken. Ignore the warnings for the static local variables that are trivial
// and should not have any destruction order issues via pragmas instead.
// https://developercommunity.visualstudio.com/content/problem/249706/gslsuppress-does-not-work-for-i22-c-core-guideline.html
#अगर_घोषित _MSC_VER
#आशय warning(push)
#आशय warning(disable : 26426)
#पूर्ण_अगर

स्थिर std::string& Status::EmptyString() noexcept {
  अटल std::string s_empty;
  वापस s_empty;
}

#अगर_घोषित _MSC_VER
#आशय warning(pop)
#पूर्ण_अगर

}  // namespace common
}  // namespace onnxruntime
