#pragma once

#include <echo/linear_algebra/blas_concept.h>
#include <echo/linear_algebra/structure.h>
#include <echo/linear_algebra/concept.h>

namespace echo {
namespace linear_algebra {

//------------------------------------------------------------------------------
// get_leading_dimension
//------------------------------------------------------------------------------
template <class A, CONCEPT_REQUIRES(concept::matrix<A>())>
auto get_leading_dimension(const A& a) {
  return get_stride<1>(a);
}

template <class A, CONCEPT_REQUIRES(concept::operated_matrix<A>())>
auto get_leading_dimension(const A& a) {
  return get_stride<0>(a);
}

//------------------------------------------------------------------------------
// get_underlying_num_rows
//------------------------------------------------------------------------------
template <class A, CONCEPT_REQUIRES(concept::matrix<A>())>
auto get_underlying_num_rows(const A& a) {
  return get_extent<0>(a);
}

template <class A, CONCEPT_REQUIRES(concept::operated_matrix<A>())>
auto get_underlying_num_rows(const A& a) {
  return get_extent<1>(a);
}

//------------------------------------------------------------------------------
// get_underlying_num_columns
//------------------------------------------------------------------------------
template <class A, CONCEPT_REQUIRES(concept::matrix<A>())>
auto get_underlying_num_columns(const A& a) {
  return get_extent<1>(a);
}

template <class A, CONCEPT_REQUIRES(concept::operated_matrix<A>())>
auto get_underlying_num_columns(const A& a) {
  return get_extent<0>(a);
}

//------------------------------------------------------------------------------
// is_product_shaped
//------------------------------------------------------------------------------
template <class A, class B, CONCEPT_REQUIRES(concept::weak_matrix<A>() &&
                                             concept::weak_matrix<B>())>
bool is_product_shaped(const A& a, const B& b) {
  return get_extent<1>(a) == get_extent<0>(b);
}

template <class A, class B, class C,
          CONCEPT_REQUIRES(concept::weak_matrix<A>() &&
                           concept::weak_matrix<B>() &&
                           concept::weak_matrix<C>())>
bool is_product_shaped(const A& a, const B& b, const C& c) {
  return is_product_shaped(a, b) && get_extent<0>(a) == get_extent<0>(c) &&
         get_extent<1>(b) == get_extent<1>(c);
}
}

//------------------------------------------------------------------------------
// get_underlying_triangular_matrix_uplo
//------------------------------------------------------------------------------
namespace matrix_traits {
template <class A,
          CONCEPT_REQUIRES(linear_algebra::concept::triangular_matrix<A>())>
constexpr auto get_underlying_triangular_matrix_uplo() {
  return A::structure::uplo;
}

template <class A, CONCEPT_REQUIRES(
                       linear_algebra::concept::weak_triangular_matrix<A>() &&
                       linear_algebra::concept::operated_matrix<A>())>
constexpr auto get_underlying_triangular_matrix_uplo() {
  return linear_algebra::structure::transpose<typename A::structure>::uplo;
}
}
}
