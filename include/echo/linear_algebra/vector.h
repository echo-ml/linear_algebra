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
template <class Scalar, class Structure, class Shape = Shape<index_t>,
          class MemoryBackend = memory::SimdAllocator<Scalar>,
          CONCEPT_REQUIRES(execution_context::concept::scalar<Scalar>() &&
                           execution_context::concept::structure<Structure>() &&
                           k_array::concept::contiguous_shape<Shape>() &&
                           k_array::concept::shape_<1, Shape>() &&
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
template <class Scalar, class Shape = Shape<StaticIndex<1>, index_t>,
          class MemoryBackend = memory::SimdAllocator<Scalar>,
          CONCEPT_REQUIRES(execution_context::concept::scalar<Scalar>() &&
                           k_array::concept::contiguous_shape<Shape>() &&
                           k_array::concept::shape_<2, Shape>() &&
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
template <class Scalar, class Shape = Shape<index_t, StaticIndex<1>>,
          class MemoryBackend = memory::SimdAllocator<Scalar>,
          CONCEPT_REQUIRES(execution_context::concept::scalar<Scalar>() &&
                           k_array::concept::contiguous_shape<Shape>() &&
                           k_array::concept::shape_<2, Shape>() &&
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
// StaticVector
//------------------------------------------------------------------------------
template <class Scalar, class... Specifiers>
using StaticVector =
    Vector<Scalar, Specifiers..., memory::SimdStaticAllocator<Scalar>>;

//------------------------------------------------------------------------------
// RowVector
//------------------------------------------------------------------------------
template <class Scalar, class... Specifiers>
using RowVector = decltype(DETAIL_NS::row_vector_type<Scalar, Specifiers...>());

//------------------------------------------------------------------------------
// StaticRowVector
//------------------------------------------------------------------------------
template <class Scalar, class... Specifiers>
using StaticRowVector =
    RowVector<Scalar, Specifiers..., memory::SimdStaticAllocator<Scalar>>;

//------------------------------------------------------------------------------
// ColumnVector
//------------------------------------------------------------------------------
template <class Scalar, class... Specifiers>
using ColumnVector =
    decltype(DETAIL_NS::column_vector_type<Scalar, Specifiers...>());

//------------------------------------------------------------------------------
// StaticColumnVector
//------------------------------------------------------------------------------
template <class Scalar, class... Specifiers>
using StaticColumnVector =
    ColumnVector<Scalar, Specifiers..., memory::SimdStaticAllocator<Scalar>>;

//------------------------------------------------------------------------------
// DiagonalMatrix
//------------------------------------------------------------------------------
template <class Scalar, class... Specifiers>
using DiagonalMatrix = decltype(
    DETAIL_NS::vector_type<Scalar, structure::diagonal, Specifiers...>());

//------------------------------------------------------------------------------
// StaticDiagonalMatrix
//------------------------------------------------------------------------------
template <class Scalar, class... Specifiers>
using StaticDiagonalMatrix =
    DiagonalMatrix<Scalar, Specifiers..., memory::SimdStaticAllocator<Scalar>>;

//------------------------------------------------------------------------------
// make_vector
//------------------------------------------------------------------------------
template <class Scalar, class Structure, class Extent,
          class Allocator = memory::SimdAllocator<Scalar>,
          CONCEPT_REQUIRES(execution_context::concept::scalar<Scalar>() &&
                           execution_context::concept::structure<Structure>() &&
                           std::is_convertible<Extent, index_t>() &&
                           memory::concept::memory_backend<Allocator>())>
auto make_vector(Extent extent, const Allocator& allocator = Allocator()) {
  return make_numeric_array<Scalar, Structure>(make_dimensionality(extent),
                                               allocator);
}

template <class Scalar, class Extent,
          class Allocator = memory::SimdAllocator<Scalar>,
          CONCEPT_REQUIRES(execution_context::concept::scalar<Scalar>() &&
                           std::is_convertible<Extent, index_t>() &&
                           memory::concept::memory_backend<Allocator>())>
auto make_vector(Extent extent, const Allocator& allocator = Allocator()) {
  return make_numeric_array<Scalar, structure::general>(
      make_dimensionality(extent), allocator);
}

//------------------------------------------------------------------------------
// make_static_vector
//------------------------------------------------------------------------------
template <class Scalar, class Structure, class Extent,
          CONCEPT_REQUIRES(execution_context::concept::scalar<Scalar>() &&
                           execution_context::concept::structure<Structure>() &&
                           k_array::concept::static_extent<Extent>())>
auto make_static_vector(Extent extent) {
  return make_static_numeric_array<Scalar, Structure>(make_dimensionality(extent));
}

template <class Scalar, class Extent,
          CONCEPT_REQUIRES(execution_context::concept::scalar<Scalar>() &&
                           k_array::concept::static_extent<Extent>())>
auto make_static_vector(Extent extent) {
  return make_static_numeric_array<Scalar, structure::general>(
      make_dimensionality(extent));
}

//------------------------------------------------------------------------------
// make_row_vector
//------------------------------------------------------------------------------
template <class Scalar, class Extent,
          class Allocator = memory::SimdAllocator<Scalar>,
          CONCEPT_REQUIRES(execution_context::concept::scalar<Scalar>() &&
                           std::is_convertible<Extent, index_t>() &&
                           memory::concept::memory_backend<Allocator>())>
auto make_row_vector(Extent extent, const Allocator& allocator = Allocator()) {
  return make_numeric_array<Scalar, structure::matrix_general>(
      make_dimensionality(1_index, extent), allocator);
}

//------------------------------------------------------------------------------
// make_static_row_vector
//------------------------------------------------------------------------------
template <class Scalar, class Extent,
          CONCEPT_REQUIRES(execution_context::concept::scalar<Scalar>() &&
                           k_array::concept::static_extent<Extent>())>
auto make_static_row_vector(Extent extent) {
  return make_static_numeric_array<Scalar, structure::matrix_general>(
      make_dimensionality(1_index, extent));
}


//------------------------------------------------------------------------------
// make_column_vector
//------------------------------------------------------------------------------
template <class Scalar, class Extent,
          class Allocator = memory::SimdAllocator<Scalar>,
          CONCEPT_REQUIRES(execution_context::concept::scalar<Scalar>() &&
                           std::is_convertible<Extent, index_t>() &&
                           memory::concept::memory_backend<Allocator>())>
auto make_column_vector(Extent extent,
                        const Allocator& allocator = Allocator()) {
  return make_numeric_array<Scalar, structure::matrix_general>(
      make_dimensionality(extent, 1_index), allocator);
}

//------------------------------------------------------------------------------
// make_static_column_vector
//------------------------------------------------------------------------------
template <class Scalar, class Extent,
          CONCEPT_REQUIRES(execution_context::concept::scalar<Scalar>() &&
                           k_array::concept::static_extent<Extent>())>
auto make_static_column_vector(Extent extent) {
  return make_static_numeric_array<Scalar, structure::matrix_general>(
      make_dimensionality(extent, 1_index));
}


//------------------------------------------------------------------------------
// get_vector_stride
//------------------------------------------------------------------------------
template <class X, CONCEPT_REQUIRES(concept::vector<X>())>
auto get_vector_stride(const X& x) {
  return get_stride<0>(x);
}

template <class X, CONCEPT_REQUIRES(concept::row_vector<X>() &&
                                    !concept::column_vector<X>())>
auto get_vector_stride(const X& x) {
  return get_stride<1>(x);
}

template <class X, CONCEPT_REQUIRES(concept::column_vector<X>())>
auto get_vector_stride(const X& x) {
  return get_stride<0>(x);
}

//------------------------------------------------------------------------------
// make_vector_view
//------------------------------------------------------------------------------
template <class X, CONCEPT_REQUIRES(concept::vector<uncvref_t<X>>() ||
                                    concept::row_vector<uncvref_t<X>>() ||
                                    concept::column_vector<uncvref_t<X>>())>
auto make_vector_view(X&& x) {
  return make_numeric_array_view<structure::general>(
      x.data(), make_subshape(make_dimensionality(get_num_elements(x)),
                              make_strides(get_vector_stride(x))));
}

//------------------------------------------------------------------------------
// make_vector_cview
//------------------------------------------------------------------------------
template <class X>
auto make_vector_cview(X&& x) -> decltype(make_cview(make_vector_view(x))) {
  return make_cview(make_vector_view(x));
}

//------------------------------------------------------------------------------
// make_row_vector_view
//------------------------------------------------------------------------------
template <class X, CONCEPT_REQUIRES((concept::vector<uncvref_t<X>>() ||
                                     concept::row_vector<uncvref_t<X>>() ||
                                     concept::column_vector<uncvref_t<X>>()))>
auto make_row_vector_view(X&& x) {
  return make_numeric_array_view<structure::matrix_general>(
      x.data(), make_subshape(make_dimensionality(1_index, get_num_elements(x)),
                              make_strides(1_index, get_vector_stride(x))));
}

//------------------------------------------------------------------------------
// make_row_vector_cview
//------------------------------------------------------------------------------
template <class X>
auto make_row_vector_cview(X&& x)
    -> decltype(make_cview(make_row_vector_view(x))) {
  return make_cview(make_row_vector_view(x));
}

//------------------------------------------------------------------------------
// make_column_vector_view
//------------------------------------------------------------------------------
template <class X, CONCEPT_REQUIRES(concept::vector<uncvref_t<X>>() ||
                                    concept::row_vector<uncvref_t<X>>() ||
                                    concept::column_vector<uncvref_t<X>>())>
auto make_column_vector_view(X&& x) {
  auto stride = get_vector_stride(x);
  return make_numeric_array_view<structure::matrix_general>(
      x.data(),
      make_subshape(make_dimensionality(get_num_elements(x), 1_index),
                    make_strides(stride, stride * get_num_elements(x))));
}

//------------------------------------------------------------------------------
// make_column_vector_cview
//------------------------------------------------------------------------------
template <class X>
auto make_column_vector_cview(X&& x)
    -> decltype(make_cview(make_column_vector_view(x))) {
  return make_cview(make_column_vector_view(x));
}
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
