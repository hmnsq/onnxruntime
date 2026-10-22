// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ہدایت once

#ضمن "boost/mp11.hpp"

namespace onnxruntime {
namespace utils {
namespace type_سیٹ {

// Given type sets A and B, IsSubset<A, B>::value is true when each element in A is also in B.
سانچا <typename A, typename B>
بنية IsSubset {
  static_assert(boost::mp11::mp_is_سیٹ<A>::قدر && boost::mp11::mp_is_سیٹ<B>::قدر,
                "A and B must be type sets.");

  ساكن constexpr bool قدر =
      // each element of A is contained in B
      boost::mp11::mp_all_دا_q<
          A,
          boost::mp11::mp_bind_front<boost::mp11::mp_سیٹ_شامل_اے, B>>::قدر;
};

// Given type sets A and B, IsEqual<A, B>::value is true when A contains the same elements as B.
سانچا <typename A, typename B>
بنية IsEqual {
  static_assert(boost::mp11::mp_is_سیٹ<A>::قدر && boost::mp11::mp_is_سیٹ<B>::قدر,
                "A and B must be type sets.");

  ساكن constexpr bool قدر =
      boost::mp11::mp_ناپ<A>::قدر == boost::mp11::mp_ناپ<B>::قدر &&
      IsSubset<A, B>::قدر;
};

}  // namespace type_set
}  // namespace utils
}  // namespace onnxruntime
