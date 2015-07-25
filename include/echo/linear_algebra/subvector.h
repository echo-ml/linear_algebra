#pragma once

#include <echo/linear_algebra/concept.h>
#include <echo/linear_algebra/matrix.h>
#include <echo/contract.h>

namespace echo {
namespace linear_algebra {

//------------------------------------------------------------------------------
// get_row
//------------------------------------------------------------------------------
template <class A, CONCEPT_REQUIRES(concept::general_matrix<uncvref_t<A>>())>
auto get_row(index_t i, A&& a) {
  CONTRACT_EXPECT { CONTRACT_ASSERT(0 <= i && i < get_num_rows(a)); };
  return make_numeric_subarray(a, i, slice::all);
}

//------------------------------------------------------------------------------
// get_row_vector
//------------------------------------------------------------------------------
template <class A, CONCEPT_REQUIRES(concept::general_matrix<uncvref_t<A>>())>
auto get_row_vector(index_t i, A&& a) {
  CONTRACT_EXPECT { CONTRACT_ASSERT(0 <= i && i < get_num_rows(a)); };
  return make_numeric_subarray(a, slice::counted_range(i, 1_index), slice::all);
}

//------------------------------------------------------------------------------
// get_column
//------------------------------------------------------------------------------
template <class A, CONCEPT_REQUIRES(concept::general_matrix<uncvref_t<A>>())>
auto get_column(index_t i, A&& a) {
  CONTRACT_EXPECT { CONTRACT_ASSERT(0 <= i && i < get_num_columns(a)); };
  return make_numeric_subarray(a, slice::all, i);
}

//------------------------------------------------------------------------------
// get_column_vector
//------------------------------------------------------------------------------
template <class A, CONCEPT_REQUIRES(concept::general_matrix<uncvref_t<A>>())>
auto get_column_vector(index_t i, A&& a) {
  CONTRACT_EXPECT { CONTRACT_ASSERT(0 <= i && i < get_num_columns(a)); };
  return make_numeric_subarray(a, slice::all, slice::counted_range(i, 1_index));
}

//------------------------------------------------------------------------------
// get_diagonal
//------------------------------------------------------------------------------
template <class A,
          CONCEPT_REQUIRES(concept::square_compatible_matrix<uncvref_t<A>>())>
auto get_diagonal(A&& a) {
  CONTRACT_EXPECT { CONTRACT_ASSERT(get_extent<0>(a) == get_extent<1>(a)); };
  return make_numeric_array_view<
      structure::general,
      numeric_array_traits::memory_backend_tag<uncvref_t<A>>>(
      a.data(),
      make_subshape(make_dimensionality(get_extent<0>(a)),
                    make_strides(get_stride<0>(a) + get_stride<1>(a))));
}

//------------------------------------------------------------------------------
// get_diagonal_vector
//------------------------------------------------------------------------------
template <class A,
          CONCEPT_REQUIRES(concept::square_compatible_matrix<uncvref_t<A>>())>
auto get_diagonal_vector(A&& a) {
  CONTRACT_EXPECT { CONTRACT_ASSERT(get_extent<0>(a) == get_extent<1>(a)); };
  return make_numeric_array_view<
      structure::diagonal,
      numeric_array_traits::memory_backend_tag<uncvref_t<A>>>(
      a.data(),
      make_subshape(make_dimensionality(get_extent<0>(a)),
                    make_strides(get_stride<0>(a) + get_stride<1>(a))));
}
}
}
