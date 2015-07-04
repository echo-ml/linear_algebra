#pragma once

#include <echo/linear_algebra/operation.h>

namespace echo {
namespace linear_algebra {

//------------------------------------------------------------------------------
// MatrixOperationExpression
//------------------------------------------------------------------------------

template <class Pointer, class Dimensionality, class Structure,
          matrix_operation_t Operation>
class MatrixOperationExpression;
}
}
