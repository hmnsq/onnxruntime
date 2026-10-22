// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#অন্তর্ভাব "boost/mp11.hpp"

namespace onnxruntime {
namespace utils {
namespace type_set {

// Given type sets A and B, IsSubset<A, B>::value is true when each element in A is also in B.
template <typename A, typename B>
সংরচনা IsSubset {
  static_assert(boost::mp11::mp_is_set<A>::মান && boost::mp11::mp_is_set<B>::মান,
                "A and B must be type sets.");

  স্থির constexpr bool মান =
      // each element of A is contained in B
      boost::mp11::mp_all_of_q<
          A,
          boost::mp11::mp_bind_front<boost::mp11::mp_set_contains, B>>::মান;
};

// Given type sets A and B, IsEqual<A, B>::value is true when A contains the same elements as B.
template <typename A, typename B>
সংরচনা IsEqual {
  static_assert(boost::mp11::mp_is_set<A>::মান && boost::mp11::mp_is_set<B>::মান,
                "A and B must be type sets.");

  স্থির constexpr bool মান =
      boost::mp11::mp_size<A>::মান == boost::mp11::mp_size<B>::মান &&
      IsSubset<A, B>::মান;
};

}  // namespace type_set
}  // namespace utils
}  // namespace onnxruntime
