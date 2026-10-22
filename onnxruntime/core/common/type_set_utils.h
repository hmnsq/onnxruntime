// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#شامل "boost/mp11.hpp"

namespace onnxruntime {
namespace utils {
namespace type_set {

// Given type sets A and B, IsSubset<A, B>::value is true when each element in A is also in B.
template <typename A, typename B>
ساختار IsSubset {
  static_assert(boost::mp11::mp_is_set<A>::مقدار && boost::mp11::mp_is_set<B>::مقدار,
                "A and B must be type sets.");

  ایستا constexpr bool مقدار =
      // each element of A is contained in B
      boost::mp11::mp_all_of_q<
          A,
          boost::mp11::mp_bind_front<boost::mp11::mp_set_contains, B>>::مقدار;
};

// Given type sets A and B, IsEqual<A, B>::value is true when A contains the same elements as B.
template <typename A, typename B>
ساختار IsEqual {
  static_assert(boost::mp11::mp_is_set<A>::مقدار && boost::mp11::mp_is_set<B>::مقدار,
                "A and B must be type sets.");

  ایستا constexpr bool مقدار =
      boost::mp11::mp_size<A>::مقدار == boost::mp11::mp_size<B>::مقدار &&
      IsSubset<A, B>::مقدار;
};

}  // namespace type_set
}  // namespace utils
}  // namespace onnxruntime
