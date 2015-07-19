#pragma once

#define DETAIL_NS detail_lapack_concept

#include <echo/linear_algebra/concept.h>
#include <echo/htl.h>

namespace echo {
namespace linear_algebra {
namespace lapack {
namespace concept {

//------------------------------------------------------------------------------
// potrf
//------------------------------------------------------------------------------
namespace DETAIL_NS {
struct Potrf : Concept {
  template <class A>
  auto require(A&& a)
      -> list<numeric_array::concept::standard_numeric_array<A>(),
              linear_algebra::concept::hermitian_matrix<A>()>;

  template <class A, class B>
  auto require(A&& a, B&& b)
      -> list<numeric_array::concept::standard_numeric_array<A>(),
              linear_algebra::concept::hermitian_matrix<A>(),
              linear_algebra::concept::triangular_matrix<B>(),
              linear_algebra::concept::like_valued<A, B>()>;
};
}

template <class A>
constexpr bool potrf() {
  return models<DETAIL_NS::Potrf, A>();
}

template <class A, class B>
constexpr bool potrf() {
  return models<DETAIL_NS::Potrf, A, B>();
}

//------------------------------------------------------------------------------
// gels
//------------------------------------------------------------------------------
namespace DETAIL_NS {
struct Gels : Concept {
  template <class A, class B>
  auto require(A&& a, B&& b)
      -> list<numeric_array::concept::standard_numeric_array<B>(),
              linear_algebra::concept::weak_general_matrix<A>(),
              linear_algebra::concept::general_matrix<B>(),
              linear_algebra::concept::like_valued<A, B>(),
              same<decltype(get_extent<0>(a)), decltype(get_extent<0>(b))>()>;
};
}

template <class A, class B>
constexpr bool gels() {
  return models<DETAIL_NS::Gels, A, B>();
}

//------------------------------------------------------------------------------
// overdetermined_gels
//------------------------------------------------------------------------------
namespace DETAIL_NS {
struct OverdeterminedGels : Concept {
  template <class A, class B>
  auto require(A&& a, B&& b)
      -> list<gels<A, B>(), !htl::concept::boolean_false_constant<decltype(
                                get_extent<0>(a) >= get_extent<1>(a))>()>;
};
}

template <class A, class B>
constexpr bool overdetermined_gels() {
  return models<DETAIL_NS::OverdeterminedGels, A, B>();
}
}
}
}
}

#undef DETAIL_NS
