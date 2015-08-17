#pragma once

#include <echo/linear_algebra/blas_concept.h>
#include <echo/linear_algebra/matrix_traits.h>
#include <echo/linear_algebra/utility.h>
#include <echo/linear_algebra/matrix.h>
#include <echo/execution_context.h>
#include <echo/contract.h>
#include <cassert>

namespace echo {
namespace linear_algebra {

//------------------------------------------------------------------------------
// emplace_product
//------------------------------------------------------------------------------
// gemm
template <class ExecutionContext, class A, class B, class C,
          CONCEPT_REQUIRES(
              execution_context::concept::blas_executer<ExecutionContext>() &&
              blas::concept::gemm<A, B, uncvref_t<C>>() &&
              linear_algebra::concept::modifiable_matrix_forward<C>())>
auto emplace_product(const ExecutionContext& execution_context,
                     numeric_array_traits::value_type<A> alpha, const A& a,
                     const B& b, numeric_array_traits::value_type<A> beta,
                     C&& c) {
  CONTRACT_EXPECT { CONTRACT_ASSERT(is_product_shaped(a, b, c)); };
  auto a_n = get_extent<1>(a);
  auto lda = get_leading_dimension(a);

  auto ldb = get_leading_dimension(b);

  auto c_m = get_extent<0>(c);
  auto c_n = get_extent<1>(c);
  auto ldc = get_leading_dimension(c);

  execution_context.gemm(matrix_traits::operation<A>(),
                         matrix_traits::operation<B>(), c_m, c_n, a_n, alpha,
                         a.const_data(), lda, b.const_data(), ldb, beta,
                         c.data(), ldc);
  return make_view(c);
}

// gemv
template <class ExecutionContext, class A, class X, class Y,
          CONCEPT_REQUIRES(
              execution_context::concept::blas_executer<ExecutionContext>() &&
              blas::concept::gemv<A, X, uncvref_t<Y>>() &&
              linear_algebra::concept::modifiable_matrix_forward<Y>())>
auto emplace_product(const ExecutionContext& execution_context,
                     numeric_array_traits::value_type<A> alpha, const A& a,
                     const X& x, numeric_array_traits::value_type<A> beta,
                     Y&& y) {
  CONTRACT_EXPECT { CONTRACT_ASSERT(is_product_shaped(a, x, y)); };
  auto a_m = get_extent<0>(a);
  auto a_n = get_extent<1>(a);
  auto lda = get_leading_dimension(a);

  auto stride_x = get_stride<0>(x);

  auto stride_y = get_stride<0>(y);

  execution_context.gemv(matrix_traits::operation<A>(), a_m, a_n, alpha,
                         a.const_data(), lda, x.const_data(), stride_x, beta,
                         y.data(), stride_y);
  return make_view(y);
}

// symm
template <class ExecutionContext, class A, class B, class C,
          CONCEPT_REQUIRES(
              execution_context::concept::blas_executer<ExecutionContext>() &&
              blas::concept::left_symm<A, B, uncvref_t<C>>() &&
              linear_algebra::concept::modifiable_matrix_forward<C>())>
auto emplace_product(const ExecutionContext& execution_context,
                     numeric_array_traits::value_type<A> alpha, const A& a,
                     const B& b, numeric_array_traits::value_type<A> beta,
                     C&& c) {
  CONTRACT_EXPECT { CONTRACT_ASSERT(is_product_shaped(a, b, c)); };
  auto a_m = get_extent<0>(a);
  auto lda = get_leading_dimension(a);

  auto b_n = get_extent<1>(b);
  auto ldb = get_leading_dimension(b);

  auto ldc = get_leading_dimension(c);

  execution_context.symm(execution_context::matrix_side_t::left,
                         A::structure::storage_uplo, a_m, b_n, alpha,
                         a.const_data(), lda, b.const_data(), ldb, beta,
                         c.data(), ldc);
  return make_view(c);
}

template <class ExecutionContext, class A, class B, class C,
          CONCEPT_REQUIRES(
              execution_context::concept::blas_executer<ExecutionContext>() &&
              blas::concept::right_symm<A, B, uncvref_t<C>>() &&
              linear_algebra::concept::modifiable_matrix_forward<C>())>
auto emplace_product(const ExecutionContext& execution_context,
                     numeric_array_traits::value_type<A> alpha, const A& a,
                     const B& b, numeric_array_traits::value_type<A> beta,
                     C&& c) {
  CONTRACT_EXPECT { CONTRACT_ASSERT(is_product_shaped(a, b, c)); };
  auto a_m = get_extent<0>(a);
  auto lda = get_leading_dimension(a);

  auto b_n = get_extent<1>(b);
  auto ldb = get_leading_dimension(b);

  auto ldc = get_leading_dimension(c);

  execution_context.symm(execution_context::matrix_side_t::right,
                         B::structure::storage_uplo, a_m, b_n, alpha,
                         b.const_data(), ldb, a.const_data(), lda, beta,
                         c.data(), ldc);
  return make_view(c);
}

// symv
template <class ExecutionContext, class A, class X, class Y,
          CONCEPT_REQUIRES(
              execution_context::concept::blas_executer<ExecutionContext>() &&
              blas::concept::symv<A, X, uncvref_t<Y>>() &&
              linear_algebra::concept::modifiable_matrix_forward<Y>())>
auto emplace_product(const ExecutionContext& execution_context,
                     numeric_array_traits::value_type<A> alpha, const A& a,
                     const X& x, numeric_array_traits::value_type<A> beta,
                     Y&& y) {
  CONTRACT_EXPECT { CONTRACT_ASSERT(is_product_shaped(a, x, y)); };
  auto a_m = get_extent<0>(a);
  auto lda = get_leading_dimension(a);

  auto incx = get_stride<0>(x);

  auto incy = get_stride<0>(y);

  execution_context.symv(A::structure::storage_uplo, a_m, alpha, a.const_data(),
                         lda, x.const_data(), incx, beta, y.data(), incy);
  return make_view(y);
}

template <class ExecutionContext, class A, class B, class C,
          CONCEPT_REQUIRES(
              execution_context::concept::blas_executer<ExecutionContext>() &&
              blas::concept::product<A, B, uncvref_t<C>>() &&
              linear_algebra::concept::modifiable_matrix_forward<C>())>
auto emplace_product(const ExecutionContext& execution_context,
                     numeric_array_traits::value_type<A> alpha, const A& a,
                     const B& b, C&& c) {
  CONTRACT_EXPECT { CONTRACT_ASSERT(is_product_shaped(a, b, c)); };
  return emplace_product(execution_context, alpha, a, b, 0, c);
}

template <class ExecutionContext, class A, class B, class C,
          CONCEPT_REQUIRES(
              execution_context::concept::blas_executer<ExecutionContext>() &&
              blas::concept::product<A, B, uncvref_t<C>>() &&
              linear_algebra::concept::modifiable_matrix_forward<C>())>
auto emplace_product(const ExecutionContext& execution_context, const A& a,
                     const B& b, numeric_array_traits::value_type<A> beta,
                     C&& c) {
  CONTRACT_EXPECT { CONTRACT_ASSERT(is_product_shaped(a, b, c)); };
  return emplace_product(execution_context, 1, a, b, beta, c);
}

template <class ExecutionContext, class A, class B, class C,
          CONCEPT_REQUIRES(
              execution_context::concept::blas_executer<ExecutionContext>() &&
              blas::concept::product<A, B, uncvref_t<C>>() &&
              linear_algebra::concept::modifiable_matrix_forward<C>())>
auto emplace_product(const ExecutionContext& execution_context, const A& a,
                     const B& b, C&& c) {
  CONTRACT_EXPECT { CONTRACT_ASSERT(is_product_shaped(a, b, c)); };
  return emplace_product(execution_context, 1, a, b, 0, c);
}

//------------------------------------------------------------------------------
// product
//------------------------------------------------------------------------------
template <
    class ExecutionContext, class A, class B, class C,
    CONCEPT_REQUIRES(
        execution_context::concept::blas_executer<ExecutionContext>() &&
        execution_context::concept::expression_executer<ExecutionContext>() &&
        execution_context::concept::allocation_backend<ExecutionContext>() &&
        blas::concept::product<A, B, C>())>
auto product(const ExecutionContext& execution_context,
             numeric_array_traits::value_type<A> alpha, const A& a, const B& b,
             numeric_array_traits::value_type<A> beta, const C& c) {
  CONTRACT_EXPECT { CONTRACT_ASSERT(is_product_shaped(a, b, c)); };
  auto result = make_numeric_array(execution_context, c);
  emplace_product(execution_context, alpha, a, b, beta, result);
  return result;
}

template <
    class ExecutionContext, class A, class B, class C,
    CONCEPT_REQUIRES(
        execution_context::concept::blas_executer<ExecutionContext>() &&
        execution_context::concept::expression_executer<ExecutionContext>() &&
        execution_context::concept::allocation_backend<ExecutionContext>() &&
        blas::concept::product<A, B, C>())>
auto product(const ExecutionContext& execution_context, const A& a, const B& b,
             numeric_array_traits::value_type<A> beta, const C& c) {
  CONTRACT_EXPECT { CONTRACT_ASSERT(is_product_shaped(a, b, c)); };
  return product(execution_context, 1, a, b, beta, c);
}

template <
    class ExecutionContext, class A, class B,
    CONCEPT_REQUIRES(
        execution_context::concept::blas_executer<ExecutionContext>() &&
        execution_context::concept::allocation_backend<ExecutionContext>() &&
        blas::concept::product<A, B>())>
auto product(const ExecutionContext& execution_context,
             numeric_array_traits::value_type<A> alpha, const A& a,
             const B& b) {
  CONTRACT_EXPECT { CONTRACT_ASSERT(is_product_shaped(a, b)); };
  using Scalar = numeric_array_traits::value_type<A>;
  using Structure = structure::product<numeric_array_traits::structure<A>,
                                       numeric_array_traits::structure<B>>;
  auto result = make_matrix<Scalar, Structure>(get_extent<0>(a),
    get_extent<1>(b), make_aligned_allocator<Scalar>(execution_context));
  emplace_product(execution_context, alpha, a, b, 0, result);
  return result;
}

template <
    class ExecutionContext, class A, class B,
    CONCEPT_REQUIRES(
        execution_context::concept::blas_executer<ExecutionContext>() &&
        execution_context::concept::allocation_backend<ExecutionContext>() &&
        blas::concept::product<A, B>())>
auto product(const ExecutionContext& execution_context, const A& a,
             const B& b) {
  CONTRACT_EXPECT { CONTRACT_ASSERT(is_product_shaped(a, b)); };
  return product(execution_context, 1, a, b);
}
}
}
