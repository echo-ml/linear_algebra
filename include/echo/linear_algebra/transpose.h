#pragma once

#define DETAIL_NS detail_transpose

#include <echo/linear_algebra/matrix_operation_expression.h>
#include <echo/linear_algebra/concept.h>

namespace echo {
namespace linear_algebra {

//------------------------------------------------------------------------------
// transpose_shape
//------------------------------------------------------------------------------
namespace DETAIL_NS {
template<class Shape, CONCEPT_REQUIRES(k_array::concept::shape<2, Shape>())>
auto transpose_shape(const Shape& shape) {
  return make_subshape(
    make_dimensionality(get_extent<1>(shape), get_extent<0>(shape)),
    make_strides(get_stride<1>(shape), get_stride<0>(shape))
  );
}
}

//------------------------------------------------------------------------------
// transpose
//------------------------------------------------------------------------------
template <class Matrix,
          CONCEPT_REQUIRES(concept::matrix<uncvref_t<Matrix>>() &&
                           !concept::symmetric_matrix<uncvref_t<Matrix>>())>
auto transpose(Matrix&& matrix) {
  return make_matrix_operation_expression<
      numeric_array_traits::structure<Matrix>, matrix_operation_t::transpose>(
      matrix.data(), DETAIL_NS::transpose_shape(matrix.shape()));
}

template <class Matrix,
          CONCEPT_REQUIRES(concept::symmetric_matrix<uncvref_t<Matrix>>())>
auto transpose(Matrix&& matrix) -> decltype(std::forward<Matrix>(matrix)) {
  return std::forward<Matrix>(matrix);
}

template <class Expression,
          CONCEPT_REQUIRES(concept::matrix_expression<Expression>())>
auto transpose(const Expression& expression) {
  return numeric_array::make_numeric_array_expression<
      structure::transpose<expression_traits::structure<Expression>>>(
      expression.dimensionality(),
      make_operation_evaluator<matrix_operation_t::transpose>(
          expression.evaluator()));
}
}
}

#undef DETAIL_NS
