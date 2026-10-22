// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#رهنمود once

#ضمن "boost/mp11.hpp"

namespace onnxruntime {
namespace utils {
namespace type_مجموعه {

// Given type sets A and B, IsSubset<A, B>::value is true when each element in A is also in B.
قالب <typename A, typename B>
بنية IsSubset {
  static_assert(boost::mp11::mp_is_مجموعه<A>::مقدار && boost::mp11::mp_is_مجموعه<B>::مقدار,
                "A and B must be type sets.");

  ساكن constexpr bool مقدار =
      // each element of A is contained in B
      boost::mp11::mp_all_از_q<
          A,
          boost::mp11::mp_bind_front<boost::mp11::mp_مجموعه_شامل_است, B>>::مقدار;
};

// Given type sets A and B, IsEqual<A, B>::value is true when A contains the same elements as B.
قالب <typename A, typename B>
بنية IsEqual {
  static_assert(boost::mp11::mp_is_مجموعه<A>::مقدار && boost::mp11::mp_is_مجموعه<B>::مقدار,
                "A and B must be type sets.");

  ساكن constexpr bool مقدار =
      boost::mp11::mp_اندازه<A>::مقدار == boost::mp11::mp_اندازه<B>::مقدار &&
      IsSubset<A, B>::مقدار;
};

}  // namespace type_set
}  // namespace utils
}  // namespace onnxruntime
