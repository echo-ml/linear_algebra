#pragma once

#include <echo/linear_algebra/structure.h>
#include <echo/numeric_array.h>
#include <cassert>

namespace echo {
namespace numeric_array {

///////////////////////////////
// NumericArrayConstAccessor //
///////////////////////////////

template<class Derived, class BaseArray, class Shape>
struct NumericArrayConstAccessor<Derived, BaseArray, Shape,
  linear_algebra::structure::matrix_general>
  : NumericArrayConstAccessor<Derived, BaseArray, Shape,
      execution_context::structure::general>
{
  using NumericArrayConstAccessor<Derived, BaseArray, Shape,
    execution_context::structure::general>::operator();

  CONCEPT_MEMBER_REQUIRES(shape_traits::fixed_dimension<1, Shape>())
  decltype(auto) operator()(access_mode::readonly_t, index_t i) const {
    return this->operator()(access_mode::readonly, i, 0);
  }

  CONCEPT_MEMBER_REQUIRES(shape_traits::fixed_dimension<0, Shape>())
  decltype(auto) operator()(access_mode::readonly_t, index_t j) const {
    return this->operator()(access_mode::readonly, 0, j);
  }

  CONCEPT_MEMBER_REQUIRES(shape_traits::fixed_dimension<1, Shape>())
  decltype(auto) operator()(access_mode::readwrite_t, index_t i) const {
    return this->operator()(access_mode::readwrite, i, 0);
  }

  CONCEPT_MEMBER_REQUIRES(shape_traits::fixed_dimension<0, Shape>())
  decltype(auto) operator()(access_mode::readwrite_t, index_t j) const {
    return this->operator()(access_mode::readwrite, 0, j);
  }

  CONCEPT_MEMBER_REQUIRES(shape_traits::fixed_dimension<1, Shape>())
  decltype(auto) operator()(access_mode::raw_t, index_t i) const {
    return this->operator()(access_mode::raw, i, 0);
  }

  CONCEPT_MEMBER_REQUIRES(shape_traits::fixed_dimension<0, Shape>())
  decltype(auto) operator()(access_mode::raw_t, index_t j) const {
    return this->operator()(access_mode::raw, 0, j);
  }

  CONCEPT_MEMBER_REQUIRES(shape_traits::fixed_dimension<1, Shape>())
  decltype(auto) operator()(index_t i) const {
    return this->operator()(i, 0);
  }

  CONCEPT_MEMBER_REQUIRES(shape_traits::fixed_dimension<0, Shape>())
  decltype(auto) operator()(index_t j) const {
    return this->operator()(0, j);
  }
};

template <class Derived, class BaseArray, class Shape>
struct NumericArrayConstAccessor<Derived, BaseArray, Shape,
                                 linear_algebra::structure::lower_symmetric> {
  decltype(auto) operator()(access_mode::readonly_t, index_t i,
                            index_t j) const {
    const Derived& derived = static_cast<const Derived&>(*this);
    std::tie(i, j) = std::minmax(i, j);
    return derived.BaseArray::operator()(j, i);
  }
  decltype(auto) operator()(access_mode::readwrite_t, index_t i,
                            index_t j) const {
    Derived& derived = static_cast<Derived&>(*this);
    std::tie(i, j) = std::minmax(i, j);
    return derived.BaseArray::operator()(j, i);
  }
  decltype(auto) operator()(access_mode::raw_t, index_t i, index_t j) const {
    Derived& derived = static_cast<Derived&>(*this);
    std::tie(i, j) = std::minmax(i, j);
    return derived.BaseArray::operator()(j, i);
  }
  decltype(auto) operator()(index_t i, index_t j) const {
    return this->operator()(access_mode::readonly, i, j);
  }
};

template <class Derived, class BaseArray, class Shape>
struct NumericArrayConstAccessor<Derived, BaseArray, Shape,
                                 linear_algebra::structure::lower_triangular> {
  decltype(auto) operator()(access_mode::readonly_t, index_t i,
                            index_t j) const {
    const Derived& derived = static_cast<const Derived&>(*this);
    if (i < j)
      return 0;
    else
      return derived.BaseArray::operator()(i, j);
  }
  decltype(auto) operator()(access_mode::readwrite_t, index_t i,
                            index_t j) const {
    assert(i >= j && "can't access upper part of matrix");
    Derived& derived = static_cast<Derived&>(*this);
    return derived.BaseArray::operator()(i, j);
  }
  decltype(auto) operator()(access_mode::raw_t, index_t i, index_t j) const {
    assert(i >= j && "can't access upper part of matrix");
    Derived& derived = static_cast<Derived&>(*this);
    return derived.BaseArray::operator()(i, j);
  }
  decltype(auto) operator()(index_t i, index_t j) const {
    return this->operator()(access_mode::readonly, i, j);
  }
};

template <class Derived, class BaseArray, class Shape>
struct NumericArrayConstAccessor<Derived, BaseArray, Shape,
                                 linear_algebra::structure::upper_triangular> {
  decltype(auto) operator()(access_mode::readonly_t, index_t i,
                            index_t j) const {
    const Derived& derived = static_cast<const Derived&>(*this);
    if (i > j)
      return 0;
    else
      return derived.BaseArray::operator()(i, j);
  }
  decltype(auto) operator()(access_mode::readwrite_t, index_t i,
                            index_t j) const {
    assert(i <= j && "can't access lower part of matrix");
    Derived& derived = static_cast<Derived&>(*this);
    return derived.BaseArray::operator()(i, j);
  }
  decltype(auto) operator()(access_mode::raw_t, index_t i, index_t j) const {
    assert(i <= j && "can't access lower part of matrix");
    Derived& derived = static_cast<Derived&>(*this);
    return derived.BaseArray::operator()(i, j);
  }
  decltype(auto) operator()(index_t i, index_t j) const {
    return this->operator()(access_mode::readonly, i, j);
  }
};
}
}
