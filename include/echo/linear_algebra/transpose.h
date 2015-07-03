#pragma once

#include <echo/linear_algebra/matrix_operation_expression.h>
#include <echo/linear_algebra/concept.h>

// namespace echo {
// namespace linear_algebra {
//
// /////////////////////
// // transpose_shape //
// /////////////////////
//
// namespace detail {
// namespace transpose {
//
// template <index_t M, index_t N>
// auto get_transposed_shape(const KShape<M, N>& shape) {
//   auto dynamic_extents = shape.dynamic_extents();
//   std::reverse(dynamic_extents.begin(), dynamic_extents.end());
//   return KShape<N, M>(dynamic_extents);
// }
//
// template <index_t M, index_t N, CONCEPT_REQUIRES(M != Dimension::kDynamic)>
// auto transpose_shape(const KShape<M, N>& shape) {
//   auto transposed_shape = get_transposed_shape(shape);
//   return k_array::KSubshape<decltype(transposed_shape),
//                             k_array::KShapeStrides<M, 1>>(
//       transposed_shape, k_array::KShapeStrides<M, 1>());
// }
//
// template <index_t M, index_t N, CONCEPT_REQUIRES(M == Dimension::kDynamic)>
// auto transpose_shape(const KShape<M, N>& shape) {
//   auto transposed_shape = get_transposed_shape(shape);
//   return k_array::KSubshape<decltype(transposed_shape),
//                             k_array::KShapeStrides<M, 1>>(
//       transposed_shape, k_array::KShapeStrides<M, 1>(get_stride<1>(shape)));
// }
//
// template <index_t M, index_t N, index_t S1, index_t S2>
// auto transpose_shape(
//     const KSubshape<KShape<M, N>, k_array::KShapeStrides<S1, S2>>& shape) {
//   auto transposed_shape = get_transposed_shape(shape);
//   auto dynamic_strides = shape.dyanmic_strides();
//   std::reverse(dynamic_strides.begin(), dynamic_strides.end());
//   return k_array::KSubshape<decltype(transposed_shape),
//                             k_array::KShapeStrides<S2, S1>>(
//       transposed_shape, k_array::KShapeStrides<S2, S1>(dynamic_strides));
// }
// }
// }  // end namespace detail::shape
//
// ///////////////
// // transpose //
// ///////////////
//
// template <class Matrix,
//           CONCEPT_REQUIRES(concept::matrix<uncvref_t<Matrix>>() &&
//                            !concept::symmetric_matrix<uncvref_t<Matrix>>())>
// auto transpose(Matrix&& matrix) {
//   return make_matrix_operation_expression<
//       numeric_array_traits::structure<Matrix>, matrix_operation_t::transpose>(
//       matrix.data(), detail::transpose::transpose_shape(matrix.shape()));
// }
//
// template <class Matrix,
//           CONCEPT_REQUIRES(concept::symmetric_matrix<uncvref_t<Matrix>>())>
// auto transpose(Matrix&& matrix) -> decltype(std::forward<Matrix>(matrix)) {
//   return std::forward<Matrix>(matrix);
// }
//
// template <class Expression,
//           CONCEPT_REQUIRES(concept::matrix_expression<Expression>())>
// auto transpose(const Expression& expression) {
//   return numeric_array::make_numeric_array_expression<
//       structure::transpose<expression_traits::structure<Expression>>>(
//       get_extent_shape(detail::transpose::transpose_shape(expression.shape())),
//       make_operation_evaluator<matrix_operation_t::transpose>(
//           expression.evaluator()));
// }
// }
// }
