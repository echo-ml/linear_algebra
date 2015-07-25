#pragma once

#include <echo/linear_algebra/blas_concept.h>
#include <echo/linear_algebra/utility.h>
#include <echo/execution_context.h>

namespace echo {
namespace linear_algebra {

//------------------------------------------------------------------------------
// emplace_left_solve
//------------------------------------------------------------------------------
// trsm
template <class ExecutionContext, class A, class B,
          CONCEPT_REQUIRES(
              execution_context::concept::blas_executer<ExecutionContext>() &&
              blas::concept::trsm<A, uncvref_t<B>>() &&
              linear_algebra::concept::modifiable_matrix_forward<B>())>
auto emplace_left_solve(const ExecutionContext& execution_context, const A& a,
                        numeric_array_traits::value_type<uncvref_t<B>> alpha,
                        B&& b) {
  CONTRACT_EXPECT { CONTRACT_ASSERT(get_extent<0>(a) == get_extent<0>(b)); };
  auto a_m = get_extent<0>(a);
  auto lda = get_leading_dimension(a);

  auto b_n = get_extent<1>(b);
  auto ldb = get_leading_dimension(b);

  constexpr auto operation_a = matrix_traits::operation<A>();
  constexpr auto a_uplo =
      matrix_traits::get_underlying_triangular_matrix_uplo<A>();

  execution_context.trsm(execution_context::matrix_side_t::left, a_uplo,
                         operation_a,
                         structure::matrix_diagonal_fill_t::general, a_m, b_n,
                         alpha, a.const_data(), lda, b.data(), ldb);

  return make_view(b);
}

// trsv
template <class ExecutionContext, class A, class X,
          CONCEPT_REQUIRES(
              execution_context::concept::blas_executer<ExecutionContext>() &&
              blas::concept::trsv<A, uncvref_t<X>>())>
auto emplace_left_solve(const ExecutionContext& execution_context, const A& a,
                        X&& x) {
  CONTRACT_EXPECT { CONTRACT_ASSERT(get_extent<0>(a) == get_extent<0>(x)); };
  auto a_m = get_extent<0>(a);
  auto lda = get_leading_dimension(a);

  auto incx = get_stride<0>(x);

  constexpr auto operation_a = matrix_traits::operation<A>();
  constexpr auto a_uplo =
      matrix_traits::get_underlying_triangular_matrix_uplo<A>();

  execution_context.trsv(a_uplo, operation_a,
                         structure::matrix_diagonal_fill_t::general, a_m,
                         a.const_data(), lda, x.data(), incx);

  return make_view(x);
}

// general
template <class ExecutionContext, class A, class B,
          CONCEPT_REQUIRES(
              execution_context::concept::blas_executer<ExecutionContext>() &&
              blas::concept::left_trsm<A, uncvref_t<B>>() &&
              linear_algebra::concept::modifiable_matrix_forward<B>())>
auto emplace_left_solve(const ExecutionContext& execution_context, const A& a,
                        B&& b) {
  CONTRACT_EXPECT { CONTRACT_ASSERT(get_extent<0>(a) == get_extent<0>(b)); };
  return emplace_left_solve(execution_context, a, 1, b);
}

//------------------------------------------------------------------------------
// left_solve
//------------------------------------------------------------------------------
template <
    class ExecutionContext, class A, class B,
    CONCEPT_REQUIRES(
        execution_context::concept::blas_executer<ExecutionContext>() &&
        execution_context::concept::allocation_backend<ExecutionContext>() &&
        blas::concept::left_trsm<A, B>())>
auto left_solve(const ExecutionContext& execution_context, const A& a,
                numeric_array_traits::value_type<B> alpha, const B& b) {
  CONTRACT_EXPECT { CONTRACT_ASSERT(get_extent<0>(a) == get_extent<0>(b)); };
  using Scalar = numeric_array_traits::value_type<B>;
  using Structure = numeric_array_traits::structure<B>;

  auto result_shape = make_shape(get_extent<0>(b), get_extent<1>(b));
  auto allocator = make_aligned_allocator<Scalar>(execution_context);
  NumericArray<Scalar, decltype(result_shape), Structure, decltype(allocator)>
      result(result_shape, allocator);
  copy(execution_context, b, result);
  emplace_left_solve(execution_context, a, alpha, result);
  return result;
}

template <
    class ExecutionContext, class A, class B,
    CONCEPT_REQUIRES(
        execution_context::concept::blas_executer<ExecutionContext>() &&
        execution_context::concept::allocation_backend<ExecutionContext>() &&
        blas::concept::trsv<A, B>())>
auto left_solve(const ExecutionContext& execution_context, const A& a,
                const B& b) {
  CONTRACT_EXPECT { CONTRACT_ASSERT(get_extent<0>(a) == get_extent<0>(b)); };
  using Scalar = numeric_array_traits::value_type<B>;
  using Structure = numeric_array_traits::structure<B>;

  auto b_shape = b.shape();
  auto allocator = make_aligned_allocator<Scalar>(execution_context);
  NumericArray<Scalar, decltype(b_shape), Structure, decltype(allocator)>
      result(b_shape, allocator);
  copy(execution_context, b, result);
  emplace_left_solve(execution_context, a, result);
  return result;
}

template <
    class ExecutionContext, class A, class B,
    CONCEPT_REQUIRES(
        execution_context::concept::blas_executer<ExecutionContext>() &&
        execution_context::concept::allocation_backend<ExecutionContext>() &&
        blas::concept::left_trsm<A, B>())>
auto left_solve(const ExecutionContext& execution_context, const A& a,
                const B& b) {
  CONTRACT_EXPECT { CONTRACT_ASSERT(get_extent<0>(a) == get_extent<0>(b)); };
  return left_solve(execution_context, a, 1, b);
}
}
}
