// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#അന്തര്ഭാവ "boost/mp11.hpp"

namespace onnxruntime {
namespace utils {
namespace type_set {

// Given type sets A and B, IsSubset<A, B>::value is true when each element in A is also in B.
template <typename A, typename B>
ഘടന IsSubset {
  static_assert(boost::mp11::mp_is_set<A>::മൂല്യം && boost::mp11::mp_is_set<B>::മൂല്യം,
                "A and B must be type sets.");

  സ്ഥിതം constexpr bool മൂല്യം =
      // each element of A is contained in B
      boost::mp11::mp_all_of_q<
          A,
          boost::mp11::mp_bind_front<boost::mp11::mp_set_contains, B>>::മൂല്യം;
};

// Given type sets A and B, IsEqual<A, B>::value is true when A contains the same elements as B.
template <typename A, typename B>
ഘടന IsEqual {
  static_assert(boost::mp11::mp_is_set<A>::മൂല്യം && boost::mp11::mp_is_set<B>::മൂല്യം,
                "A and B must be type sets.");

  സ്ഥിതം constexpr bool മൂല്യം =
      boost::mp11::mp_size<A>::മൂല്യം == boost::mp11::mp_size<B>::മൂല്യം &&
      IsSubset<A, B>::മൂല്യം;
};

}  // namespace type_set
}  // namespace utils
}  // namespace onnxruntime
