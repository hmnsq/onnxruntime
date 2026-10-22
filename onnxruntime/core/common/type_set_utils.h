// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ಸೂಚನೆ once

#ಅನ್ತರ್ಭಾವ "boost/mp11.hpp"

namespace onnxruntime {
namespace utils {
namespace type_ಗಣ {

// Given type sets A and B, IsSubset<A, B>::value is true when each element in A is also in B.
ಮಾದರಿ <typename A, typename B>
ರಚನೆ IsSubset {
  static_assert(boost::mp11::mp_is_ಗಣ<A>::ಮೌಲ್ಯ && boost::mp11::mp_is_ಗಣ<B>::ಮೌಲ್ಯ,
                "A and B must be type sets.");

  ಸ್ಥಿರ constexpr bool ಮೌಲ್ಯ =
      // each element of A is contained in B
      boost::mp11::mp_all_ದ_q<
          A,
          boost::mp11::mp_bind_front<boost::mp11::mp_ಗಣ_ಒಳಗೊಂಡಿದೆ, B>>::ಮೌಲ್ಯ;
};

// Given type sets A and B, IsEqual<A, B>::value is true when A contains the same elements as B.
ಮಾದರಿ <typename A, typename B>
ರಚನೆ IsEqual {
  static_assert(boost::mp11::mp_is_ಗಣ<A>::ಮೌಲ್ಯ && boost::mp11::mp_is_ಗಣ<B>::ಮೌಲ್ಯ,
                "A and B must be type sets.");

  ಸ್ಥಿರ constexpr bool ಮೌಲ್ಯ =
      boost::mp11::mp_ಗಾತ್ರ<A>::ಮೌಲ್ಯ == boost::mp11::mp_ಗಾತ್ರ<B>::ಮೌಲ್ಯ &&
      IsSubset<A, B>::ಮೌಲ್ಯ;
};

}  // namespace type_set
}  // namespace utils
}  // namespace onnxruntime
