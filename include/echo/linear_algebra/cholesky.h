#pragma once

#include <echo/linear_algebra/lapack_concept.h>
#include <echo/linear_algebra/matrix_traits.h>
#include <echo/linear_algebra/utility.h>
#include <echo/execution_context.h>
#include <boost/optional.hpp>

namespace echo {
namespace linear_algebra {

//------------------------------------------------------------------------------
// emplace_cholesky_factorize
//------------------------------------------------------------------------------
template <class ExecutionContext, class A,
          CONCEPT_REQUIRES(
              execution_context::concept::lapack_executer<ExecutionContext>() &&
              lapack::concept::potrf<uncvref_t<A>>() &&
              concept::modifiable_matrix_forward<A>())>
auto emplace_cholesky_factorize(const ExecutionContext& execution_context,
                                A&& a) {
  auto n = get_extent<0>(a);
  constexpr auto storage_uplo =
      numeric_array_traits::structure<uncvref_t<A>>::storage_uplo;
  using ResultView = decltype(make_numeric_array_view<
      structure::triangular<storage_uplo>,
      numeric_array_traits::memory_backend_tag<uncvref_t<A>>>(a.data(),
                                                              a.shape()));
  auto lda = get_leading_dimension(a);
  boost::optional<ResultView> result;
  auto was_successful = execution_context.potrf(storage_uplo, n, a.data(), lda);
  if (was_successful)
    result = make_numeric_array_view<
        structure::triangular<storage_uplo>,
        numeric_array_traits::memory_backend_tag<uncvref_t<A>>>(a.data(),
                                                                a.shape());
  return result;
}

template <class ExecutionContext, class A, class B,
          CONCEPT_REQUIRES(
              execution_context::concept::lapack_executer<ExecutionContext>() &&
              lapack::concept::potrf<A, uncvref_t<B>>() &&
              concept::modifiable_matrix_forward<B>())>
auto emplace_cholesky_factorize(const ExecutionContext& execution_context,
                                numeric_array_traits::value_type<A> alpha,
                                const A& a, B&& b) {
  CONTRACT_EXPECT { CONTRACT_ASSERT(get_extent<0>(a) == get_extent<0>(b)); };
  constexpr auto a_storage_uplo =
      numeric_array_traits::structure<A>::storage_uplo;
  auto a_hermitian_view =
      make_numeric_array_view<structure::hermitian_uplo<a_storage_uplo>,
                              numeric_array_traits::memory_backend_tag<A>>(
          a.data(), a.shape());
  constexpr auto b_uplo = numeric_array_traits::structure<uncvref_t<B>>::uplo;
  auto b_hermitian_view = make_numeric_array_view<
      structure::hermitian_uplo<b_uplo>,
      numeric_array_traits::memory_backend_tag<uncvref_t<B>>>(b.data(),
                                                              b.shape());
  execution_context(execution_mode::simd | execution_mode::parallel_coarse |
                        execution_mode::nontemporal,
                    b_hermitian_view = alpha * a_hermitian_view);
  return emplace_cholesky_factorize(execution_context, b_hermitian_view);
}

template <class ExecutionContext, class A, class B,
          CONCEPT_REQUIRES(
              execution_context::concept::lapack_executer<ExecutionContext>() &&
              lapack::concept::potrf<A, uncvref_t<B>>() &&
              concept::modifiable_matrix_forward<B>())>
auto emplace_cholesky_factorize(const ExecutionContext& execution_context,
                                const A& a, B&& b) {
  CONTRACT_EXPECT { CONTRACT_ASSERT(get_extent<0>(a) == get_extent<0>(b)); };
  return emplace_cholesky_factorize(execution_context, 1, a, b);
}
}
}
