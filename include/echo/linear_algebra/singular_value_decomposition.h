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
// SingularValueDecompositionError
//------------------------------------------------------------------------------
struct SingularValueDecompositionError : virtual std::runtime_error {
  SingularValueDecompositionError()
      : std::runtime_error("SingularValueDecompositionError") {}
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
  auto smallest_extent = echo::min(m, n);

  auto allocator = make_aligned_allocator<Scalar>(execution_context);

  auto s = make_vector<Scalar>(smallest_extent, allocator);
  auto vt = make_matrix<Scalar>(smallest_extent, n, allocator);
  auto superb = make_vector<Scalar>(echo::min(m, n) - 1_index, allocator);

  auto return_code = execution_context.gesvd(
      execution_context::result_output_mode_t::overwrite,
      execution_context::result_output_mode_t::subset, m, n, a.data(),
      get_leading_dimension(a), s.data(), nullptr, index_t(1), vt.data(),
      get_leading_dimension(vt), superb.data());
  if (return_code != 0) throw SingularValueDecompositionError();
  auto u_view = make_numeric_subarray(a, slice::all,
                                      slice::counted_range(0, smallest_extent));
  return std::make_tuple(u_view, std::move(s), std::move(vt));
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
  auto decomposition =
      emplace_singular_value_decompose(execution_context, a_copy);
  return std::make_tuple(
      make_numeric_array(execution_context, std::get<0>(decomposition)),
      std::move(std::get<1>(decomposition)),
      std::move(std::get<2>(decomposition)));
}
}
}
