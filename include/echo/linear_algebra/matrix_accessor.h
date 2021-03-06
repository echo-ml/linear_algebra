#pragma once

#include <echo/linear_algebra/structure.h>
#include <echo/numeric_array.h>
#include <cassert>

namespace echo {
namespace numeric_array {

//------------------------------------------------------------------------------
// NumericArrayConstAccessor
//------------------------------------------------------------------------------
template <class Derived, class Shape>
struct NumericArrayConstAccessor<Derived, Shape,
                                 linear_algebra::structure::lower_symmetric> {
  decltype(auto) operator()(access_mode::readonly_t, index_t i_prime,
                            index_t j_prime) const {
    const Derived& derived = static_cast<const Derived&>(*this);
    index_t i, j;
    std::tie(i, j) = std::minmax(i_prime, j_prime);
    return derived.k_array()(j, i);
  }
  decltype(auto) operator()(access_mode::readwrite_t, index_t i_prime,
                            index_t j_prime) const {
    const Derived& derived = static_cast<const Derived&>(*this);
    index_t i, j;
    std::tie(i, j) = std::minmax(i_prime, j_prime);
    return derived.k_array()(j, i);
  }
  decltype(auto) operator()(access_mode::raw_t, index_t i_prime,
                            index_t j_prime) const {
    Derived& derived = static_cast<Derived&>(*this);
    index_t i, j;
    std::tie(i, j) = std::minmax(i_prime, j_prime);
    return derived.k_array()(j, i);
  }
  decltype(auto) operator()(index_t i, index_t j) const {
    return this->operator()(access_mode::readwrite, i, j);
  }
};

template <class Derived, class Shape>
struct NumericArrayConstAccessor<Derived, Shape,
                                 linear_algebra::structure::lower_triangular> {
  decltype(auto) operator()(access_mode::readonly_t, index_t i,
                            index_t j) const {
    const Derived& derived = static_cast<const Derived&>(*this);
    uncvref_t<decltype(*derived.data())> result = 0;
    if (i >= j) result = derived.k_array()(i, j);
    return result;
  }
  decltype(auto) operator()(access_mode::readwrite_t, index_t i,
                            index_t j) const {
    assert(i >= j && "can't access upper part of matrix");
    const Derived& derived = static_cast<const Derived&>(*this);
    return derived.k_array()(i, j);
  }
  decltype(auto) operator()(access_mode::raw_t, index_t i, index_t j) const {
    assert(i >= j && "can't access upper part of matrix");
    Derived& derived = static_cast<Derived&>(*this);
    return derived.k_array()(i, j);
  }
  decltype(auto) operator()(index_t i, index_t j) const {
    return this->operator()(access_mode::readonly, i, j);
  }
};

template <class Derived, class Shape>
struct NumericArrayConstAccessor<Derived, Shape,
                                 linear_algebra::structure::upper_triangular> {
  decltype(auto) operator()(access_mode::readonly_t, index_t i,
                            index_t j) const {
    const Derived& derived = static_cast<const Derived&>(*this);
    uncvref_t<decltype(*derived.data())> result = 0;
    if (i <= j) result = derived.k_array()(i, j);
    return result;
  }
  decltype(auto) operator()(access_mode::readwrite_t, index_t i,
                            index_t j) const {
    assert(i <= j && "can't access lower part of matrix");
    const Derived& derived = static_cast<const Derived&>(*this);
    return derived.k_array()(i, j);
  }
  decltype(auto) operator()(access_mode::raw_t, index_t i, index_t j) const {
    assert(i <= j && "can't access lower part of matrix");
    const Derived& derived = static_cast<const Derived&>(*this);
    return derived.k_array()(i, j);
  }
  decltype(auto) operator()(index_t i, index_t j) const {
    return this->operator()(access_mode::readonly, i, j);
  }
};
}
}
