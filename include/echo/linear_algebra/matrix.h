#pragma once

#define DETAIL_NS detail_matrix

#include <echo/linear_algebra/concept.h>
#include <echo/linear_algebra/structure.h>
#include <echo/linear_algebra/matrix_accessor.h>
#include <echo/linear_algebra/matrix_initializer.h>
#include <echo/execution_context.h>
#include <echo/numeric_array.h>

namespace echo {
namespace linear_algebra {

//------------------------------------------------------------------------------
// get_num_rows
//------------------------------------------------------------------------------
template <class Matrix, CONCEPT_REQUIRES(concept::weak_matrix<Matrix>())>
auto get_num_rows(const Matrix& matrix) {
  return get_extent<0>(matrix);
}

//------------------------------------------------------------------------------
// get_num_columns
//------------------------------------------------------------------------------
template <class Matrix, CONCEPT_REQUIRES(concept::weak_matrix<Matrix>())>
auto get_num_columns(const Matrix& matrix) {
  return get_extent<1>(matrix);
}

//------------------------------------------------------------------------------
// matrix_type
//------------------------------------------------------------------------------
namespace DETAIL_NS {
template <class Scalar, class Structure, class Shape = Shape<index_t, index_t>,
          class MemoryBackend = memory::SimdAllocator<Scalar>,
          CONCEPT_REQUIRES(execution_context::concept::scalar<Scalar>() &&
                           execution_context::concept::structure<Structure>() &&
                           !structure::concept::square<Structure>() &&
                           k_array::concept::contiguous_shape<Shape>() &&
                           k_array::concept::shape_<2, Shape>() &&
                           memory::concept::memory_backend<MemoryBackend>())>
auto matrix_type() -> NumericArray<Scalar, Shape, Structure, MemoryBackend>;

template <class Scalar, class Structure, class Extent1, class Extent2,
          class MemoryBackend = memory::SimdAllocator<Scalar>,
          CONCEPT_REQUIRES(execution_context::concept::scalar<Scalar>() &&
                           execution_context::concept::structure<Structure>() &&
                           !structure::concept::square<Structure>() &&
                           k_array::concept::extent<Extent1>() &&
                           k_array::concept::extent<Extent2>() &&
                           memory::concept::memory_backend<MemoryBackend>())>
auto matrix_type()
    -> NumericArray<Scalar, Shape<Extent1, Extent2>, Structure, MemoryBackend>;

template <class Scalar, class Structure, class MemoryBackend,
          CONCEPT_REQUIRES(execution_context::concept::scalar<Scalar>() &&
                           execution_context::concept::structure<Structure>() &&
                           memory::concept::memory_backend<MemoryBackend>())>
auto matrix_type()
    -> NumericArray<Scalar, Shape<index_t, index_t>, Structure, MemoryBackend>;

template <class Scalar, class Structure, class Shape = Shape<index_t, index_t>,
          class MemoryBackend = memory::SimdAllocator<Scalar>,
          CONCEPT_REQUIRES(execution_context::concept::scalar<Scalar>() &&
                           structure::concept::square<Structure>() &&
                           k_array::concept::contiguous_shape<Shape>() &&
                           k_array::concept::shape_<2, Shape>() &&
                           concept::square_compatible_shape<Shape>() &&
                           memory::concept::memory_backend<MemoryBackend>())>
auto matrix_type() -> NumericArray<Scalar, Shape, Structure, MemoryBackend>;

template <class Scalar, class Structure, class Extent,
          class MemoryBackend = memory::SimdAllocator<Scalar>,
          CONCEPT_REQUIRES(execution_context::concept::scalar<Scalar>() &&
                           execution_context::concept::structure<Structure>() &&
                           k_array::concept::extent<Extent>() &&
                           structure::concept::square<Structure>() &&
                           memory::concept::memory_backend<MemoryBackend>())>
auto matrix_type()
    -> NumericArray<Scalar, Shape<Extent, Extent>, Structure, MemoryBackend>;

template <class Scalar, class Structure, class MemoryBackend,
          CONCEPT_REQUIRES(execution_context::concept::scalar<Scalar>() &&
                           execution_context::concept::structure<Structure>() &&
                           structure::concept::square<Structure>() &&
                           memory::concept::memory_backend<MemoryBackend>())>
auto matrix_type()
    -> NumericArray<Scalar, Shape<index_t, index_t>, Structure, MemoryBackend>;
}

//------------------------------------------------------------------------------
// Matrix
//------------------------------------------------------------------------------
template <class Scalar, class... Specifiers>
using Matrix = decltype(
    DETAIL_NS::matrix_type<Scalar, structure::matrix_general, Specifiers...>());

//------------------------------------------------------------------------------
// StaticMatrix
//------------------------------------------------------------------------------
template <class Scalar, class... Specifiers>
using StaticMatrix =
    Matrix<Scalar, Specifiers..., memory::SimdStaticAllocator<Scalar>>;

//------------------------------------------------------------------------------
// SymmetricMatrix
//------------------------------------------------------------------------------
template <class Scalar, class... Specifiers>
using SymmetricMatrix = decltype(
    DETAIL_NS::matrix_type<Scalar, structure::symmetric, Specifiers...>());

//------------------------------------------------------------------------------
// StaticSymmetricMatrix
//------------------------------------------------------------------------------
template <class Scalar, class... Specifiers>
using StaticSymmetricMatrix =
    SymmetricMatrix<Scalar, Specifiers..., memory::SimdStaticAllocator<Scalar>>;

//------------------------------------------------------------------------------
// HermitianMatrix
//------------------------------------------------------------------------------
template <class Scalar, class... Specifiers>
using HermitianMatrix = decltype(
    DETAIL_NS::matrix_type<Scalar, structure::hermitian, Specifiers...>());

//------------------------------------------------------------------------------
// StaticHermitianMatrix
//------------------------------------------------------------------------------
template <class Scalar, class... Specifiers>
using StaticHermitianMatrix =
    HermitianMatrix<Scalar, Specifiers..., memory::SimdStaticAllocator<Scalar>>;

//------------------------------------------------------------------------------
// LowerTriangularMatrix
//------------------------------------------------------------------------------
template <class Scalar, class... Specifiers>
using LowerTriangularMatrix =
    decltype(DETAIL_NS::matrix_type<Scalar, structure::lower_triangular,
                                    Specifiers...>());

//------------------------------------------------------------------------------
// StaticLowerTriangularMatrix
//------------------------------------------------------------------------------
template <class Scalar, class... Specifiers>
using StaticLowerTriangularMatrix =
    LowerTriangularMatrix<Scalar, Specifiers...,
                          memory::SimdStaticAllocator<Scalar>>;

//------------------------------------------------------------------------------
// UpperTriangularMatrix
//------------------------------------------------------------------------------
template <class Scalar, class... Specifiers>
using UpperTriangularMatrix =
    decltype(DETAIL_NS::matrix_type<Scalar, structure::upper_triangular,
                                    Specifiers...>());

//------------------------------------------------------------------------------
// StaticUpperTriangularMatrix
//------------------------------------------------------------------------------
template <class Scalar, class... Specifiers>
using StaticUpperTriangularMatrix =
    UpperTriangularMatrix<Scalar, Specifiers...,
                          memory::SimdStaticAllocator<Scalar>>;

//------------------------------------------------------------------------------
// make_matrix
//------------------------------------------------------------------------------
template <class Scalar, class Structure, class RowExtent, class ColumnExtent,
          class Allocator = memory::SimdAllocator<Scalar>,
          CONCEPT_REQUIRES(execution_context::concept::scalar<Scalar>() &&
                           execution_context::concept::structure<Structure>() &&
                           k_array::concept::extent<RowExtent>() &&
                           k_array::concept::extent<ColumnExtent>() &&
                           memory::concept::memory_backend<Allocator>())>
auto make_matrix(const Dimensionality<RowExtent, ColumnExtent>& dimensionality,
                 const Allocator& allocator = Allocator()) {
  return make_numeric_array<Scalar, Structure>(dimensionality, allocator);
}

template <class Scalar, class Structure, class RowExtent, class ColumnExtent,
          class Allocator = memory::SimdAllocator<Scalar>,
          CONCEPT_REQUIRES(execution_context::concept::scalar<Scalar>() &&
                           execution_context::concept::structure<Structure>() &&
                           std::is_convertible<RowExtent, index_t>() &&
                           std::is_convertible<ColumnExtent, index_t>() &&
                           memory::concept::memory_backend<Allocator>())>
auto make_matrix(RowExtent num_rows, ColumnExtent num_columns,
                 const Allocator& allocator = Allocator()) {
  return make_numeric_array<Scalar, Structure>(
      make_dimensionality(num_rows, num_columns), allocator);
}

template <class Scalar, class RowExtent, class ColumnExtent,
          class Allocator = memory::SimdAllocator<Scalar>,
          CONCEPT_REQUIRES(execution_context::concept::scalar<Scalar>() &&
                           k_array::concept::extent<RowExtent>() &&
                           k_array::concept::extent<ColumnExtent>() &&
                           memory::concept::memory_backend<Allocator>())>
auto make_matrix(const Dimensionality<RowExtent, ColumnExtent>& dimensionality,
                 const Allocator& allocator = Allocator()) {
  return make_numeric_array<Scalar, structure::matrix_general>(dimensionality,
                                                               allocator);
}

template <class Scalar, class RowExtent, class ColumnExtent,
          class Allocator = memory::SimdAllocator<Scalar>,
          CONCEPT_REQUIRES(execution_context::concept::scalar<Scalar>() &&
                           std::is_convertible<RowExtent, index_t>() &&
                           std::is_convertible<ColumnExtent, index_t>() &&
                           memory::concept::memory_backend<Allocator>())>
auto make_matrix(RowExtent num_rows, ColumnExtent num_columns,
                 const Allocator& allocator = Allocator()) {
  return make_numeric_array<Scalar, structure::matrix_general>(
      make_dimensionality(num_rows, num_columns), allocator);
}

//------------------------------------------------------------------------------
// make_static_matrix
//------------------------------------------------------------------------------
template <class Scalar, class Structure, class RowExtent, class ColumnExtent,
          CONCEPT_REQUIRES(execution_context::concept::scalar<Scalar>() &&
                           execution_context::concept::structure<Structure>() &&
                           k_array::concept::static_extent<RowExtent>() &&
                           k_array::concept::static_extent<ColumnExtent>())>
auto make_static_matrix(
    const Dimensionality<RowExtent, ColumnExtent>& dimensionality) {
  return make_static_numeric_array<Scalar, Structure>(dimensionality);
}

template <class Scalar, class Structure, class RowExtent, class ColumnExtent,
          CONCEPT_REQUIRES(execution_context::concept::scalar<Scalar>() &&
                           execution_context::concept::structure<Structure>() &&
                           k_array::concept::static_extent<RowExtent>() &&
                           k_array::concept::static_extent<ColumnExtent>())>
auto make_static_matrix(RowExtent num_rows, ColumnExtent num_columns) {
  return make_static_numeric_array<Scalar, Structure>(
      make_dimensionality(num_rows, num_columns));
}

template <class Scalar, class RowExtent, class ColumnExtent,
          CONCEPT_REQUIRES(execution_context::concept::scalar<Scalar>() &&
                           k_array::concept::static_extent<RowExtent>() &&
                           k_array::concept::static_extent<ColumnExtent>())>
auto make_static_matrix(
    const Dimensionality<RowExtent, ColumnExtent>& dimensionality) {
  return make_static_numeric_array<Scalar, structure::matrix_general>(
      dimensionality);
}

template <class Scalar, class Structure, class RowExtent, class ColumnExtent,
          CONCEPT_REQUIRES(execution_context::concept::scalar<Scalar>() &&
                           k_array::concept::static_extent<RowExtent>() &&
                           k_array::concept::static_extent<ColumnExtent>())>
auto make_static_matrix(RowExtent num_rows, ColumnExtent num_columns) {
  return make_static_numeric_array<Scalar, structure::matrix_general>(
      make_dimensionality(num_rows, num_columns));
}
}
}

#undef DETAIL_NS
