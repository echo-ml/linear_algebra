#pragma once

#define DETAIL_NS detail_vector

#include <echo/linear_algebra/diagonal_matrix_evaluator.h>
#include <echo/linear_algebra/concept.h>
#include <echo/linear_algebra/structure.h>
#include <echo/execution_context.h>
#include <echo/numeric_array.h>

namespace echo {
namespace linear_algebra {

//------------------------------------------------------------------------------
// vector_type
//------------------------------------------------------------------------------
namespace DETAIL_NS {
template <class Scalar, class Structure, class Shape,
          class MemoryBackend = memory::SimdAllocator<Scalar>,
          CONCEPT_REQUIRES(execution_context::concept::scalar<Scalar>() &&
                           execution_context::concept::structure<Structure>() &&
                           k_array::concept::contiguous_shape<Shape>() &&
                           k_array::concept::shape<1, Shape>() &&
                           memory::concept::memory_backend<MemoryBackend>())>
auto vector_type() -> NumericArray<Scalar, Shape, Structure, MemoryBackend>;

template <class Scalar, class Structure, class Extent,
          class MemoryBackend = memory::SimdAllocator<Scalar>,
          CONCEPT_REQUIRES(execution_context::concept::scalar<Scalar>() &&
                           execution_context::concept::structure<Structure>() &&
                           k_array::concept::extent<Extent>() &&
                           memory::concept::memory_backend<MemoryBackend>())>
auto vector_type()
    -> NumericArray<Scalar, Shape<Extent>, Structure, MemoryBackend>;
}

//------------------------------------------------------------------------------
// row_vector_type
//------------------------------------------------------------------------------
namespace DETAIL_NS {
template <class Scalar, class Shape,
          class MemoryBackend = memory::SimdAllocator<Scalar>,
          CONCEPT_REQUIRES(execution_context::concept::scalar<Scalar>() &&
                           k_array::concept::contiguous_shape<Shape>() &&
                           k_array::concept::shape<2, Shape>() &&
                           std::is_same<shape_traits::extent_type<0, Shape>,
                                        StaticIndex<1>>::value &&
                           memory::concept::memory_backend<MemoryBackend>())>
auto row_vector_type()
    -> NumericArray<Scalar, Shape, structure::matrix_general, MemoryBackend>;

template <class Scalar, class Extent,
          class MemoryBackend = memory::SimdAllocator<Scalar>,
          CONCEPT_REQUIRES(execution_context::concept::scalar<Scalar>() &&
                           k_array::concept::extent<Extent>() &&
                           memory::concept::memory_backend<MemoryBackend>())>
auto row_vector_type()
    -> NumericArray<Scalar, Shape<StaticIndex<1>, Extent>,
                    structure::matrix_general, MemoryBackend>;
}

//------------------------------------------------------------------------------
// column_vector_type
//------------------------------------------------------------------------------
namespace DETAIL_NS {
template <class Scalar, class Shape,
          class MemoryBackend = memory::SimdAllocator<Scalar>,
          CONCEPT_REQUIRES(execution_context::concept::scalar<Scalar>() &&
                           k_array::concept::contiguous_shape<Shape>() &&
                           k_array::concept::shape<2, Shape>() &&
                           std::is_same<shape_traits::extent_type<1, Shape>,
                                        StaticIndex<1>>::value &&
                           memory::concept::memory_backend<MemoryBackend>())>
auto column_vector_type()
    -> NumericArray<Scalar, Shape, structure::matrix_general, MemoryBackend>;

template <class Scalar, class Extent,
          class MemoryBackend = memory::SimdAllocator<Scalar>,
          CONCEPT_REQUIRES(execution_context::concept::scalar<Scalar>() &&
                           k_array::concept::extent<Extent>() &&
                           memory::concept::memory_backend<MemoryBackend>())>
auto column_vector_type()
    -> NumericArray<Scalar, Shape<Extent, StaticIndex<1>>,
                    structure::matrix_general, MemoryBackend>;
}

//------------------------------------------------------------------------------
// Vector
//------------------------------------------------------------------------------
template <class Scalar, class... Specifiers>
using Vector = decltype(
    DETAIL_NS::vector_type<Scalar, structure::general, Specifiers...>());

//------------------------------------------------------------------------------
// RowVector
//------------------------------------------------------------------------------
template <class Scalar, class... Specifiers>
using RowVector = decltype(DETAIL_NS::row_vector_type<Scalar, Specifiers...>());

//------------------------------------------------------------------------------
// ColumnVector
//------------------------------------------------------------------------------
template <class Scalar, class... Specifiers>
using ColumnVector =
    decltype(DETAIL_NS::column_vector_type<Scalar, Specifiers...>());

//------------------------------------------------------------------------------
// DiagonalMatrix
//------------------------------------------------------------------------------
template <class Scalar, class... Specifiers>
using DiagonalMatrix = decltype(
    DETAIL_NS::vector_type<Scalar, structure::diagonal, Specifiers...>());
}

//------------------------------------------------------------------------------
// make_expression
//------------------------------------------------------------------------------
namespace numeric_array {
namespace DETAIL_NS {
template <class NumericArray,
          CONCEPT_REQUIRES(linear_algebra::concept::diagonal_matrix<
              uncvref_t<NumericArray>>())>
auto make_diagonal_matrix_expression_impl(NumericArray&& numeric_array) {
  auto shape = numeric_array.shape();
  return echo::numeric_array::make_numeric_array_expression<
      expression_traits::structure<uncvref_t<NumericArray>>>(
      make_dimensionality(get_extent<0>(shape), get_extent<0>(shape)),
      linear_algebra::make_diagonal_matrix_evaluator(
          make_numeric_array_evaluator(numeric_array)));
}
}

template <class Scalar, class Shape, class MemoryBackend>
auto make_expression(
    numeric_array_expression_tag,
    const NumericArray<Scalar, Shape, linear_algebra::structure::diagonal,
                       MemoryBackend>& numeric_array) {
  return DETAIL_NS::make_diagonal_matrix_expression_impl(numeric_array);
}

template <class Scalar, class Shape, class MemoryBackend>
auto make_expression(
    numeric_array_expression_tag,
    NumericArray<Scalar, Shape, linear_algebra::structure::diagonal,
                 MemoryBackend>& numeric_array) {
  return DETAIL_NS::make_diagonal_matrix_expression_impl(numeric_array);
}
}
}

#undef DETAIL_NS
