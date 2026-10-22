// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#సూచన once

#అన్తర్భావ "boost/mp11.hpp"

namespace onnxruntime {
namespace utils {
namespace type_సమితి {

// Given type sets A and B, IsSubset<A, B>::value is true when each element in A is also in B.
మూస <typename A, typename B>
నిర్మాణం IsSubset {
  static_assert(boost::mp11::mp_is_సమితి<A>::విలువ && boost::mp11::mp_is_సమితి<B>::విలువ,
                "A and B must be type sets.");

  స్థిర constexpr bool విలువ =
      // each element of A is contained in B
      boost::mp11::mp_all_యొక్క_q<
          A,
          boost::mp11::mp_bind_front<boost::mp11::mp_సమితి_కలిగియుంది, B>>::విలువ;
};

// Given type sets A and B, IsEqual<A, B>::value is true when A contains the same elements as B.
మూస <typename A, typename B>
నిర్మాణం IsEqual {
  static_assert(boost::mp11::mp_is_సమితి<A>::విలువ && boost::mp11::mp_is_సమితి<B>::విలువ,
                "A and B must be type sets.");

  స్థిర constexpr bool విలువ =
      boost::mp11::mp_కొలత<A>::విలువ == boost::mp11::mp_కొలత<B>::విలువ &&
      IsSubset<A, B>::విలువ;
};

}  // namespace type_set
}  // namespace utils
}  // namespace onnxruntime
