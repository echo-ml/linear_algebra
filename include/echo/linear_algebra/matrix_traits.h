#pragma once

#include <echo/linear_algebra/concept.h>
#include <echo/execution_context.h>

namespace echo {
namespace matrix_traits {

template <class Matrix,
          CONCEPT_REQUIRES(linear_algebra::concept::matrix<Matrix>())>
constexpr execution_context::matrix_operation_t operation() {
  return execution_context::matrix_operation_t::none;
}

template <class Matrix,
          CONCEPT_REQUIRES(linear_algebra::concept::operated_matrix<Matrix>())>
constexpr execution_context::matrix_operation_t operation() {
  return Matrix::operation;
}
}
}
