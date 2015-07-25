#pragma once

#include <echo/linear_algebra/matrix.h>
#include <echo/linear_algebra/vector.h>
#include <echo/linear_algebra/subvector.h>
#include <echo/linear_algebra/submatrix.h>
#include <echo/linear_algebra/product.h>
#include <echo/linear_algebra/solve.h>
#include <echo/linear_algebra/transpose.h>

namespace echo {

using echo::linear_algebra::Matrix;
using echo::linear_algebra::SymmetricMatrix;
using echo::linear_algebra::LowerTriangularMatrix;
using echo::linear_algebra::UpperTriangularMatrix;

using echo::linear_algebra::Vector;
using echo::linear_algebra::RowVector;
using echo::linear_algebra::ColumnVector;

using echo::linear_algebra::get_num_rows;
using echo::linear_algebra::get_num_columns;

using echo::linear_algebra::get_row;
using echo::linear_algebra::get_row_vector;

using echo::linear_algebra::get_column;
using echo::linear_algebra::get_column_vector;

using echo::linear_algebra::make_submatrix;

}
