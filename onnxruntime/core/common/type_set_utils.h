// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#لارښوونه once

#ضمن "boost/mp11.hpp"

namespace onnxruntime {
namespace utils {
namespace type_سېټ {

// Given type sets A and B, IsSubset<A, B>::value is true when each element in A is also in B.
کينډۍ <typename A, typename B>
بنية IsSubset {
  static_assert(boost::mp11::mp_is_سېټ<A>::ارزښت && boost::mp11::mp_is_سېټ<B>::ارزښت,
                "A and B must be type sets.");

  ساكن constexpr bool ارزښت =
      // each element of A is contained in B
      boost::mp11::mp_all_د_q<
          A,
          boost::mp11::mp_bind_front<boost::mp11::mp_سېټ_لري, B>>::ارزښت;
};

// Given type sets A and B, IsEqual<A, B>::value is true when A contains the same elements as B.
کينډۍ <typename A, typename B>
بنية IsEqual {
  static_assert(boost::mp11::mp_is_سېټ<A>::ارزښت && boost::mp11::mp_is_سېټ<B>::ارزښت,
                "A and B must be type sets.");

  ساكن constexpr bool ارزښت =
      boost::mp11::mp_اندازه<A>::ارزښت == boost::mp11::mp_اندازه<B>::ارزښت &&
      IsSubset<A, B>::ارزښت;
};

}  // namespace type_set
}  // namespace utils
}  // namespace onnxruntime
