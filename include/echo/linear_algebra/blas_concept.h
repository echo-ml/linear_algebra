#pragma once

#define DETAIL_NS detail_blas_concept

#include <echo/linear_algebra/concept.h>

namespace echo {
namespace linear_algebra {
namespace blas {
namespace concept {

//------------------------------------------------------------------------------
// compatible_product_shapes
//------------------------------------------------------------------------------
namespace DETAIL_NS {
struct CompatibleProductShapes : Concept {
  template <class A, class B>
  auto require(A&& a, B&& b)
      -> list<same<decltype(get_extent<1>(a)), decltype(get_extent<0>(b))>()>;

  template <class A, class B, class C>
  auto require(A&& a, B&& b, C&& c)
      -> list<same<decltype(get_extent<0>(a)), decltype(get_extent<0>(c))>(),
              same<decltype(get_extent<1>(a)), decltype(get_extent<0>(b))>(),
              same<decltype(get_extent<1>(b)), decltype(get_extent<1>(c))>()>;
};
}

template <class A, class B>
constexpr bool compatible_product_shapes() {
  return models<DETAIL_NS::CompatibleProductShapes, A, B>();
}

template <class A, class B, class C>
constexpr bool compatible_product_shapes() {
  return models<DETAIL_NS::CompatibleProductShapes, A, B, C>();
}

//------------------------------------------------------------------------------
// matrix_strided
//------------------------------------------------------------------------------
namespace DETAIL_NS {
struct MatrixStrided : Concept {
  template <class T>
  auto require(T&& x)
      -> list<(linear_algebra::concept::matrix<T>() &&
               same<decltype(get_stride<0>(x)), StaticIndex<1>>()) ||
              (linear_algebra::concept::weak_matrix<T>() &&
               matrix_traits::operation<T>() != matrix_operation_t::none &&
               same<decltype(get_stride<1>(x)), StaticIndex<1>>())>;
};
}

template <class T>
constexpr bool matrix_strided() {
  return models<DETAIL_NS::MatrixStrided, T>();
}

//------------------------------------------------------------------------------
// compatible_product_values
//------------------------------------------------------------------------------
namespace DETAIL_NS {
struct CompatibleProductValues : Concept {
  template <class A, class B>
  auto require(A&& a, B&& b) -> list<
      same<uncvref_t<decltype(a.data())>, uncvref_t<decltype(b.data())>>()>;

  template <class A, class B, class C>
  auto require(A&& a, B&& b, C&& c) -> list<
      same<uncvref_t<decltype(a.data())>, uncvref_t<decltype(b.data())>>(),
      same<uncvref_t<decltype(a.data())>, uncvref_t<decltype(c.data())>>()>;
};
}

template <class A, class B>
constexpr bool compatible_product_values() {
  return models<DETAIL_NS::CompatibleProductValues, A, B>();
}

template <class A, class B, class C>
constexpr bool compatible_product_values() {
  return models<DETAIL_NS::CompatibleProductValues, A, B, C>();
}

//------------------------------------------------------------------------------
// matrix_matrix_product_shaped
//------------------------------------------------------------------------------
namespace DETAIL_NS {
struct MatrixMatrixProductShaped : Concept {
  template <class A, class B>
  auto require(A&&, B && )
      -> list<compatible_product_shapes<A, B>(), matrix_strided<A>(),
              matrix_strided<B>(), compatible_product_values<A, B>()>;

  template <class A, class B, class C>
  auto require(A&&, B&&, C && )
      -> list<compatible_product_shapes<A, B, C>(), matrix_strided<A>(),
              matrix_strided<B>(), matrix_strided<C>(),
              compatible_product_values<A, B, C>()>;
};
}

template <class A, class B>
constexpr bool matrix_matrix_product_shaped() {
  return models<DETAIL_NS::MatrixMatrixProductShaped, A, B>();
}

template <class A, class B, class C>
constexpr bool matrix_matrix_product_shaped() {
  return models<DETAIL_NS::MatrixMatrixProductShaped, A, B, C>();
}

//------------------------------------------------------------------------------
// matrix_vector_product_shaped
//------------------------------------------------------------------------------
namespace DETAIL_NS {
struct MatrixVectorProductShaped : Concept {
  template <class A, class X>
  auto require(A&&, X && )
      -> list<compatible_product_shapes<A, X>(), matrix_strided<A>(),
              compatible_product_values<A, X>()>;

