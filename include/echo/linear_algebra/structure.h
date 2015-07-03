#pragma once

#include <echo/execution_context.h>
#include <echo/numeric_array.h>
#include <echo/concept.h>

namespace echo {
namespace linear_algebra {

struct linear_algebra_structure_adl_tag {};

namespace structure {

using execution_context::structure::uplo_t;
using execution_context::matrix_diagonal_fill_t;

struct general : execution_context::structure::general,
                 linear_algebra_structure_adl_tag {};

struct matrix_general : execution_context::structure::general,
                        linear_algebra_structure_adl_tag {};

struct vector_general : execution_context::structure::general,
                        linear_algebra_structure_adl_tag {};

struct symmetric_base : numeric_array::structure::equal_dimensional,
                        linear_algebra_structure_adl_tag {};
struct hermitian_base : numeric_array::structure::equal_dimensional,
                        linear_algebra_structure_adl_tag {};
struct triangular_base : numeric_array::structure::equal_dimensional,
                         linear_algebra_structure_adl_tag {};

template <uplo_t Uplo>
struct symmetric_uplo : symmetric_base {
  static const uplo_t storage_uplo = Uplo;
};

template <uplo_t StorageUplo>
struct hermitian_uplo : hermitian_base {
  static const uplo_t storage_uplo = StorageUplo;
};

template <uplo_t Uplo>
struct triangular : triangular_base {
  static const uplo_t uplo = Uplo;
};

using lower_symmetric = symmetric_uplo<uplo_t::lower>;
using upper_symmetric = symmetric_uplo<uplo_t::upper>;
using symmetric = lower_symmetric;

using lower_hermitian = hermitian_uplo<uplo_t::lower>;
using upper_hermitian = hermitian_uplo<uplo_t::upper>;
using hermitian = hermitian_uplo<uplo_t::lower>;

using lower_triangular = triangular<uplo_t::lower>;
using upper_triangular = triangular<uplo_t::upper>;

struct diagonal : execution_context::structure::base,
                  linear_algebra_structure_adl_tag {};

namespace concept {

///////////////
// symmetric //
///////////////

namespace detail {
namespace structure {
template <uplo_t Uplo>
auto symmetric_impl(symmetric_uplo<Uplo>) -> std::true_type;

auto symmetric_impl(...) -> std::false_type;
}
}

template <class T>
constexpr bool symmetric() {
  using Result = decltype(detail::structure::symmetric_impl(std::declval<T>()));
  return Result::value;
}

///////////////
// hermitian //
///////////////

namespace detail {
namespace structure {
template <uplo_t Uplo>
auto hermitian_impl(hermitian_uplo<Uplo>) -> std::true_type;

auto hermitian_impl(...) -> std::false_type;
}
}

template <class T>
constexpr bool hermitian() {
  using Result = decltype(detail::structure::hermitian_impl(std::declval<T>()));
  return Result::value;
}

////////////////
// triangular //
////////////////

namespace detail {
namespace structure {
template <uplo_t Uplo>
auto triangular_impl(triangular<Uplo>) -> std::true_type;

auto triangular_impl(...) -> std::false_type;
}
}

template <class T>
constexpr bool triangular() {
  using Result =
      decltype(detail::structure::triangular_impl(std::declval<T>()));
  return Result::value;
}

//////////////
// diagonal //
//////////////

template <class T>
constexpr bool diagonal() {
  return std::is_same<T, linear_algebra::structure::diagonal>::value;
}

////////////
// square //
////////////

template <class T>
constexpr bool square() {
  return symmetric<T>() || hermitian<T>() || triangular<T>() || diagonal<T>();
}
}

/////////////
// product //
/////////////

namespace detail {
namespace structure {

template <class Structure>
auto product_impl(Structure, Structure) -> Structure;

auto product_impl(diagonal, matrix_general) -> matrix_general;
auto product_impl(matrix_general, diagonal) -> matrix_general;

template <class T,
          CONCEPT_REQUIRES(concept::symmetric<T>() || concept::hermitian<T>())>
auto product_impl(diagonal, T) -> T;

template <class T,
          CONCEPT_REQUIRES(concept::symmetric<T>() || concept::hermitian<T>())>
auto product_impl(T, diagonal) -> T;

template <class T, CONCEPT_REQUIRES(concept::triangular<T>())>
auto product_impl(diagonal, T) -> T;

template <class T, CONCEPT_REQUIRES(concept::triangular<T>())>
auto product_impl(T, diagonal) -> T;

template <class T,
          CONCEPT_REQUIRES(concept::symmetric<T>() || concept::triangular<T>())>
auto product_impl(matrix_general, T) -> matrix_general;

template <class T,
          CONCEPT_REQUIRES(concept::symmetric<T>() || concept::triangular<T>())>
auto product_impl(T, matrix_general) -> matrix_general;
}
}

template <class A, class B>
using product = decltype(
    detail::structure::product_impl(std::declval<A>(), std::declval<B>()));

///////////////
// transpose //
///////////////

namespace detail {
namespace structure {

inline auto transpose_impl(matrix_general) -> matrix_general;

template <class T, CONCEPT_REQUIRES(concept::symmetric<T>())>
auto transpose_impl(T) -> T;

inline auto transpose_impl(lower_triangular) -> upper_triangular;

inline auto transpose_impl(upper_triangular) -> lower_triangular;
}
}

template <class A>
using transpose =
    decltype(detail::structure::transpose_impl(std::declval<A>()));
}
}
}

///////////
// merge //
///////////

namespace echo {
namespace numeric_array {
namespace structure_traits {

template <class T>
struct merge<linear_algebra::structure::matrix_general, T> {
  using type = linear_algebra::structure::matrix_general;
};

template <>
struct merge<linear_algebra::structure::matrix_general,
             execution_context::structure::general> {
  using type = execution_context::structure::general;
};

template <class T>
struct merge<linear_algebra::structure::diagonal, T> {
  using type = T;
};

template <>
struct merge<linear_algebra::structure::lower_symmetric,
             linear_algebra::structure::upper_symmetric> {
  using type = linear_algebra::structure::lower_symmetric;
};

template <>
struct merge<linear_algebra::structure::lower_triangular,
             linear_algebra::structure::upper_triangular> {
  using type = linear_algebra::structure::matrix_general;
};
}
}
}
