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

#అన్తర్భావ "core/common/status.h"
#అన్తర్భావ "core/common/common.h"

నామాకాశ onnxruntime {
నామాకాశ common {
స్థితి_3::స్థితి_3(స్థితి_Category category, పూర్ణాంకం code, స్థిరరాశి std::string& సందేశం_2) {
  // state_ will be allocated here causing the status to be treated as a failure
  ORT_ENFORCE(code != static_cast<పూర్ణాంకం>(common::OK));

  state_ = std::make_unique<State>(category, code, సందేశం_2);
}

స్థితి_3::స్థితి_3(స్థితి_Category category, పూర్ణాంకం code, స్థిరరాశి అక్షరం* సందేశం_2) {
  // state_ will be allocated here causing the status to be treated as a failure
  ORT_ENFORCE(code != static_cast<పూర్ణాంకం>(common::OK));

  state_ = std::make_unique<State>(category, code, సందేశం_2);
}

స్థితి_3::స్థితి_3(స్థితి_Category category, పూర్ణాంకం code)
    : స్థితి_3(category, code, "") {
}

స్థితి_Category స్థితి_3::Category() స్థిరరాశి noexcept {
  తిరిగి IsOK() ? common::NONE : state_->category;
}

పూర్ణాంకం స్థితి_3::సంకేతం_2() స్థిరరాశి noexcept {
  తిరిగి IsOK() ? static_cast<పూర్ణాంకం>(common::OK) : state_->code;
}

స్థిరరాశి std::string& స్థితి_3::Error_సందేశం() స్థిరరాశి noexcept {
  తిరిగి IsOK() ? ఖాళీలు_String() : state_->సందేశం_2;
}

std::string స్థితి_3::గమ్యం_String() స్థిరరాశి {
  ఒకవేళ (state_ == శూన్యసూచిక) {
    తిరిగి std::string("OK");
  }

  std::string result;

  ఒకవేళ (common::SYSTEM == state_->category) {
    result += "SystemError";
    result += " : ";
    result += std::to_string(errno);
  } లేకపోతే ఒకవేళ (common::ONNXRUNTIME == state_->category) {
    result += "[ONNXRuntimeError]";
    result += " : ";
    result += std::to_string(సంకేతం_2());
    result += " : ";
    result += స్థితి_సంకేతం_గమ్యం_String(static_cast<స్థితి_సంకేతం>(సంకేతం_2()));
    result += " : ";
    result += state_->సందేశం_2;
  }

  తిరిగి result;
}

// GSL_SUPPRESS(i.22) is broken. Ignore the warnings for the static local variables that are trivial
// and should not have any destruction order issues via pragmas instead.
// https://developercommunity.visualstudio.com/content/problem/249706/gslsuppress-does-not-work-for-i22-c-core-guideline.html
#నిర్వచితమైతే _MSC_VER
#సూచన warning(push)
#సూచన warning(disable : 26426)
#అయితే_ముగింపు

స్థిరరాశి std::string& స్థితి_3::ఖాళీలు_String() noexcept {
  స్థిర std::string s_ఖాళీలు;
  తిరిగి s_ఖాళీలు;
}

#నిర్వచితమైతే _MSC_VER
#సూచన warning(pop)
#అయితే_ముగింపు

}  // namespace common
}  // namespace onnxruntime