  template <class A, class X, class Y>
  auto require(A&&, X&&, Y && )
      -> list<compatible_product_shapes<A, X, Y>(), matrix_strided<A>(),
              compatible_product_values<A, X, Y>()>;
};
}

template <class A, class X>
constexpr bool matrix_vector_product_shaped() {
  return models<DETAIL_NS::MatrixVectorProductShaped, A, X>();
}

template <class A, class X, class Y>
constexpr bool matrix_vector_product_shaped() {
  return models<DETAIL_NS::MatrixVectorProductShaped, A, X, Y>();
}

//------------------------------------------------------------------------------
// gemm
//------------------------------------------------------------------------------
namespace DETAIL_NS {
struct Gemm : Concept {
  template <class A, class B>
  auto require(A&&, B && )
      -> list<matrix_matrix_product_shaped<A, B>() &&
              linear_algebra::concept::weak_general_matrix<A>() &&
              linear_algebra::concept::weak_general_matrix<B>() &&
              !linear_algebra::concept::column_vector<B>()>;
  template <class A, class B, class C>
  auto require(A&&, B&&, C && )
      -> list<matrix_matrix_product_shaped<A, B, C>() &&
              linear_algebra::concept::weak_general_matrix<A>() &&
              linear_algebra::concept::weak_general_matrix<B>() &&
              linear_algebra::concept::general_matrix<C>() &&
              !linear_algebra::concept::column_vector<C>()>;
};
}

template <class A, class B>
constexpr bool gemm() {
  return matrix_matrix_product_shaped<A, B>() &&
         linear_algebra::concept::weak_general_matrix<A>() &&
         linear_algebra::concept::weak_general_matrix<B>() &&
         !linear_algebra::concept::column_vector<B>();
}

template <class A, class B, class C>
constexpr bool gemm() {
  return matrix_matrix_product_shaped<A, B, C>() &&
         linear_algebra::concept::weak_general_matrix<A>() &&
         linear_algebra::concept::weak_general_matrix<B>() &&
         linear_algebra::concept::general_matrix<C>() &&
         !linear_algebra::concept::column_vector<C>();
}

//------------------------------------------------------------------------------
// gemv
//------------------------------------------------------------------------------
template <class A, class X>
constexpr bool gemv() {
  return matrix_vector_product_shaped<A, X>() &&
         linear_algebra::concept::weak_general_matrix<A>() &&
         linear_algebra::concept::weak_general_matrix<X>() &&
         linear_algebra::concept::column_vector<X>();
}

template <class A, class X, class Y>
constexpr bool gemv() {
  return matrix_vector_product_shaped<A, X, Y>() &&
         linear_algebra::concept::weak_general_matrix<A>() &&
         linear_algebra::concept::weak_general_matrix<X>() &&
         linear_algebra::concept::column_vector<Y>();
}

//------------------------------------------------------------------------------
// left_symm
//------------------------------------------------------------------------------
template <class A, class B>
constexpr bool left_symm() {
  return matrix_matrix_product_shaped<A, B>() &&
         linear_algebra::concept::symmetric_matrix<A>() &&
         linear_algebra::concept::general_matrix<B>() &&
         !linear_algebra::concept::column_vector<B>();
}

template <class A, class B, class C>
constexpr bool left_symm() {
  return matrix_matrix_product_shaped<A, B, C>() &&
         linear_algebra::concept::symmetric_matrix<A>() &&
         linear_algebra::concept::general_matrix<B>() &&
         linear_algebra::concept::general_matrix<C>() &&
         !linear_algebra::concept::column_vector<C>();
}

//------------------------------------------------------------------------------
// right_symm
//------------------------------------------------------------------------------
template <class A, class B>
constexpr bool right_symm() {
  return matrix_matrix_product_shaped<A, B>() &&
         linear_algebra::concept::general_matrix<A>() &&
         linear_algebra::concept::symmetric_matrix<B>();
}

template <class A, class B, class C>
constexpr bool right_symm() {
  return matrix_matrix_product_shaped<A, B, C>() &&
         linear_algebra::concept::general_matrix<A>() &&
         linear_algebra::concept::symmetric_matrix<B>() &&
         linear_algebra::concept::general_matrix<C>();
}

//------------------------------------------------------------------------------
// symm
//------------------------------------------------------------------------------
template <class A, class B>
constexpr bool symm() {
  return left_symm<A, B>() || right_symm<A, B>();
}

template <class A, class B, class C>
constexpr bool symm() {
  return left_symm<A, B, C>() || right_symm<A, B, C>();
}

//------------------------------------------------------------------------------
// symv
//------------------------------------------------------------------------------
template <class A, class X>
constexpr bool symv() {
  return matrix_vector_product_shaped<A, X>() &&
         linear_algebra::concept::symmetric_matrix<A>() &&
         linear_algebra::concept::column_vector<X>();
}

template <class A, class X, class Y>
constexpr bool symv() {
  return matrix_vector_product_shaped<A, X, Y>() &&
         linear_algebra::concept::symmetric_matrix<A>() &&
         linear_algebra::concept::column_vector<X>() &&
         linear_algebra::concept::column_vector<Y>();
}

//------------------------------------------------------------------------------
// left_trsm
//------------------------------------------------------------------------------
template <class A, class B>
constexpr bool left_trsm() {
  return matrix_matrix_product_shaped<A, B, B>() &&
         linear_algebra::concept::weak_triangular_matrix<A>() &&
         linear_algebra::concept::general_matrix<B>() &&
         !linear_algebra::concept::column_vector<B>();
}

//------------------------------------------------------------------------------
// right_trsm
//------------------------------------------------------------------------------
template <class A, class B>
constexpr bool right_trsm() {
  return matrix_matrix_product_shaped<B, A, B>() &&
         linear_algebra::concept::weak_triangular_matrix<A>() &&
         linear_algebra::concept::general_matrix<B>();
}

//------------------------------------------------------------------------------
// trsm
//------------------------------------------------------------------------------
template <class A, class B>
constexpr bool trsm() {
  return left_trsm<A, B>() || right_trsm<A, B>();
}

//------------------------------------------------------------------------------
// trsv
//------------------------------------------------------------------------------
template <class A, class X>
constexpr bool trsv() {
  return matrix_matrix_product_shaped<A, X, X>() &&
         linear_algebra::concept::weak_triangular_matrix<A>() &&
         linear_algebra::concept::column_vector<X>();
}

//------------------------------------------------------------------------------
// product
//------------------------------------------------------------------------------
template <class A, class B>
constexpr bool product() {
  return gemm<A, B>() || gemv<A, B>() || symm<A, B>() || symv<A, B>();
}

template <class A, class B, class C>
constexpr bool product() {
  return gemm<A, B, C>() || gemv<A, B, C>() || symm<A, B, C>() ||
         symv<A, B, C>();
}

//------------------------------------------------------------------------------
// left_solve
//------------------------------------------------------------------------------
template <class A, class B>
constexpr bool left_solve() {
  return left_trsm<A, B>() || trsv<A, B>();
}

//------------------------------------------------------------------------------
// solve
//------------------------------------------------------------------------------
template <class A, class B>
constexpr bool solve() {
  return trsm<A, B>() || trsv<A, B>();
}
}
}
}
}

#undef DETAIL_NS
