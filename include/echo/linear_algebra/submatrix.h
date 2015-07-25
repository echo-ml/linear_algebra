#pragma once

#include <echo/linear_algebra/concept.h>
#include <echo/linear_algebra/matrix.h>
#include <echo/contract.h>

namespace echo {
namespace linear_algebra {

//------------------------------------------------------------------------------
// make_submatrix
//------------------------------------------------------------------------------
template <class RowSlice, class ColumnSlice, class A,
          CONCEPT_REQUIRES(concept::general_matrix<uncvref_t<A>>() &&
                           numeric_array::concept::numeric_subarray<
                               uncvref_t<A>, RowSlice, ColumnSlice>())>
auto make_submatrix(A&& a, const RowSlice& row_slice,
                    const ColumnSlice& column_slice) {
  return make_numeric_subarray(std::forward<A>(a), row_slice, column_slice);
}
}
}
