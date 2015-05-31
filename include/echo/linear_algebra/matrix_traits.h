#pragma once

#include <echo/linear_algebra/concept.h>
#include <echo/execution_context.h>

namespace echo {
namespace matrix_traits {

namespace detail {
namespace matrix_traits {
struct HasOperation : Concept {
  template <class T>
  auto require(T&& x) -> list<
      same<execution_context::matrix_operation_t, uncvref_t<decltype(T::operation)>>()>;
};

template <class T>
constexpr bool has_operation() {
  return models<HasOperation, T>();
}
}
}

template <class Matrix,
          CONCEPT_REQUIRES(!detail::matrix_traits::has_operation<Matrix>())>
constexpr execution_context::matrix_operation_t operation() {
  return execution_context::matrix_operation_t::none;
}

template <class Matrix,
          CONCEPT_REQUIRES(detail::matrix_traits::has_operation<Matrix>())>
constexpr execution_context::matrix_operation_t operation() {
  return Matrix::operation;
}
}
}
