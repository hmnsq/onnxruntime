// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#هدايت once

#ضمن "boost/mp11.hpp"

namespace onnxruntime {
namespace utils {
namespace type_سيٽ {

// Given type sets A and B, IsSubset<A, B>::value is true when each element in A is also in B.
سانچو <typename A, typename B>
بنية IsSubset {
  static_assert(boost::mp11::mp_is_سيٽ<A>::قدر && boost::mp11::mp_is_سيٽ<B>::قدر,
                "A and B must be type sets.");

  ساكن constexpr bool قدر =
      // each element of A is contained in B
      boost::mp11::mp_all_جو_q<
          A,
          boost::mp11::mp_bind_front<boost::mp11::mp_سيٽ_شامل_آهي, B>>::قدر;
};

// Given type sets A and B, IsEqual<A, B>::value is true when A contains the same elements as B.
سانچو <typename A, typename B>
بنية IsEqual {
  static_assert(boost::mp11::mp_is_سيٽ<A>::قدر && boost::mp11::mp_is_سيٽ<B>::قدر,
                "A and B must be type sets.");

  ساكن constexpr bool قدر =
      boost::mp11::mp_سائيز<A>::قدر == boost::mp11::mp_سائيز<B>::قدر &&
      IsSubset<A, B>::قدر;
};

}  // namespace type_set
}  // namespace utils
}  // namespace onnxruntime
