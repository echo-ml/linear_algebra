#pragma once

#include <echo/linear_algebra/blas_concept.h>
#include <echo/execution_context.h>
#include <echo/linear_algebra/matrix_traits.h>
#include <cassert>

namespace echo {
namespace linear_algebra {

///////////////////////////
// get_leading_dimension //
///////////////////////////

namespace detail {
namespace product {

template <class A, CONCEPT_REQUIRES(concept::matrix<A>())>
auto get_leading_dimension(const A& a) {
  return get_stride<1>(a);
}

template <class A, CONCEPT_REQUIRES(concept::operated_matrix<A>())>
auto get_leading_dimension(const A& a) {
  return get_stride<0>(a);
}
}
}

/////////////////////
// emplace_product //
/////////////////////

// gemm
template <class ExecutionContext, class A, class B, class C,
          CONCEPT_REQUIRES(
              execution_context::concept::blas_executer<ExecutionContext>() &&
              blas::concept::gemm<A, B, uncvref_t<C>>() &&
              linear_algebra::concept::modifiable_matrix_forward<C>())>
auto emplace_product(const ExecutionContext& execution_context,
                     k_array_traits::value_type<A> alpha, const A& a,
                     const B& b, k_array_traits::value_type<A> beta, C&& c) {
  auto a_m = get_extent<0>(a);
  auto a_n = get_extent<1>(a);
  auto lda = detail::product::get_leading_dimension(a);

  auto b_m = get_extent<0>(b);
  auto b_n = get_extent<1>(b);
  auto ldb = detail::product::get_leading_dimension(b);

  auto c_m = get_extent<0>(c);
  auto c_n = get_extent<1>(c);
  auto ldc = detail::product::get_leading_dimension(c);

  assert(a_m == c_m);
  assert(a_n == b_m);
  assert(b_n == c_n);

  constexpr auto a_operation = matrix_traits::operation<A>();
  constexpr auto b_operation = matrix_traits::operation<B>();

  execution_context.gemm(matrix_traits::operation<A>(),
                         matrix_traits::operation<B>(), c_m, c_n, a_n, alpha,
                         a.data(), lda, b.data(), ldb, beta, c.data(), ldc);
  return make_view(c);
}

// gemv
template <class ExecutionContext, class A, class X, class Y,
          CONCEPT_REQUIRES(
              execution_context::concept::blas_executer<ExecutionContext>() &&
              blas::concept::gemv<A, X, uncvref_t<Y>>() &&
              linear_algebra::concept::modifiable_matrix_forward<Y>())>
auto emplace_product(const ExecutionContext& execution_context,
                     k_array_traits::value_type<A> alpha, const A& a,
                     const X& x, k_array_traits::value_type<A> beta, Y&& y) {
  auto a_m = get_extent<0>(a);
  auto a_n = get_extent<1>(a);
  auto lda = detail::product::get_leading_dimension(a);

  auto x_m = get_extent<0>(x);
  auto stride_x = get_stride<0>(x);

  auto y_m = get_extent<0>(y);
  auto stride_y = get_stride<0>(y);

  assert(a_m == y_m);
  assert(a_n == x_m);

  constexpr auto a_operation = matrix_traits::operation<A>();

  execution_context.gemv(matrix_traits::operation<A>(), a_m, a_n, alpha,
                         a.data(), lda, x.data(), stride_x, beta, y.data(),
                         stride_y);
  return make_view(y);
}

// symm
template <class ExecutionContext, class A, class B, class C,
          CONCEPT_REQUIRES(
              execution_context::concept::blas_executer<ExecutionContext>() &&
              blas::concept::left_symm<A, B, uncvref_t<C>>() &&
              linear_algebra::concept::modifiable_matrix_forward<C>())>
auto emplace_product(const ExecutionContext& execution_context,
                     k_array_traits::value_type<A> alpha, const A& a,
                     const B& b, k_array_traits::value_type<A> beta, C&& c) {
  auto a_m = get_extent<0>(a);
  auto a_n = get_extent<1>(a);
  auto lda = detail::product::get_leading_dimension(a);

  auto b_m = get_extent<0>(b);
  auto b_n = get_extent<1>(b);
  auto ldb = detail::product::get_leading_dimension(b);

  auto c_m = get_extent<0>(c);
  auto c_n = get_extent<1>(c);
  auto ldc = detail::product::get_leading_dimension(c);

  assert(a_m == c_m);
  assert(a_n == b_m);
  assert(b_n == c_n);

  execution_context->symm(execution_context::matrix_side_t::left,
                          A::storage_uplo, a_m, b_n, alpha, a.data(), lda,
                          b.data(), ldb, beta, c.data(), ldc);
  return make_matrix_view(c);
}

template <class ExecutionContext, class A, class B, class C,
          CONCEPT_REQUIRES(
              execution_context::concept::blas_executer<ExecutionContext>() &&
              blas::concept::right_symm<A, B, uncvref_t<C>>() &&
              linear_algebra::concept::modifiable_matrix_forward<C>())>
auto emplace_product(const ExecutionContext& execution_context,
                     k_array_traits::value_type<A> alpha, const A& a,
                     const B& b, k_array_traits::value_type<A> beta, C&& c) {
  auto a_m = get_extent<0>(a);
  auto a_n = get_extent<1>(a);
  auto lda = detail::product::get_leading_dimension(a);

  auto b_m = get_extent<0>(b);
  auto b_n = get_extent<1>(b);
  auto ldb = detail::product::get_leading_dimension(b);

  auto c_m = get_extent<0>(c);
  auto c_n = get_extent<1>(c);
  auto ldc = detail::product::get_leading_dimension(c);

  assert(a_m == c_m);
  assert(a_n == b_m);
  assert(b_n == c_n);

  execution_context->symm(execution_context::matrix_side_t::right,
                          B::storage_uplo, a_m, b_n, alpha, b.data(), ldb,
                          a.data(), lda, beta, c.data(), ldc);
  return make_matrix_view(c);
}

template <class ExecutionContext, class A, class B, class C,
          CONCEPT_REQUIRES(
              execution_context::concept::blas_executer<ExecutionContext>() &&
              blas::concept::product<A, B, uncvref_t<C>>() &&
              linear_algebra::concept::modifiable_matrix_forward<C>())>
auto emplace_product(const ExecutionContext& execution_context,
                     k_array_traits::value_type<A> alpha, const A& a,
                     const B& b, C&& c) {
  return emplace_product(execution_context, alpha, a, b, 0, c);
}

template <class ExecutionContext, class A, class B, class C,
          CONCEPT_REQUIRES(
              execution_context::concept::blas_executer<ExecutionContext>() &&
              blas::concept::product<A, B, uncvref_t<C>>() &&
              linear_algebra::concept::modifiable_matrix_forward<C>())>
auto emplace_product(const ExecutionContext& execution_context, const A& a,
                     const B& b, k_array_traits::value_type<A> beta, C&& c) {
  return emplace_product(execution_context, 1, a, b, beta, c);
}

template <class ExecutionContext, class A, class B, class C,
          CONCEPT_REQUIRES(
              execution_context::concept::blas_executer<ExecutionContext>() &&
              blas::concept::product<A, B, uncvref_t<C>>() &&
              linear_algebra::concept::modifiable_matrix_forward<C>())>
auto emplace_product(const ExecutionContext& execution_context, const A& a,
                     const B& b, C&& c) {
  return emplace_product(execution_context, 1, a, b, 0, c);
}

/////////////
// product //
/////////////

template <
    class ExecutionContext, class A, class B, class C,
    CONCEPT_REQUIRES(
        execution_context::concept::blas_executer<ExecutionContext>() &&
        execution_context::concept::allocation_backend<ExecutionContext>() &&
        blas::concept::product<A, B, C>())>
auto product(const ExecutionContext& execution_context,
             k_array_traits::value_type<A> alpha, const A& a, const B& b,
             k_array_traits::value_type<A> beta, const C& c) {
  using Scalar = k_array_traits::value_type<A>;
  using Structure = numeric_array_traits::structure<C>;
  auto c_shape = c.shape();
  auto allocator = make_aligned_allocator<Scalar>(execution_context);
  NumericArray<Scalar, decltype(c_shape), Structure, decltype(allocator)>
      result(c_shape, allocator);
  emplace_product(execution_context, alpha, a, b, beta, result);
  return result;
}

template <
    class ExecutionContext, class A, class B, class C,
    CONCEPT_REQUIRES(
        execution_context::concept::blas_executer<ExecutionContext>() &&
        execution_context::concept::allocation_backend<ExecutionContext>() &&
        blas::concept::product<A, B, C>())>
auto product(const ExecutionContext& execution_context,
             k_array_traits::value_type<A> alpha, const A& a, const B& b,
             const C& c) {
  return product(execution_context, alpha, a, b, 0, c);
}

template <
    class ExecutionContext, class A, class B, class C,
    CONCEPT_REQUIRES(
        execution_context::concept::blas_executer<ExecutionContext>() &&
        execution_context::concept::allocation_backend<ExecutionContext>() &&
        blas::concept::product<A, B, C>())>
auto product(const ExecutionContext& execution_context, const A& a, const B& b,
             k_array_traits::value_type<A> beta, const C& c) {
  return product(execution_context, 1, a, b, beta, c);
}

template <
    class ExecutionContext, class A, class B, class C,
    CONCEPT_REQUIRES(
        execution_context::concept::blas_executer<ExecutionContext>() &&
        execution_context::concept::allocation_backend<ExecutionContext>() &&
        blas::concept::product<A, B, C>())>
auto product(const ExecutionContext& execution_context, const A& a, const B& b,
             const C& c) {
  return product(execution_context, 1, a, b, 0, c);
}
}
}
