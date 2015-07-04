#pragma once

#define DETAIL_NS detail_operation

#include <echo/linear_algebra/structure.h>
#include <echo/execution_context.h>

namespace echo {
namespace linear_algebra {

//------------------------------------------------------------------------------
// matrix_operation_t
//------------------------------------------------------------------------------
using execution_context::matrix_operation_t;

//------------------------------------------------------------------------------
// operation_structure
//------------------------------------------------------------------------------
namespace DETAIL_NS {
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

template <matrix_operation_t Operation, class Structure>
using operation_structure =
    typename DETAIL_NS::operation_structure_impl<Operation, Structure>::type;
}
}

#undef DETAIL_NS
