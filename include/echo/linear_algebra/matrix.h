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

template <class Scalar, class Structure,
          class MemoryBackend = memory::SimdAllocator<Scalar>,
          CONCEPT_REQUIRES(execution_context::concept::scalar<Scalar>() &&
                           execution_context::concept::structure<Structure>() &&
                           memory::concept::memory_backend<MemoryBackend>())>
auto matrix_type()
    -> NumericArray<Scalar, Shape<index_t, index_t>, Structure, MemoryBackend>;

template <class Scalar, class Structure, class Shape,
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
}

//------------------------------------------------------------------------------
// Matrix
//------------------------------------------------------------------------------
template <class Scalar, class... Specifiers>
using Matrix = decltype(
    DETAIL_NS::matrix_type<Scalar, structure::matrix_general, Specifiers...>());

//------------------------------------------------------------------------------
// SymmetricMatrix
//------------------------------------------------------------------------------
template <class Scalar, class... Specifiers>
using SymmetricMatrix = decltype(
    DETAIL_NS::matrix_type<Scalar, structure::symmetric, Specifiers...>());

//------------------------------------------------------------------------------
// HermitianMatrix
//------------------------------------------------------------------------------
template <class Scalar, class... Specifiers>
using HermitianMatrix = decltype(
    DETAIL_NS::matrix_type<Scalar, structure::hermitian, Specifiers...>());

//------------------------------------------------------------------------------
// LowerTriangularMatrix
//------------------------------------------------------------------------------
template <class Scalar, class... Specifiers>
using LowerTriangularMatrix =
    decltype(DETAIL_NS::matrix_type<Scalar, structure::lower_triangular,
                                    Specifiers...>());

//------------------------------------------------------------------------------
// UpperTriangularMatrix
//------------------------------------------------------------------------------
template <class Scalar, class... Specifiers>
using UpperTriangularMatrix =
    decltype(DETAIL_NS::matrix_type<Scalar, structure::upper_triangular,
                                    Specifiers...>());
}
}

#undef DETAIL_NS
