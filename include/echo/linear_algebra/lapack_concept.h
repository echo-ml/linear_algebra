#pragma once

#define DETAIL_NS detail_lapack_concept

#include <echo/linear_algebra/concept.h>

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
}
}
}
}

#undef DETAIL_NS
