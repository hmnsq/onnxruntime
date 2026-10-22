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

نامخلا onnxruntime {
نامخلا common {
حالت_3::حالت_3(حالت_Category category, صحیح code, ثابت std::string& پیغام_2) {
  // state_ will be allocated here causing the status to be treated as a failure
  ORT_ENFORCE(code != static_cast<صحیح>(common::OK));

  state_ = std::make_unique<State>(category, code, پیغام_2);
}

حالت_3::حالت_3(حالت_Category category, صحیح code, ثابت حرف* پیغام_2) {
  // state_ will be allocated here causing the status to be treated as a failure
  ORT_ENFORCE(code != static_cast<صحیح>(common::OK));

  state_ = std::make_unique<State>(category, code, پیغام_2);
}

حالت_3::حالت_3(حالت_Category category, صحیح code)
    : حالت_3(category, code, "") {
}

حالت_Category حالت_3::Category() ثابت noexcept {
  واپس IsOK() ? common::NONE : state_->category;
}

صحیح حالت_3::رمز() ثابت noexcept {
  واپس IsOK() ? static_cast<صحیح>(common::OK) : state_->code;
}

ثابت std::string& حالت_3::Error_پیغام() ثابت noexcept {
  واپس IsOK() ? خالی_خانے_String() : state_->پیغام_2;
}

std::string حالت_3::مقصد_String() ثابت {
  اگر (state_ == خالی_اشاریہ) {
    واپس std::string("OK");
  }

  std::string result;

  اگر (common::SYSTEM == state_->category) {
    result += "SystemError";
    result += " : ";
    result += std::to_string(errno);
  } ورنہ اگر (common::ONNXRUNTIME == state_->category) {
    result += "[ONNXRuntimeError]";
    result += " : ";
    result += std::to_string(رمز());
    result += " : ";
    result += حالت_رمز_مقصد_String(static_cast<حالت_رمز>(رمز()));
    result += " : ";
    result += state_->پیغام_2;
  }

  واپس result;
}

// GSL_SUPPRESS(i.22) is broken. Ignore the warnings for the static local variables that are trivial
// and should not have any destruction order issues via pragmas instead.
// https://developercommunity.visualstudio.com/content/problem/249706/gslsuppress-does-not-work-for-i22-c-core-guideline.html
#اگر_تعریف_شدہ _MSC_VER
#ہدایت warning(push)
#ہدایت warning(disable : 26426)
#اگر_ختم

ثابت std::string& حالت_3::خالی_خانے_String() noexcept {
  ساکن std::string s_خالی_خانے;
  واپس s_خالی_خانے;
}

#اگر_تعریف_شدہ _MSC_VER
#ہدایت warning(pop)
#اگر_ختم

}  // namespace common
}  // namespace onnxruntime
