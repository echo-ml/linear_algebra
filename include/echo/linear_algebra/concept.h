#pragma once

#include <echo/linear_algebra/structure.h>
#include <echo/linear_algebra/matrix_traits.h>
#include <echo/numeric_array.h>

// namespace echo {
// namespace linear_algebra {
// namespace concept {
//
// /////////////////////////
// // square_matrix_shape //
// /////////////////////////
//
// namespace detail {
// namespace concept {
// struct SquareMatrixShape : Concept {
//   template <class T>
//   auto require(T&& shape) -> list<
//       k_array::concept::k_shape<2, T>(),
//       same<decltype(get_extent<0>(shape)), decltype(get_extent<1>(shape))>()>;
// };
// }
// }
//
// template <class T>
// constexpr bool square_matrix_shape() {
//   return models<detail::concept::SquareMatrixShape, T>();
// }
//
// ////////////
// // matrix //
// ////////////
//
// namespace detail {
// namespace concept {
// struct Matrix : Concept {
//   template <class T>
//   auto require(T&& x) -> list<k_array::concept::k_array<2, T>(),
//                               numeric_array::concept::numeric_array<T>(),
//                               matrix_traits::operation<T>() ==
//                                   execution_context::matrix_operation_t::none>;
// };
// }
// }
//
// template <class T>
// constexpr bool matrix() {
//   return models<detail::concept::Matrix, T>();
// }
//
// ///////////////////////////////
// // modifiable_matrix_forward //
// ///////////////////////////////
//
// namespace detail { namespace concept {
// struct ModifiableMatrixForward : Concept {
//   template<class T>
//   auto require(T&& x) -> list<
//     matrix<uncvref_t<T>>(),
//     echo::concept::writable<decltype(x.data())>()
//   >;
// };
// }}
//
// template<class T>
// constexpr bool modifiable_matrix_forward() {
//   return models<detail::concept::ModifiableMatrixForward, T>();
// }
//
// /////////////////////
// // operated_matrix //
// /////////////////////
//
// namespace detail {
// namespace concept {
// struct OperatedMatrix : Concept {
//   template <class T>
//   auto require(T&& x)
//       -> list<echo::concept::contiguous_iterator<decltype(x.data())>(),
//               k_array::concept::k_shape<2, uncvref_t<decltype(x.shape())>>(),
//               valid<decltype(T::operation)>(),
//               same<execution_context::matrix_operation_t,
//                    uncvref_t<decltype(T::operation)>>(),
//               execution_context::concept::k_compatible_evaluator<
//                   2, decltype(x.evaluator())>(),
//               execution_context::concept::structure<typename T::structure>()>;
// };
// }
// }
//
// template <class T>
// constexpr bool operated_matrix() {
//   return models<detail::concept::OperatedMatrix, T>();
// }
//
// /////////////////
// // weak_matrix //
// /////////////////
//
// template <class T>
// constexpr bool weak_matrix() {
//   return matrix<T>() || operated_matrix<T>();
// }
//
// namespace detail {
// namespace concept {
// template <class Structure>
// struct WeakStructuredMatrix : Concept {
//   template <class T>
//   auto require(T&& x)
//       -> list<weak_matrix<T>(), same<Structure, typename T::structure>()>;
// };
// }
// }
//
// template <class Structure, class T>
// constexpr bool weak_matrix() {
//   return models<detail::concept::WeakStructuredMatrix<Structure>, T>();
// }
//
// ////////////
// // vector //
// ////////////
//
// template <class T>
// constexpr bool vector() {
//   return k_array::concept::k_array<1, T>() &&
//          numeric_array::concept::numeric_array<T>();
// }
//
// ////////////////
// // row_vector //
// ////////////////
//
// namespace detail {
// namespace concept {
// struct RowVector : Concept {
//   template <class T>
//   auto require() -> list<
//       matrix<T>(), same<k_array_traits::extent_type<0, T>, StaticIndex<1>>()>;
// };
// }
// }
//
// template <class T>
// constexpr bool row_vector() {
//   return models<detail::concept::RowVector, T>();
// }
//
// ///////////////////////////////////
// // modifiable_row_vector_forward //
// ///////////////////////////////////
//
// namespace detail {
// namespace concept {
// struct ModifiableRowVectorForward : Concept {
//   template <class T>
//   auto require(T&& x)
//       -> list<row_vector<uncvref_t<T>>(),
//               echo::concept::contiguous_iterator<decltype(x.data())>()>;
// };
// }
// }
//
// template <class T>
// constexpr bool modifiable_row_vector_forward() {
//   return models<detail::concept::ModifiableRowVectorForward, T>();
// }
//
// ///////////////////
// // column_vector //
// ///////////////////
//
// namespace detail {
// namespace concept {
// struct ColumnVector : Concept {
//   template <class T>
//   auto require(T&&) -> list<
//       matrix<T>(), 
//       same<k_array_traits::extent_type<1, T>, StaticIndex<1>>()
//       >;
// };
// }
// }
//
// template <class T>
// constexpr bool column_vector() {
//   return models<detail::concept::ColumnVector, T>();
// }
//
// //////////////////////////////////////
// // modifiable_column_vector_forward //
// //////////////////////////////////////
//
// namespace detail {
// namespace concept {
// struct ModifiableColumnVectorForward : Concept {
//   template <class T>
//   auto require(T&& x)
//       -> list<column_vector<uncvref_t<T>>(),
//               echo::concept::contiguous_iterator<decltype(x.data())>()>;
// };
// }
// }
//
// template <class T>
// constexpr bool modifiable_column_vector_forward() {
//   return models<detail::concept::ModifiableColumnVectorForward, T>();
// }
//
// /////////////////////
// // diagonal_matrix //
// /////////////////////
//
// template <class T>
// constexpr bool diagonal_matrix() {
//   return vector<T>() &&
//          numeric_array::concept::numeric_array<structure::diagonal, T>();
// }
//
// ///////////////////////
// // contiguous_matrix //
// ///////////////////////
//
// template <class T>
// constexpr bool contiguous_matrix() {
//   return k_array::concept::contiguous_k_array<2, T>();
// }
//
// ////////////////////
// // general_matrix //
// ////////////////////
//
// template <class T>
// constexpr bool general_matrix() {
//   return numeric_array::concept::numeric_array<structure::matrix_general,
//                                                T>() &&
//          matrix<T>();
// }
//
// /////////////////////////
// // weak_general_matrix //
// /////////////////////////
//
// template <class T>
// constexpr bool weak_general_matrix() {
//   return weak_matrix<structure::matrix_general, T>();
// }
//
// ///////////////////////////////////////
// // modifiable_general_matrix_forward //
// ///////////////////////////////////////
//
// namespace detail {
// namespace concept {
// struct ModifiableGeneralMatrixForward : Concept {
//   template <class T>
//   auto require(T&& x) -> list<general_matrix<uncvref_t<T>>(),
//                               echo::concept::writable<decltype(x.data())>()>;
// };
// }
// }
//
// template <class T>
// constexpr bool modifiable_general_matrix_forward() {
//   return models<detail::concept::ModifiableGeneralMatrixForward, T>();
// }
//
// //////////////////////
// // symmetric_matrix //
// //////////////////////
//
// namespace detail {
// namespace concept {
// struct SymmetricMatrix : Concept {
//   template <class T>
//   auto require(T&& x) -> list<
//       matrix<T>(), structure::concept::symmetric<typename T::structure>()>;
// };
// }
// }
//
// template <class T>
// constexpr bool symmetric_matrix() {
//   return models<detail::concept::SymmetricMatrix, T>();
// }
//
// ///////////////////////////
// // weak_symmetric_matrix //
// ///////////////////////////
//
// namespace detail {
// namespace concept {
// struct WeakSymmetricMatrix : Concept {
//   template <class T>
//   auto require(T&& x) -> list<
//       weak_matrix<T>(), structure::concept::symmetric<typename T::structure>()>;
// };
// }
// }
//
// template <class T>
// constexpr bool weak_symmetric_matrix() {
//   return weak_matrix<T>() && models<detail::concept::WeakSymmetricMatrix, T>();
// }
//
// /////////////////////////////////////////
// // modifiable_symmetric_matrix_forward //
// /////////////////////////////////////////
//
// namespace detail {
// namespace concept {
// struct ModifiableSymmetricMatrixForward : Concept {
//   template <class T>
//   auto require(T&& x) -> list<symmetric_matrix<uncvref_t<T>>(),
//                               echo::concept::writable<decltype(x.data())>()>;
// };
// }
// }
//
// template <class T>
// constexpr bool modifiable_symmetric_matrix_forward() {
//   return models<detail::concept::ModifiableSymmetricMatrixForward, T>();
// }
//
// ///////////////////////
// // triangular_matrix //
// ///////////////////////
//
// namespace detail {
// namespace concept {
// struct TriangularMatrix : Concept {
//   template <class T>
//   auto require(T&& x) -> list<
//       matrix<T>(), structure::concept::triangular<typename T::structure>()>;
// };
// }
// }
//
// template <class T>
// constexpr bool triangular_matrix() {
//   return models<detail::concept::TriangularMatrix, T>();
// }
//
// ////////////////////////////
// // weak_triangular_matrix //
// ////////////////////////////
//
// namespace detail {
// namespace concept {
// struct WeakTriangularMatrix : Concept {
//   template <class T>
//   auto require(T&& x)
//       -> list<weak_matrix<T>(),
//               structure::concept::triangular<typename T::structure>()>;
// };
// }
// }
//
// template <class T>
// constexpr bool weak_triangular_matrix() {
//   return models<detail::concept::WeakTriangularMatrix, T>();
// }
//
// //////////////////////////////////////////
// // modifiable_triangular_matrix_forward //
// //////////////////////////////////////////
//
// namespace detail {
// namespace concept {
// struct ModifiableTriangularMatrixForward : Concept {
//   template <class T>
//   auto require(T&& x) -> list<triangular_matrix<uncvref_t<T>>(),
//                               echo::concept::writable<decltype(x.data())>()>;
// };
// }
// }
//
// template <class T>
// constexpr bool modifiable_triangular_matrix_forward() {
//   return models<detail::concept::ModifiableTriangularMatrixForward, T>();
// }
//
// /////////////////////////////
// // lower_triangular_matrix //
// /////////////////////////////
//
// template <class T>
// constexpr bool lower_triangular_matrix() {
//   return numeric_array::concept::numeric_array<structure::lower_triangular,
//                                                T>() &&
//          matrix<T>();
// }
//
// //////////////////////////////////
// // weak_lower_triangular_matrix //
// //////////////////////////////////
//
// template <class T>
// constexpr bool weak_lower_triangular_matrix() {
//   return weak_matrix<structure::lower_triangular, T>();
// }
//
// ////////////////////////////////////////////////
// // modifiable_lower_triangular_matrix_forward //
// ////////////////////////////////////////////////
//
// namespace detail {
// namespace concept {
// struct ModifiableLowerTriangularMatrixForward : Concept {
//   template <class T>
//   auto require(T&& x) -> list<lower_triangular_matrix<uncvref_t<T>>(),
//                               echo::concept::writable<decltype(x.data())>()>;
// };
// }
// }
//
// template <class T>
// constexpr bool modifiable_lower_triangular_matrix_forward() {
//   return models<detail::concept::ModifiableLowerTriangularMatrixForward, T>();
// }
//
// /////////////////////////////
// // upper_triangular_matrix //
// /////////////////////////////
//
// template <class T>
// constexpr bool upper_triangular_matrix() {
//   return numeric_array::concept::numeric_array<structure::upper_triangular,
//                                                T>() &&
//          matrix<T>();
// }
//
// //////////////////////////////////
// // weak_upper_triangular_matrix //
// //////////////////////////////////
//
// template <class T>
// constexpr bool weak_upper_triangular_matrix() {
//   return weak_matrix<structure::upper_triangular, T>();
// }
//
// ////////////////////////////////////////////////
// // modifiable_upper_triangular_matrix_forward //
// ////////////////////////////////////////////////
//
// namespace detail {
// namespace concept {
// struct ModifiableUpperTriangularMatrixForward : Concept {
//   template <class T>
//   auto require(T&& x) -> list<upper_triangular_matrix<uncvref_t<T>>(),
//                               echo::concept::writable<decltype(x.data())>()>;
// };
// }
// }
//
// template <class T>
// constexpr bool modifiable_upper_triangular_matrix_forward() {
//   return models<detail::concept::ModifiableUpperTriangularMatrixForward, T>();
// }
//
// ///////////////////////
// // matrix_expression //
// ///////////////////////
//
// template <class T>
// constexpr bool matrix_expression() {
//   return numeric_array::concept::shaped_expression<2, T>();
// }
// }
// }
// }
