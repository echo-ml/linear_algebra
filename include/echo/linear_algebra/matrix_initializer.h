#pragma once

#include <echo/linear_algebra/structure.h>
#include <echo/numeric_array.h>

// namespace echo {
// namespace numeric_array {
//
// /////////////////////////////
// // NumericArrayInitializer //
// /////////////////////////////
//
// // symmetric
// template <class Derived, class Scalar, index_t Dimension,
//           linear_algebra::structure::uplo_t Uplo>
// struct NumericArrayInitializer<
//     Derived, Scalar, KShape<Dimension, Dimension>,
//     linear_algebra::structure::symmetric_uplo<Uplo> > {
//   void initialize(InitializerMultilist<Scalar, 2> values) {
//     auto& derived = static_cast<Derived&>(*this);
//     auto n = get_extent<0>(derived);
//     auto accessor = InitializerMultilistAccessor<Scalar, 2>(values);
//     for (int i = 0; i < n; ++i) {
//       for (int j = 0; j <= i; ++j) {
//         if (accessor(i, j) != accessor(j, i)) throw InitializationError();
//         derived(i, j) = accessor(i, j);
//       }
//     }
//   }
// };
//
// // lower_triangular
// template <class Derived, class Scalar, index_t Dimension>
// struct NumericArrayInitializer<Derived, Scalar, KShape<Dimension, Dimension>,
//                                linear_algebra::structure::lower_triangular> {
//   void initialize(InitializerMultilist<Scalar, 2> values) {
//     auto& derived = static_cast<Derived&>(*this);
//     auto n = get_extent<0>(derived);
//     auto accessor = InitializerMultilistAccessor<Scalar, 2>(values);
//     for (int i = 0; i < n; ++i) {
//       for (int j = 0; j <= i; ++j) {
//         if (i != j && accessor(j, i) != 0) throw InitializationError();
//         derived(access_mode::readwrite, i, j) = accessor(i, j);
//       }
//     }
//   }
// };
//
// // upper_triangular
// template <class Derived, class Scalar, index_t Dimension>
// struct NumericArrayInitializer<Derived, Scalar, KShape<Dimension, Dimension>,
//                                linear_algebra::structure::upper_triangular> {
//   void initialize(InitializerMultilist<Scalar, 2> values) {
//     auto& derived = static_cast<Derived&>(*this);
//     auto n = get_extent<0>(derived);
//     auto accessor = InitializerMultilistAccessor<Scalar, 2>(values);
//     for (int i = 0; i < n; ++i) {
//       for (int j = 0; j <= i; ++j) {
//         if (i != j && accessor(i, j) != 0) throw InitializationError();
//         derived(access_mode::readwrite, j, i) = accessor(j, i);
//       }
//     }
//   }
// };
// }
// }
