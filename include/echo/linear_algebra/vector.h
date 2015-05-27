#pragma once

#include <echo/linear_algebra/diagonal_matrix_evaluator.h>
#include <echo/linear_algebra/concept.h>
#include <echo/linear_algebra/structure.h>
#include <echo/execution_context.h>
#include <echo/numeric_array.h>

namespace echo {
namespace linear_algebra {

/////////////////
// vector_type //
/////////////////

namespace detail {
namespace vector {

template <class Scalar, class Structure, class Shape,
          class MemoryBackend = SimdAllocator<Scalar>,
          CONCEPT_REQUIRES(execution_context::concept::scalar<Scalar>() &&
                           execution_context::concept::structure<Structure>() &&
                           k_array::concept::contiguous_k_shape<1, Shape>() &&
                           memory::concept::memory_backend<MemoryBackend>())>
auto vector_type() -> NumericArray<Scalar, Shape, Structure, MemoryBackend>;

template <class Scalar, class Structure, class Extent,
          class MemoryBackend = SimdAllocator<Scalar>,
          CONCEPT_REQUIRES(execution_context::concept::scalar<Scalar>() &&
                           execution_context::concept::structure<Structure>() &&
                           k_array::concept::extent<Extent>() &&
                           memory::concept::memory_backend<MemoryBackend>())>
auto vector_type() -> NumericArray<Scalar, KShapeFromExtents<Extent>, Structure,
                                   MemoryBackend>;
}
}

////////////
// Vector //
////////////

template <class Scalar, class... Specifiers>
using Vector = decltype(
    detail::vector::vector_type<Scalar, structure::general, Specifiers...>());

////////////////////
// DiagonalMatrix //
////////////////////

template <class Scalar, class... Specifiers>
using DiagonalMatrix = decltype(
    detail::vector::vector_type<Scalar, structure::diagonal, Specifiers...>());
}

/////////////////////
// make_expression //
/////////////////////

namespace numeric_array {
namespace detail {
namespace vector {

template <class NumericArray,
          CONCEPT_REQUIRES(linear_algebra::concept::diagonal_matrix<
              uncvref_t<NumericArray>>())>
auto make_diagonal_matrix_expression_impl(NumericArray&& numeric_array) {
  auto shape = get_extent_shape(numeric_array.shape());
  return echo::numeric_array::make_numeric_array_expression<
      expression_traits::structure<uncvref_t<NumericArray>>>(
      make_k_shape(get_extent<0>(shape), get_extent<0>(shape)),
      linear_algebra::make_diagonal_matrix_evaluator(
          make_numeric_array_evaluator_impl(numeric_array)));
}
}
}

template <class Scalar, class Shape, class MemoryBackend>
auto make_expression(
    numeric_array_expression_tag,
    const NumericArray<Scalar, Shape, linear_algebra::structure::diagonal,
                       MemoryBackend>& numeric_array) {
  return detail::vector::make_diagonal_matrix_expression_impl(numeric_array);
}

template <class Scalar, class Shape, class MemoryBackend>
auto make_expression(
    numeric_array_expression_tag,
    NumericArray<Scalar, Shape, linear_algebra::structure::diagonal,
                 MemoryBackend>& numeric_array) {
  return detail::vector::make_diagonal_matrix_expression_impl(numeric_array);
}
}
}
