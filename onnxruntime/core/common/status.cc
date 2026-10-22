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

#ਅਨ੍ਤਰ੍ਭਾਵ "core/common/status.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "core/common/common.h"

ਨਾਮਾਕਾਸ਼ onnxruntime {
ਨਾਮਾਕਾਸ਼ common {
ਸਥਿਤੀ_2::ਸਥਿਤੀ_2(ਸਥਿਤੀ_Category category, ਪੂਰਨ code, ਅਚਲ std::string& ਸੁਨੇਹਾ_2) {
  // state_ will be allocated here causing the status to be treated as a failure
  ORT_ENFORCE(code != static_cast<ਪੂਰਨ>(common::OK));

  state_ = std::make_unique<State>(category, code, ਸੁਨੇਹਾ_2);
}

ਸਥਿਤੀ_2::ਸਥਿਤੀ_2(ਸਥਿਤੀ_Category category, ਪੂਰਨ code, ਅਚਲ ਅੱਖਰ* ਸੁਨੇਹਾ_2) {
  // state_ will be allocated here causing the status to be treated as a failure
  ORT_ENFORCE(code != static_cast<ਪੂਰਨ>(common::OK));

  state_ = std::make_unique<State>(category, code, ਸੁਨੇਹਾ_2);
}

ਸਥਿਤੀ_2::ਸਥਿਤੀ_2(ਸਥਿਤੀ_Category category, ਪੂਰਨ code)
    : ਸਥਿਤੀ_2(category, code, "") {
}

ਸਥਿਤੀ_Category ਸਥਿਤੀ_2::Category() ਅਚਲ noexcept {
  ਵਾਪਸ IsOK() ? common::NONE : state_->category;
}

ਪੂਰਨ ਸਥਿਤੀ_2::ਕੋਡ() ਅਚਲ noexcept {
  ਵਾਪਸ IsOK() ? static_cast<ਪੂਰਨ>(common::OK) : state_->code;
}

ਅਚਲ std::string& ਸਥਿਤੀ_2::Error_ਸੁਨੇਹਾ() ਅਚਲ noexcept {
  ਵਾਪਸ IsOK() ? ਖਾਲੀ_ਥਾਵਾਂ_String() : state_->ਸੁਨੇਹਾ_2;
}

std::string ਸਥਿਤੀ_2::ਮੰਜ਼ਿਲ_String() ਅਚਲ {
  ਜੇ (state_ == ਖਾਲੀ_ਸੂਚਕ) {
    ਵਾਪਸ std::string("OK");
  }

  std::string result;

  ਜੇ (common::SYSTEM == state_->category) {
    result += "SystemError";
    result += " : ";
    result += std::to_string(errno);
  } ਨਹੀਂਤਾਂ ਜੇ (common::ONNXRUNTIME == state_->category) {
    result += "[ONNXRuntimeError]";
    result += " : ";
    result += std::to_string(ਕੋਡ());
    result += " : ";
    result += ਸਥਿਤੀ_ਕੋਡ_ਮੰਜ਼ਿਲ_String(static_cast<ਸਥਿਤੀ_ਕੋਡ>(ਕੋਡ()));
    result += " : ";
    result += state_->ਸੁਨੇਹਾ_2;
  }

  ਵਾਪਸ result;
}

// GSL_SUPPRESS(i.22) is broken. Ignore the warnings for the static local variables that are trivial
// and should not have any destruction order issues via pragmas instead.
// https://developercommunity.visualstudio.com/content/problem/249706/gslsuppress-does-not-work-for-i22-c-core-guideline.html
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _MSC_VER
#ਹਦਾਇਤ warning(push)
#ਹਦਾਇਤ warning(disable : 26426)
#ਜੇ_ਅੰਤ

ਅਚਲ std::string& ਸਥਿਤੀ_2::ਖਾਲੀ_ਥਾਵਾਂ_String() noexcept {
  ਸਥਿਰ std::string s_ਖਾਲੀ_ਥਾਵਾਂ;
  ਵਾਪਸ s_ਖਾਲੀ_ਥਾਵਾਂ;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _MSC_VER
#ਹਦਾਇਤ warning(pop)
#ਜੇ_ਅੰਤ

}  // namespace common
}  // namespace onnxruntime
