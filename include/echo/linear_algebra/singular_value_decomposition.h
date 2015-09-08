#pragma once

#include <echo/linear_algebra/concept.h>
#include <echo/linear_algebra/matrix.h>
#include <echo/linear_algebra/vector.h>
#include <echo/linear_algebra/utility.h>
#include <echo/execution_context.h>
#include <echo/adl.h>
#include <tuple>

namespace echo {
namespace linear_algebra {

//------------------------------------------------------------------------------
// SingularValueDecompositionFailure
//------------------------------------------------------------------------------
struct SingularValueDecompositionFailure : virtual std::runtime_error {
  SingularValueDecompositionFailure()
      : std::runtime_error("SingularValueDecompositionFailure") {}
};

//------------------------------------------------------------------------------
// emplace_singular_value_decompose
//------------------------------------------------------------------------------
template <
    class ExecutionContext, class A,
    CONCEPT_REQUIRES(
        execution_context::concept::allocation_backend<ExecutionContext>() &&
        execution_context::concept::lapack_executer<ExecutionContext>() &&
        concept::general_matrix<uncvref_t<A>>() &&
        concept::modifiable_matrix_forward<A>())>
auto emplace_singular_value_decompose(const ExecutionContext& execution_context,
                                      A&& a) {
  using Scalar = numeric_array_traits::value_type<uncvref_t<A>>;
  auto m = get_num_rows(a);
  auto n = get_num_columns(a);

  auto allocator = make_aligned_allocator<Scalar>(execution_context);

  auto u = make_matrix<Scalar>(m, m, allocator);
  auto s = make_vector<Scalar>(echo::min(m, n), allocator);
  auto vt = make_matrix<Scalar>(n, n, allocator);
  auto superb = make_vector<Scalar>(echo::min(m, n) - 1_index, allocator);

  auto return_code = execution_context.gesvd(
      execution_context::result_output_mode_t::all,
      execution_context::result_output_mode_t::all, m, n, a.data(),
      get_leading_dimension(a), s.data(), u.data(), get_leading_dimension(u),
      vt.data(), get_leading_dimension(vt), superb.data());
  if (return_code != 0) throw SingularValueDecompositionFailure();
  return std::make_tuple(std::move(u), std::move(s), std::move(vt));
}

//------------------------------------------------------------------------------
// singular_value_decompose
//------------------------------------------------------------------------------
template <
    class ExecutionContext, class A,
    CONCEPT_REQUIRES(
        execution_context::concept::allocation_backend<ExecutionContext>() &&
        execution_context::concept::lapack_executer<ExecutionContext>() &&
        concept::general_matrix<A>())>
auto singular_value_decompose(const ExecutionContext& execution_context,
                              const A& a) {
  auto a_copy = make_numeric_array(execution_context, a);
  return emplace_singular_value_decompose(execution_context, a_copy);
}
}
}
