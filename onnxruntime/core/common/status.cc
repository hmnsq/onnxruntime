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

#അന്തര്ഭാവ "core/common/status.h"
#അന്തര്ഭാവ "core/common/common.h"

നാമാകാശ onnxruntime {
നാമാകാശ common {
Status::Status(StatusCategory category, പൂർണ്ണസംഖ്യ code, സ്ഥിരം std::string& സന്ദേശം) {
  // state_ will be allocated here causing the status to be treated as a failure
  ORT_ENFORCE(code != static_cast<പൂർണ്ണസംഖ്യ>(common::OK));

  state_ = std::make_unique<State>(category, code, സന്ദേശം);
}

Status::Status(StatusCategory category, പൂർണ്ണസംഖ്യ code, സ്ഥിരം അക്ഷരം* സന്ദേശം) {
  // state_ will be allocated here causing the status to be treated as a failure
  ORT_ENFORCE(code != static_cast<പൂർണ്ണസംഖ്യ>(common::OK));

  state_ = std::make_unique<State>(category, code, സന്ദേശം);
}

Status::Status(StatusCategory category, പൂർണ്ണസംഖ്യ code)
    : Status(category, code, "") {
}

StatusCategory Status::Category() സ്ഥിരം noexcept {
  തിരികെ IsOK() ? common::NONE : state_->category;
}

പൂർണ്ണസംഖ്യ Status::Code() സ്ഥിരം noexcept {
  തിരികെ IsOK() ? static_cast<പൂർണ്ണസംഖ്യ>(common::OK) : state_->code;
}

സ്ഥിരം std::string& Status::ErrorMessage() സ്ഥിരം noexcept {
  തിരികെ IsOK() ? EmptyString() : state_->സന്ദേശം;
}

std::string Status::ToString() സ്ഥിരം {
  എങ്കിൽ (state_ == nullptr) {
    തിരികെ std::string("OK");
  }

  std::string result;

  എങ്കിൽ (common::SYSTEM == state_->category) {
    result += "SystemError";
    result += " : ";
    result += std::to_string(errno);
  } അല്ലെങ്കിൽ എങ്കിൽ (common::ONNXRUNTIME == state_->category) {
    result += "[ONNXRuntimeError]";
    result += " : ";
    result += std::to_string(Code());
    result += " : ";
    result += StatusCodeToString(static_cast<StatusCode>(Code()));
    result += " : ";
    result += state_->സന്ദേശം;
  }

  തിരികെ result;
}

// GSL_SUPPRESS(i.22) is broken. Ignore the warnings for the static local variables that are trivial
// and should not have any destruction order issues via pragmas instead.
// https://developercommunity.visualstudio.com/content/problem/249706/gslsuppress-does-not-work-for-i22-c-core-guideline.html
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 26426)
#endif

സ്ഥിരം std::string& Status::EmptyString() noexcept {
  സ്ഥിതം std::string s_empty;
  തിരികെ s_empty;
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif

}  // namespace common
}  // namespace onnxruntime
