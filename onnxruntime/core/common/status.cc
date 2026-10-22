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

#ضمن "core/common/status.h"
#ضمن "core/common/common.h"

نطاق onnxruntime {
نطاق common {
حالت_2::حالت_2(حالت_Category category, صحيح code, ثابت std::string& نياپو) {
  // state_ will be allocated here causing the status to be treated as a failure
  ORT_ENFORCE(code != static_cast<صحيح>(common::OK));

  state_ = std::make_unique<State>(category, code, نياپو);
}

حالت_2::حالت_2(حالت_Category category, صحيح code, ثابت حرف* نياپو) {
  // state_ will be allocated here causing the status to be treated as a failure
  ORT_ENFORCE(code != static_cast<صحيح>(common::OK));

  state_ = std::make_unique<State>(category, code, نياپو);
}

حالت_2::حالت_2(حالت_Category category, صحيح code)
    : حالت_2(category, code, "") {
}

حالت_Category حالت_2::Category() ثابت noexcept {
  أرجع IsOK() ? common::NONE : state_->category;
}

صحيح حالت_2::ڪوڊ() ثابت noexcept {
  أرجع IsOK() ? static_cast<صحيح>(common::OK) : state_->code;
}

ثابت std::string& حالت_2::Error_پيغام() ثابت noexcept {
  أرجع IsOK() ? خالي_جايون_String() : state_->نياپو;
}

std::string حالت_2::منزل_String() ثابت {
  إذا (state_ == خالي_اشارو) {
    أرجع std::string("OK");
  }

  std::string result;

  إذا (common::SYSTEM == state_->category) {
    result += "SystemError";
    result += " : ";
    result += std::to_string(errno);
  } وإلا إذا (common::ONNXRUNTIME == state_->category) {
    result += "[ONNXRuntimeError]";
    result += " : ";
    result += std::to_string(ڪوڊ());
    result += " : ";
    result += حالت_ڪوڊ_منزل_String(static_cast<حالت_ڪوڊ>(ڪوڊ()));
    result += " : ";
    result += state_->نياپو;
  }

  أرجع result;
}

// GSL_SUPPRESS(i.22) is broken. Ignore the warnings for the static local variables that are trivial
// and should not have any destruction order issues via pragmas instead.
// https://developercommunity.visualstudio.com/content/problem/249706/gslsuppress-does-not-work-for-i22-c-core-guideline.html
#جيڪڏهن_وصف_ٿيل _MSC_VER
#هدايت warning(push)
#هدايت warning(disable : 26426)
#جيڪڏهن_پورو

ثابت std::string& حالت_2::خالي_جايون_String() noexcept {
  ساكن std::string s_خالي_جايون;
  أرجع s_خالي_جايون;
}

#جيڪڏهن_وصف_ٿيل _MSC_VER
#هدايت warning(pop)
#جيڪڏهن_پورو

}  // namespace common
}  // namespace onnxruntime
