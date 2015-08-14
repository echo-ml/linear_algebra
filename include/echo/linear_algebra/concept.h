#pragma once

#define DETAIL_NS detail_concept

#include <echo/linear_algebra/structure.h>
#include <echo/linear_algebra/matrix_traits.h>
#include <echo/linear_algebra/matrix_operation_expression_fwd.h>
#include <echo/numeric_array.h>

namespace echo {
namespace linear_algebra {
namespace concept {

//------------------------------------------------------------------------------
// square_compatible_shape
//------------------------------------------------------------------------------
namespace DETAIL_NS {
struct SquareCompatibleShape : Concept {
  template <class T>
  auto require(T&& shape) -> list<
      k_array::concept::shape_<2, T>(),
      same<decltype(get_extent<0>(shape)), decltype(get_extent<1>(shape))>()>;
};
}

template <class T>
constexpr bool square_compatible_shape() {
  return models<DETAIL_NS::SquareCompatibleShape, T>();
}

//------------------------------------------------------------------------------
// matrix
//------------------------------------------------------------------------------
namespace DETAIL_NS {
struct Matrix : Concept {
  template <class T>
  auto require(T&& x)
      -> list<k_array::concept::shape_<2, uncvref_t<decltype(x.shape())>>(),
              numeric_array::concept::numeric_array<T>(),
              matrix_traits::operation<T>() ==
                  execution_context::matrix_operation_t::none>;
};
}

template <class T>
constexpr bool matrix() {
  return models<DETAIL_NS::Matrix, T>();
}

//------------------------------------------------------------------------------
// square_compatible_matrix
//------------------------------------------------------------------------------
namespace DETAIL_NS {
struct SquareCompatibleMatrix : Concept {
  template <class T>
  auto require(T&& x)
      -> list<matrix<T>(), square_compatible_shape<uncvref_t<decltype(x.shape())>>()>;
};
}

template <class T>
constexpr bool square_compatible_matrix() {
  return models<DETAIL_NS::SquareCompatibleMatrix, T>();
}

//------------------------------------------------------------------------------
// modifiable_matrix_forward
//------------------------------------------------------------------------------
namespace DETAIL_NS {
struct ModifiableMatrixForward : Concept {
  template <class T>
  auto require(T&& x) -> list<matrix<uncvref_t<T>>(),
                              echo::concept::writable<decltype(x.data())>()>;
};
}

template <class T>
constexpr bool modifiable_matrix_forward() {
  return models<DETAIL_NS::ModifiableMatrixForward, T>();
}

//------------------------------------------------------------------------------
// operated_matrix
//------------------------------------------------------------------------------
namespace DETAIL_NS {
template <class Pointer, class Dimensionality, class Structure,
          matrix_operation_t Operation>
auto operated_matrix_impl(
    MatrixOperationExpression<Pointer, Dimensionality, Structure, Operation> && )
    -> std::true_type;

template <class T>
auto operated_matrix_impl(T && ) -> std::false_type;
}

template <class T>
constexpr bool operated_matrix() {
  using Result = decltype(DETAIL_NS::operated_matrix_impl(std::declval<T>()));
  return Result::value;
}

//------------------------------------------------------------------------------
// weak_matrix
//------------------------------------------------------------------------------
template <class T>
constexpr bool weak_matrix() {
  return matrix<T>() || operated_matrix<T>();
}

namespace DETAIL_NS {
template <class Structure>
struct WeakStructuredMatrix : Concept {
  template <class T>
  auto require(T&& x) -> list<weak_matrix<T>(),
                              convertible<typename T::structure, Structure>()>;
};
}

template <class Structure, class T>
constexpr bool weak_matrix() {
  return models<DETAIL_NS::WeakStructuredMatrix<Structure>, T>();
}

//------------------------------------------------------------------------------
// modifiable_weak_matrix_forward
//------------------------------------------------------------------------------
namespace DETAIL_NS {
struct ModifiableWeakMatrixForward : Concept {
  template <class T>
  auto require(T&& x) -> list<weak_matrix<uncvref_t<T>>(),
                              echo::concept::writable<decltype(x.data())>()>;
};
}

template <class T>
constexpr bool modifiable_weak_matrix_forward() {
  return models<DETAIL_NS::ModifiableWeakMatrixForward, T>();
}

//------------------------------------------------------------------------------
// vector
//------------------------------------------------------------------------------
template <class T>
constexpr bool vector() {
  return numeric_array::concept::numeric_array_<1, T>();
}

//------------------------------------------------------------------------------
// modifiable_vector_forward
//------------------------------------------------------------------------------
template<class T>
constexpr bool modifiable_vector_forward() {
  return vector<uncvref_t<T>>() &&
         numeric_array::concept::modifiable_numeric_array_forward<T>();
}

//------------------------------------------------------------------------------
// row_vector
//------------------------------------------------------------------------------
namespace DETAIL_NS {
struct RowVector : Concept {
  template <class T>
  auto require(T&& x)
      -> list<matrix<T>(), same<decltype(get_extent<0>(x)), StaticIndex<1>>()>;
};
}

template <class T>
constexpr bool row_vector() {
  return models<DETAIL_NS::RowVector, T>();
}

//------------------------------------------------------------------------------
// modifiable_row_vector_forward
//------------------------------------------------------------------------------
template <class T>
constexpr bool modifiable_row_vector_forward() {
  return row_vector<uncvref_t<T>>() && modifiable_matrix_forward<T>();
}

//------------------------------------------------------------------------------
// column_vector
//------------------------------------------------------------------------------
namespace DETAIL_NS {
struct ColumnVector : Concept {
  template <class T>
  auto require(T&& x)
      -> list<matrix<T>(), same<decltype(get_extent<1>(x)), StaticIndex<1>>()>;
};
}

template <class T>
constexpr bool column_vector() {
  return models<DETAIL_NS::ColumnVector, T>();
}

//------------------------------------------------------------------------------
// modifiable_column_vector_forward
//------------------------------------------------------------------------------
template <class T>
constexpr bool modifiable_column_vector_forward() {
  return column_vector<uncvref_t<T>>() && modifiable_matrix_forward<T>();
}

//------------------------------------------------------------------------------
// diagonal_matrix
//------------------------------------------------------------------------------
template <class T>
constexpr bool diagonal_matrix() {
  return vector<T>() &&
         numeric_array::concept::numeric_array<structure::diagonal, T>();
}

//------------------------------------------------------------------------------
// contiguous_matrix
//------------------------------------------------------------------------------
template <class T>
constexpr bool contiguous_matrix() {
  return matrix<T>() && numeric_array::concept::contiguous_numeric_array<T>();
}

//------------------------------------------------------------------------------
// general_matrix
//------------------------------------------------------------------------------
template <class T>
constexpr bool general_matrix() {
  return numeric_array::concept::numeric_array<structure::matrix_general,
                                               T>() &&
         matrix<T>();
}

//------------------------------------------------------------------------------
// weak_general_matrix
//------------------------------------------------------------------------------
template <class T>
constexpr bool weak_general_matrix() {
  return weak_matrix<structure::matrix_general, T>();
}

//------------------------------------------------------------------------------
// symmetric_matrix
//------------------------------------------------------------------------------
namespace DETAIL_NS {
struct SymmetricMatrix : Concept {
  template <class T>
  auto require(T&& x) -> list<
      matrix<T>(), structure::concept::symmetric<typename T::structure>()>;
};
}

template <class T>
constexpr bool symmetric_matrix() {
  return models<DETAIL_NS::SymmetricMatrix, T>();
}

//------------------------------------------------------------------------------
// hermitian_matrix
//------------------------------------------------------------------------------
namespace DETAIL_NS {
struct StrictHermitianMatrix : Concept {
  template <class T>
  auto require(T&& a) -> list<
      matrix<T>(), structure::concept::hermitian<typename T::structure>()>;
};
}

template <class T>
constexpr bool hermitian_matrix() {
  return models<DETAIL_NS::StrictHermitianMatrix, T>() ||
         (numeric_array::concept::real_numeric_array<T>() &&
          symmetric_matrix<T>());
}

//------------------------------------------------------------------------------
// triangular_matrix
//------------------------------------------------------------------------------
namespace DETAIL_NS {
struct TriangularMatrix : Concept {
  template <class T>
  auto require(T&& x) -> list<
      matrix<T>(), structure::concept::triangular<typename T::structure>()>;
};
}

template <class T>
constexpr bool triangular_matrix() {
  return models<DETAIL_NS::TriangularMatrix, T>();
}

//------------------------------------------------------------------------------
// weak_triangular_matrix
//------------------------------------------------------------------------------
namespace DETAIL_NS {
struct WeakTriangularMatrix : Concept {
  template <class T>
  auto require(T&& x)
      -> list<weak_matrix<T>(),
              structure::concept::triangular<typename T::structure>()>;
};
}

template <class T>
constexpr bool weak_triangular_matrix() {
  return models<DETAIL_NS::WeakTriangularMatrix, T>();
}

//------------------------------------------------------------------------------
// lower_triangular_matrix
//------------------------------------------------------------------------------
template <class T>
constexpr bool lower_triangular_matrix() {
  return numeric_array::concept::numeric_array<structure::lower_triangular,
                                               T>() &&
         matrix<T>();
}

//------------------------------------------------------------------------------
// weak_lower_triangular_matrix
//------------------------------------------------------------------------------
template <class T>
constexpr bool weak_lower_triangular_matrix() {
  return weak_matrix<structure::lower_triangular, T>();
}

//------------------------------------------------------------------------------
// upper_triangular_matrix
//------------------------------------------------------------------------------
template <class T>
constexpr bool upper_triangular_matrix() {
  return numeric_array::concept::numeric_array<structure::upper_triangular,
                                               T>() &&
         matrix<T>();
}

//------------------------------------------------------------------------------
// weak_upper_triangular_matrix
//------------------------------------------------------------------------------
template <class T>
constexpr bool weak_upper_triangular_matrix() {
  return weak_matrix<structure::upper_triangular, T>();
}

//------------------------------------------------------------------------------
// matrix_expression
//------------------------------------------------------------------------------
template <class T>
constexpr bool matrix_expression() {
  return numeric_array::concept::dimensioned_expression<2, T>();
}

//------------------------------------------------------------------------------
// standard_numeric_valued
//------------------------------------------------------------------------------
namespace DETAIL_NS {
struct StandardNumericValued : Concept {
  template <class T>
  auto require(T&& a)
      -> list<execution_context::concept::standard_numeric_scalar<
          uncvref_t<decltype(*a.data())>>()>;
};
}

template <class T>
constexpr bool standard_numeric_valued() {
  return models<DETAIL_NS::StandardNumericValued, T>();
}

//------------------------------------------------------------------------------
// like_valued
//------------------------------------------------------------------------------
namespace DETAIL_NS {
struct LikeValued : Concept {
  template <class AFirst, class... ARest>
  auto require(AFirst&& a_first, ARest&&... a_rest)
      -> list<and_c<same<uncvref_t<decltype(a_first.data())>,
                         uncvref_t<decltype(a_rest.data())>>()...>()>;
};
}

template <class... AX>
constexpr bool like_valued() {
  return models<DETAIL_NS::LikeValued, AX...>();
}
}
}
}

#undef DETAIL_NS
