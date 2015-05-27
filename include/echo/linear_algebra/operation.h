#pragma once

#include <echo/linear_algebra/structure.h>
#include <echo/execution_context.h>

namespace echo {
namespace linear_algebra {

using execution_context::matrix_operation_t;

/////////////////
// matrix_operation_t //
/////////////////

// enum class matrix_operation_t { none, transpose, conjugate_transpose };

/////////////////////////
// operation_structure //
/////////////////////////

namespace detail {
namespace operation {

template <matrix_operation_t Operation, class Structure>
struct operation_structure_impl {};

template <class Structure>
struct operation_structure_impl<matrix_operation_t::none, Structure> {
  using type = Structure;
};

template <class Structure>
struct operation_structure_impl<matrix_operation_t::transpose, Structure> {
  using type = structure::transpose<Structure>;
};
}
}

template <matrix_operation_t Operation, class Structure>
using operation_structure =
    typename detail::operation::operation_structure_impl<Operation,
                                                         Structure>::type;
}
}
