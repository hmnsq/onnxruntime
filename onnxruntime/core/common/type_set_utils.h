// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#अन्तर्भाव "boost/mp11.hpp"

namespace onnxruntime {
namespace utils {
namespace type_set {

// Given type sets A and B, IsSubset<A, B>::value is true when each element in A is also in B.
template <typename A, typename B>
संरचना IsSubset {
  static_assert(boost::mp11::mp_is_set<A>::मानम् && boost::mp11::mp_is_set<B>::मानम्,
                "A and B must be type sets.");

  स्थिर constexpr bool मानम् =
      // each element of A is contained in B
      boost::mp11::mp_all_of_q<
          A,
          boost::mp11::mp_bind_front<boost::mp11::mp_set_contains, B>>::मानम्;
};

// Given type sets A and B, IsEqual<A, B>::value is true when A contains the same elements as B.
template <typename A, typename B>
संरचना IsEqual {
  static_assert(boost::mp11::mp_is_set<A>::मानम् && boost::mp11::mp_is_set<B>::मानम्,
                "A and B must be type sets.");

  स्थिर constexpr bool मानम् =
      boost::mp11::mp_size<A>::मानम् == boost::mp11::mp_size<B>::मानम् &&
      IsSubset<A, B>::मानम्;
};

}  // namespace type_set
}  // namespace utils
}  // namespace onnxruntime
