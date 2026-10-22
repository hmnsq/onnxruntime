// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#अन्तर्भाव "boost/mp11.hpp"

namespace onnxruntime {
namespace utils {
namespace type_set {

// Given type sets A and B, IsSubset<A, B>::value is true when each element in A is also in B.
ढाँचा <typename A, typename B>
संरचना IsSubset {
  static_assert(boost::mp11::mp_is_set<A>::मान && boost::mp11::mp_is_set<B>::मान,
                "A and B must be type sets.");

  स्थिर constexpr bool मान =
      // each element of A is contained in B
      boost::mp11::mp_all_of_q<
          A,
          boost::mp11::mp_bind_front<boost::mp11::mp_set_contains, B>>::मान;
};

// Given type sets A and B, IsEqual<A, B>::value is true when A contains the same elements as B.
ढाँचा <typename A, typename B>
संरचना IsEqual {
  static_assert(boost::mp11::mp_is_set<A>::मान && boost::mp11::mp_is_set<B>::मान,
                "A and B must be type sets.");

  स्थिर constexpr bool मान =
      boost::mp11::mp_size<A>::मान == boost::mp11::mp_size<B>::मान &&
      IsSubset<A, B>::मान;
};

}  // namespace type_set
}  // namespace utils
}  // namespace onnxruntime
