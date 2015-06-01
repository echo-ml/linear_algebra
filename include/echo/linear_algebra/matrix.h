#pragma once

#include <echo/linear_algebra/concept.h>
#include <echo/linear_algebra/structure.h>
#include <echo/linear_algebra/matrix_accessor.h>
#include <echo/linear_algebra/matrix_initializer.h>
#include <echo/execution_context.h>
#include <echo/numeric_array.h>

namespace echo {
namespace linear_algebra {

/////////////////
// matrix_type //
/////////////////

namespace detail {
namespace matrix {

template <class Scalar, class Structure,
          class Shape = KShape<Dimension::kDynamic, Dimension::kDynamic>,
          class MemoryBackend = SimdAllocator<Scalar>,
          CONCEPT_REQUIRES(execution_context::concept::scalar<Scalar>() &&
                           execution_context::concept::structure<Structure>() &&
                           !structure::concept::square<Structure>() &&
                           k_array::concept::contiguous_k_shape<2, Shape>() &&
                           memory::concept::memory_backend<MemoryBackend>())>
auto matrix_type() -> NumericArray<Scalar, Shape, Structure, MemoryBackend>;

template <class Scalar, class Structure, class Extent1, class Extent2,
          class MemoryBackend = SimdAllocator<Scalar>,
          CONCEPT_REQUIRES(execution_context::concept::scalar<Scalar>() &&
                           execution_context::concept::structure<Structure>() &&
                           !structure::concept::square<Structure>() &&
                           k_array::concept::extent<Extent1>() &&
                           k_array::concept::extent<Extent2>() &&
                           memory::concept::memory_backend<MemoryBackend>())>
auto matrix_type() -> NumericArray<Scalar, KShapeFromExtents<Extent1, Extent2>,
                                   Structure, MemoryBackend>;

template <class Scalar, class Structure,
          class MemoryBackend = SimdAllocator<Scalar>,
          CONCEPT_REQUIRES(execution_context::concept::scalar<Scalar>() &&
                           execution_context::concept::structure<Structure>() &&
                           memory::concept::memory_backend<MemoryBackend>())>
auto matrix_type()
    -> NumericArray<Scalar, KShape<Dimension::kDynamic, Dimension::kDynamic>,
                    Structure, MemoryBackend>;

template <class Scalar, class Structure, class Shape,
          class MemoryBackend = SimdAllocator<Scalar>,
          CONCEPT_REQUIRES(execution_context::concept::scalar<Scalar>() &&
                           structure::concept::square<Structure>() &&
                           k_array::concept::contiguous_k_shape<2, Shape>() &&
                           concept::square_matrix_shape<Shape>() &&
                           memory::concept::memory_backend<MemoryBackend>())>
auto matrix_type() -> NumericArray<Scalar, Shape, Structure, MemoryBackend>;

template <class Scalar, class Structure, class Extent,
          class MemoryBackend = SimdAllocator<Scalar>,
          CONCEPT_REQUIRES(execution_context::concept::scalar<Scalar>() &&
                           execution_context::concept::structure<Structure>() &&
                           k_array::concept::extent<Extent>() &&
                           structure::concept::square<Structure>() &&
                           memory::concept::memory_backend<MemoryBackend>())>
auto matrix_type() -> NumericArray<Scalar, KShapeFromExtents<Extent, Extent>,
                                   Structure, MemoryBackend>;
}
}

////////////
// Matrix //
////////////

template <class Scalar, class... Specifiers>
using Matrix =
    decltype(detail::matrix::matrix_type<Scalar, structure::matrix_general,
                                         Specifiers...>());

/////////////////////
// SymmetricMatrix //
/////////////////////

template <class Scalar, class... Specifiers>
using SymmetricMatrix = decltype(
    detail::matrix::matrix_type<Scalar, structure::symmetric, Specifiers...>());

/////////////////////
// HermitianMatrix //
/////////////////////

template <class Scalar, class... Specifiers>
using HermitianMatrix = decltype(
    detail::matrix::matrix_type<Scalar, structure::hermitian, Specifiers...>());

///////////////////////////
// LowerTriangularMatrix //
///////////////////////////

template <class Scalar, class... Specifiers>
using LowerTriangularMatrix =
    decltype(detail::matrix::matrix_type<Scalar, structure::lower_triangular,
                                         Specifiers...>());

///////////////////////////
// UpperTriangularMatrix //
///////////////////////////

template <class Scalar, class... Specifiers>
using UpperTriangularMatrix =
    decltype(detail::matrix::matrix_type<Scalar, structure::upper_triangular,
                                         Specifiers...>());
}
}
