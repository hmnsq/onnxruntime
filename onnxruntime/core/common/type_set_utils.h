// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#ಅನ್ತರ್ಭಾವ "boost/mp11.hpp"

namespace onnxruntime {
namespace utils {
namespace type_set {

// Given type sets A and B, IsSubset<A, B>::value is true when each element in A is also in B.
template <typename A, typename B>
ಸಂರಚನಾ IsSubset {
  static_assert(boost::mp11::mp_is_set<A>::ಮೌಲ್ಯ && boost::mp11::mp_is_set<B>::ಮೌಲ್ಯ,
                "A and B must be type sets.");

  ಸ್ಥಿರ constexpr bool ಮೌಲ್ಯ =
      // each element of A is contained in B
      boost::mp11::mp_all_of_q<
          A,
          boost::mp11::mp_bind_front<boost::mp11::mp_set_contains, B>>::ಮೌಲ್ಯ;
};

// Given type sets A and B, IsEqual<A, B>::value is true when A contains the same elements as B.
template <typename A, typename B>
ಸಂರಚನಾ IsEqual {
  static_assert(boost::mp11::mp_is_set<A>::ಮೌಲ್ಯ && boost::mp11::mp_is_set<B>::ಮೌಲ್ಯ,
                "A and B must be type sets.");

  ಸ್ಥಿರ constexpr bool ಮೌಲ್ಯ =
      boost::mp11::mp_size<A>::ಮೌಲ್ಯ == boost::mp11::mp_size<B>::ಮೌಲ್ಯ &&
      IsSubset<A, B>::ಮೌಲ್ಯ;
};

}  // namespace type_set
}  // namespace utils
}  // namespace onnxruntime
