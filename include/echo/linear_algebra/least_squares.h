#pragma once

#include <echo/linear_algebra/lapack_concept.h>
#include <echo/linear_algebra/matrix_traits.h>
#include <echo/linear_algebra/utility.h>
#include <echo/execution_context.h>
#include <stdexcept>
#include <boost/optional.hpp>

namespace echo {
namespace linear_algebra {

//------------------------------------------------------------------------------
// emplace_compute_least_squares
//------------------------------------------------------------------------------
template <
    class ExecutionContext, class A, class B,
    CONCEPT_REQUIRES(
        execution_context::concept::lapack_executer<ExecutionContext>() &&
        lapack::concept::overdetermined_gels<uncvref_t<A>, uncvref_t<B>>() &&
        concept::modifiable_weak_matrix_forward<A>() &&
        concept::modifiable_matrix_forward<B>())>
auto emplace_compute_least_squares(const ExecutionContext& execution_context,
                                   A&& a, B&& b) {
  CONTRACT_EXPECT {
    CONTRACT_ASSERT(get_extent<0>(a) >= get_extent<1>(a));
    CONTRACT_ASSERT(get_extent<0>(a) == get_extent<0>(b));
  };

  constexpr auto operation_a = matrix_traits::operation<uncvref_t<A>>();
  auto a_m = get_extent<0>(a);
  auto a_n = get_extent<1>(a);

  auto solution_view =
      make_numeric_subarray(b, slice::counted_range(0, a_n), slice::all);
  boost::optional<decltype(solution_view)> result;

  auto error_code = execution_context.gels(
      operation_a, operation_a == matrix_operation_t::none ? a_m : a_n,
      operation_a == matrix_operation_t::none ? a_n : a_m, get_extent<1>(b),
      a.data(), get_leading_dimension(a), b.data(), get_leading_dimension(b));

  assert(error_code >= 0);

  if (error_code == 0) result = solution_view;

  return result;
}

//------------------------------------------------------------------------------
// LeastSquaresRankError
//------------------------------------------------------------------------------
struct LeastSquaresRankError : virtual std::runtime_error {
  LeastSquaresRankError() : std::runtime_error("LeastSquaresRankError") {}
};

//------------------------------------------------------------------------------
// compute_least_squares
//------------------------------------------------------------------------------
template <
    class ExecutionContext, class A, class B,
    CONCEPT_REQUIRES(
        execution_context::concept::lapack_executer<ExecutionContext>() &&
        execution_context::concept::allocation_backend<ExecutionContext>() &&
        lapack::concept::overdetermined_gels<A, B>())>
auto compute_least_squares(const ExecutionContext& execution_context,
                           const A& a, const B& b) {
  CONTRACT_EXPECT {
    CONTRACT_ASSERT(get_extent<0>(a) >= get_extent<1>(a));
    CONTRACT_ASSERT(get_extent<0>(a) == get_extent<0>(b));
  };

  auto a_shape = make_shape(get_dimensionality(a));
  auto b_shape = make_shape(get_dimensionality(b));

  using Scalar = uncvref_t<decltype(*a.data())>;

  auto allocator = make_aligned_allocator<Scalar>(execution_context);
  NumericArray<Scalar, decltype(a_shape), typename A::structure,
               decltype(allocator)> a_copy(a_shape, allocator);
  NumericArray<Scalar, decltype(b_shape), typename B::structure,
               decltype(allocator)> b_copy(b_shape, allocator);

  copy(execution_context, a, a_copy);
  copy(execution_context, b, b_copy);

  auto solution =
      emplace_compute_least_squares(execution_context, a_copy, b_copy);

  if (!solution) throw LeastSquaresRankError();

  using Solution = uncvref_t<decltype(*solution)>;
  auto result_shape = make_shape(get_dimensionality(*solution));

  NumericArray<Scalar, decltype(result_shape), typename Solution::structure,
               decltype(allocator)> result(result_shape, allocator);

  copy(execution_context, *solution, result);

  return result;
}
}
}
