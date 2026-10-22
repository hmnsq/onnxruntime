// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#ضمن "boost/mp11.hpp"

namespace onnxruntime {
namespace utils {
namespace type_set {

// Given type sets A and B, IsSubset<A, B>::value is true when each element in A is also in B.
template <typename A, typename B>
بنية IsSubset {
  static_assert(boost::mp11::mp_is_set<A>::قىممەت && boost::mp11::mp_is_set<B>::قىممەت,
                "A and B must be type sets.");

  ساكن constexpr bool قىممەت =
      // each element of A is contained in B
      boost::mp11::mp_all_of_q<
          A,
          boost::mp11::mp_bind_front<boost::mp11::mp_set_contains, B>>::قىممەت;
};

// Given type sets A and B, IsEqual<A, B>::value is true when A contains the same elements as B.
template <typename A, typename B>
بنية IsEqual {
  static_assert(boost::mp11::mp_is_set<A>::قىممەت && boost::mp11::mp_is_set<B>::قىممەت,
                "A and B must be type sets.");

  ساكن constexpr bool قىممەت =
      boost::mp11::mp_size<A>::قىممەت == boost::mp11::mp_size<B>::قىممەت &&
      IsSubset<A, B>::قىممەت;
};

}  // namespace type_set
}  // namespace utils
}  // namespace onnxruntime
