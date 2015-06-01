#pragma once

#include <echo/linear_algebra/concept.h>

namespace echo {
namespace linear_algebra {
namespace blas {
namespace concept {

///////////////////////////////
// compatible_product_shapes //
///////////////////////////////

namespace detail {
namespace blas_concept {
struct CompatibleProductShapes : Concept {
  template <class A, class B>
  auto require(A&&, B && ) -> list<same<k_array_traits::extent_type<1, A>,
                                        k_array_traits::extent_type<0, B>>()

                                   >;

  template <class A, class B, class C>
  auto require(A&&, B&&, C && )
      -> list<same<k_array_traits::extent_type<0, A>,
                   k_array_traits::extent_type<0, C>>(),
              same<k_array_traits::extent_type<1, A>,
                   k_array_traits::extent_type<0, B>>(),
              same<k_array_traits::extent_type<1, B>,
                   k_array_traits::extent_type<1, C>>()>;
};
}
}

template <class A, class B>
constexpr bool compatible_product_shapes() {
  return models<detail::blas_concept::CompatibleProductShapes, A, B>();
}

template <class A, class B, class C>
constexpr bool compatible_product_shapes() {
  return models<detail::blas_concept::CompatibleProductShapes, A, B, C>();
}

////////////////////
// matrix_strided //
////////////////////

namespace detail {
namespace blas_concept {
struct MatrixStrided : Concept {
  template <class T>
  auto require(T&& x) -> list<
      std::is_same<k_array_traits::stride_type<0, T>, StaticIndex<1>>::value>;
};
struct TransposedMatrixStrided : Concept {
  template <class T>
  auto require(T&& x) -> list<
      std::is_same<k_array_traits::stride_type<1, T>, StaticIndex<1>>::value>;
};
}
}

template <class T>
constexpr bool matrix_strided() {
  return linear_algebra::concept::matrix<T>()
             ? models<detail::blas_concept::MatrixStrided, T>()
             : linear_algebra::concept::weak_matrix<T>()
                   ? models<detail::blas_concept::TransposedMatrixStrided, T>()
                   : false;
}

///////////////////////////////
// compatible_product_values //
///////////////////////////////

namespace detail {
namespace blas_concept {
struct CompatibleProductValues : Concept {
  template <class A, class B>
  auto require(A&&, B && ) -> list<
      same<k_array_traits::value_type<A>, k_array_traits::value_type<B>>()>;

  template <class A, class B, class C>
  auto require(A&&, B&&, C && ) -> list<
      same<k_array_traits::value_type<A>, k_array_traits::value_type<B>>(),
      same<k_array_traits::value_type<C>, k_array_traits::value_type<B>>()>;
};
}
}

template <class A, class B>
constexpr bool compatible_product_values() {
  return models<detail::blas_concept::CompatibleProductValues, A, B>();
}

template <class A, class B, class C>
constexpr bool compatible_product_values() {
  return models<detail::blas_concept::CompatibleProductValues, A, B, C>();
}

//////////////////////////////////
// matrix_matrix_product_shaped //
//////////////////////////////////

namespace detail {
namespace blas_concept {
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
}

template <class A, class B>
constexpr bool matrix_matrix_product_shaped() {
  return models<detail::blas_concept::MatrixMatrixProductShaped, A, B>();
}

template <class A, class B, class C>
constexpr bool matrix_matrix_product_shaped() {
  return models<detail::blas_concept::MatrixMatrixProductShaped, A, B, C>();
}

//////////////////////////////////
// matrix_vector_product_shaped //
//////////////////////////////////

namespace detail {
namespace blas_concept {
struct MatrixVectorProductShaped : Concept {
  template<class A, class X>
  auto require(A&&, X&&) -> list<
    compatible_product_shapes<A, X>(), matrix_strided<A>(),
    compatible_product_values<A, X>()
  >;

  template <class A, class X, class Y>
  auto require(A&&, X&&, Y && )
      -> list<compatible_product_shapes<A, X, Y>(), matrix_strided<A>(),
              compatible_product_values<A, X, Y>()>;
};
}
}

template<class A, class X>
constexpr bool matrix_vector_product_shaped() {
  return models<detail::blas_concept::MatrixVectorProductShaped, A, X>();
}

template <class A, class X, class Y>
constexpr bool matrix_vector_product_shaped() {
  return models<detail::blas_concept::MatrixVectorProductShaped, A, X, Y>();
}

/////////////////////////
// product_dimensioned //
/////////////////////////

template <class A, class B, class C,
          CONCEPT_REQUIRES(linear_algebra::concept::weak_matrix<A>() &&
                           linear_algebra::concept::weak_matrix<B>() &&
                           linear_algebra::concept::weak_matrix<C>())>
constexpr bool product_dimensioned() {
  return std::is_same<k_array_traits::extent_type<0, A>,
                      k_array_traits::extent_type<0, C>>::value &&
         std::is_same<k_array_traits::extent_type<1, A>,
                      k_array_traits::extent_type<0, B>>::value &&
         std::is_same<k_array_traits::extent_type<1, B>,
                      k_array_traits::extent_type<1, C>>::value &&
         std::is_same<k_array_traits::value_type<A>,
                      k_array_traits::value_type<B>>::value &&
         std::is_same<k_array_traits::value_type<B>,
                      k_array_traits::value_type<C>>::value;
}

//////////
// gemm //
//////////

template<class A, class B>
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

//////////
// gemv //
//////////

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

///////////////
// left_symm //
///////////////

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

////////////////
// right_symm //
////////////////

template <class A, class B>
constexpr bool right_symm() {
  return matrix_matrix_product_shaped<A, B>() &&
         linear_algebra::concept::general_matrix<A>() &&
         linear_algebra::concept::symmetric_matrix<B>() &&
         !linear_algebra::concept::column_vector<B>();
}

template <class A, class B, class C>
constexpr bool right_symm() {
  return matrix_matrix_product_shaped<A, B, C>() &&
         linear_algebra::concept::general_matrix<A>() &&
         linear_algebra::concept::symmetric_matrix<B>() &&
         linear_algebra::concept::general_matrix<C>() &&
         !linear_algebra::concept::column_vector<C>();
}

//////////
// symm //
//////////

template <class A, class B>
constexpr bool symm() {
  return left_symm<A, B>() || right_symm<A, B>();
}

template <class A, class B, class C>
constexpr bool symm() {
  return left_symm<A, B, C>() || right_symm<A, B, C>();
}

//////////
// symv //
//////////

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

//////////
// trsm //
//////////

template <class A, class B>
constexpr bool trsm() {
  return linear_algebra::concept::weak_triangular_matrix<A>() &&
         linear_algebra::concept::modifiable_general_matrix_forward<B>();
}


//////////
// trsv //
//////////

template <class A, class X>
constexpr bool trsv() {
  return linear_algebra::concept::weak_triangular_matrix<A>() &&
         linear_algebra::concept::modifiable_column_vector_forward<X>();
}

/////////////
// product //
/////////////

template <class A, class B>
constexpr bool product() {
  return gemm<A, B>() || gemv<A, B>() || symm<A, B>() ||
         symv<A, B>();
}

template <class A, class B, class C>
constexpr bool product() {
  return gemm<A, B, C>() || gemv<A, B, C>() || symm<A, B, C>() ||
         symv<A, B, C>();
}
}
}
}
}
