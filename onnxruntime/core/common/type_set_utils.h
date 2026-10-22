// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#निर्देशः once

#अन्तर्भाव "boost/mp11.hpp"

namespace onnxruntime {
namespace utils {
namespace type_समुच्चयः {

// Given type sets A and B, IsSubset<A, B>::value is true when each element in A is also in B.
प्रतिरूपम् <typename A, typename B>
संरचना IsSubset {
  static_assert(boost::mp11::mp_is_समुच्चयः<A>::मानम् && boost::mp11::mp_is_समुच्चयः<B>::मानम्,
                "A and B must be type sets.");

  स्थिर constexpr bool मानम् =
      // each element of A is contained in B
      boost::mp11::mp_all_सम्बन्धि_q<
          A,
          boost::mp11::mp_bind_front<boost::mp11::mp_समुच्चयः_अन्तर्भवति, B>>::मानम्;
};

// Given type sets A and B, IsEqual<A, B>::value is true when A contains the same elements as B.
प्रतिरूपम् <typename A, typename B>
संरचना IsEqual {
  static_assert(boost::mp11::mp_is_समुच्चयः<A>::मानम् && boost::mp11::mp_is_समुच्चयः<B>::मानम्,
                "A and B must be type sets.");

  स्थिर constexpr bool मानम् =
      boost::mp11::mp_आकार<A>::मानम् == boost::mp11::mp_आकार<B>::मानम् &&
      IsSubset<A, B>::मानम्;
};

}  // namespace type_set
}  // namespace utils
}  // namespace onnxruntime
