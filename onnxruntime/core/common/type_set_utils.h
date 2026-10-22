// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ਹਦਾਇਤ once

#ਅਨ੍ਤਰ੍ਭਾਵ "boost/mp11.hpp"

namespace onnxruntime {
namespace utils {
namespace type_ਸੈੱਟ {

// Given type sets A and B, IsSubset<A, B>::value is true when each element in A is also in B.
ਸਾਂਚਾ <typename A, typename B>
ਬਣਤਰ IsSubset {
  static_assert(boost::mp11::mp_is_ਸੈੱਟ<A>::ਮੁੱਲ && boost::mp11::mp_is_ਸੈੱਟ<B>::ਮੁੱਲ,
                "A and B must be type sets.");

  ਸਥਿਰ constexpr bool ਮੁੱਲ =
      // each element of A is contained in B
      boost::mp11::mp_all_ਦਾ_q<
          A,
          boost::mp11::mp_bind_front<boost::mp11::mp_ਸੈੱਟ_ਸ਼ਾਮਲ_ਹੈ, B>>::ਮੁੱਲ;
};

// Given type sets A and B, IsEqual<A, B>::value is true when A contains the same elements as B.
ਸਾਂਚਾ <typename A, typename B>
ਬਣਤਰ IsEqual {
  static_assert(boost::mp11::mp_is_ਸੈੱਟ<A>::ਮੁੱਲ && boost::mp11::mp_is_ਸੈੱਟ<B>::ਮੁੱਲ,
                "A and B must be type sets.");

  ਸਥਿਰ constexpr bool ਮੁੱਲ =
      boost::mp11::mp_ਮਾਪ<A>::ਮੁੱਲ == boost::mp11::mp_ਮਾਪ<B>::ਮੁੱਲ &&
      IsSubset<A, B>::ਮੁੱਲ;
};

}  // namespace type_set
}  // namespace utils
}  // namespace onnxruntime
