// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#ଅନ୍ତର୍ଭାଵ "boost/mp11.hpp"

namespace onnxruntime {
namespace utils {
namespace type_set {

// Given type sets A and B, IsSubset<A, B>::value is true when each element in A is also in B.
template <typename A, typename B>
ସଂରଚନା IsSubset {
  static_assert(boost::mp11::mp_is_set<A>::ମୂଲ୍ୟ && boost::mp11::mp_is_set<B>::ମୂଲ୍ୟ,
                "A and B must be type sets.");

  ସ୍ଥିର constexpr bool ମୂଲ୍ୟ =
      // each element of A is contained in B
      boost::mp11::mp_all_of_q<
          A,
          boost::mp11::mp_bind_front<boost::mp11::mp_set_contains, B>>::ମୂଲ୍ୟ;
};

// Given type sets A and B, IsEqual<A, B>::value is true when A contains the same elements as B.
template <typename A, typename B>
ସଂରଚନା IsEqual {
  static_assert(boost::mp11::mp_is_set<A>::ମୂଲ୍ୟ && boost::mp11::mp_is_set<B>::ମୂଲ୍ୟ,
                "A and B must be type sets.");

  ସ୍ଥିର constexpr bool ମୂଲ୍ୟ =
      boost::mp11::mp_size<A>::ମୂଲ୍ୟ == boost::mp11::mp_size<B>::ମୂଲ୍ୟ &&
      IsSubset<A, B>::ମୂଲ୍ୟ;
};

}  // namespace type_set
}  // namespace utils
}  // namespace onnxruntime
