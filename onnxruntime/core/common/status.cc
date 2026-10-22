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
स्थितिः_2::स्थितिः_2(स्थितिः_Category category, पूर्णाङ्क code, ध्रुव std::string& सन्देशः_2) {
  // state_ will be allocated here causing the status to be treated as a failure
  ORT_ENFORCE(code != static_cast<पूर्णाङ्क>(common::OK));

  state_ = std::make_unique<State>(category, code, सन्देशः_2);
}

स्थितिः_2::स्थितिः_2(स्थितिः_Category category, पूर्णाङ्क code, ध्रुव वर्ण* सन्देशः_2) {
  // state_ will be allocated here causing the status to be treated as a failure
  ORT_ENFORCE(code != static_cast<पूर्णाङ्क>(common::OK));

  state_ = std::make_unique<State>(category, code, सन्देशः_2);
}

स्थितिः_2::स्थितिः_2(स्थितिः_Category category, पूर्णाङ्क code)
    : स्थितिः_2(category, code, "") {
}

स्थितिः_Category स्थितिः_2::Category() ध्रुव noexcept {
  प्रत्यागम IsOK() ? common::NONE : state_->category;
}

पूर्णाङ्क स्थितिः_2::संहिता() ध्रुव noexcept {
  प्रत्यागम IsOK() ? static_cast<पूर्णाङ्क>(common::OK) : state_->code;
}

ध्रुव std::string& स्थितिः_2::Error_सन्देशः() ध्रुव noexcept {
  प्रत्यागम IsOK() ? रिक्तानि_String() : state_->सन्देशः_2;
}

std::string स्थितिः_2::गन्तव्यम्_String() ध्रुव {
  यदि (state_ == शून्यसूचकः) {
    प्रत्यागम std::string("OK");
  }

  std::string result;

  यदि (common::SYSTEM == state_->category) {
    result += "SystemError";
    result += " : ";
    result += std::to_string(errno);
  } अन्यथा यदि (common::ONNXRUNTIME == state_->category) {
    result += "[ONNXRuntimeError]";
    result += " : ";
    result += std::to_string(संहिता());
    result += " : ";
    result += स्थितिः_संहिता_गन्तव्यम्_String(static_cast<स्थितिः_संहिता>(संहिता()));
    result += " : ";
    result += state_->सन्देशः_2;
  }

  प्रत्यागम result;
}

// GSL_SUPPRESS(i.22) is broken. Ignore the warnings for the static local variables that are trivial
// and should not have any destruction order issues via pragmas instead.
// https://developercommunity.visualstudio.com/content/problem/249706/gslsuppress-does-not-work-for-i22-c-core-guideline.html
#यदि_परिभाषितम् _MSC_VER
#निर्देशः warning(push)
#निर्देशः warning(disable : 26426)
#यदि_अन्तः

ध्रुव std::string& स्थितिः_2::रिक्तानि_String() noexcept {
  स्थिर std::string s_रिक्तानि;
  प्रत्यागम s_रिक्तानि;
}

#यदि_परिभाषितम् _MSC_VER
#निर्देशः warning(pop)
#यदि_अन्तः

}  // namespace common
}  // namespace onnxruntime
