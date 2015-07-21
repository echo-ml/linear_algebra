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
}
}
