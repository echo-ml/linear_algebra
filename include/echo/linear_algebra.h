#pragma once

#include <echo/linear_algebra/matrix.h>
#include <echo/linear_algebra/vector.h>
#include <echo/linear_algebra/subvector.h>
#include <echo/linear_algebra/submatrix.h>
#include <echo/linear_algebra/product.h>
#include <echo/linear_algebra/least_squares.h>
#include <echo/linear_algebra/singular_value_decomposition.h>
#include <echo/linear_algebra/solve.h>
#include <echo/linear_algebra/transpose.h>

namespace echo {

using echo::linear_algebra::Matrix;
using echo::linear_algebra::SymmetricMatrix;
using echo::linear_algebra::LowerTriangularMatrix;
using echo::linear_algebra::UpperTriangularMatrix;

using echo::linear_algebra::StaticMatrix;
using echo::linear_algebra::StaticSymmetricMatrix;
using echo::linear_algebra::StaticLowerTriangularMatrix;
using echo::linear_algebra::StaticUpperTriangularMatrix;

using echo::linear_algebra::Vector;
using echo::linear_algebra::RowVector;
using echo::linear_algebra::ColumnVector;

using echo::linear_algebra::StaticVector;
using echo::linear_algebra::StaticRowVector;
using echo::linear_algebra::StaticColumnVector;

using echo::linear_algebra::get_num_rows;
using echo::linear_algebra::get_num_columns;

using echo::linear_algebra::get_row;
using echo::linear_algebra::get_row_vector;

using echo::linear_algebra::get_column;
using echo::linear_algebra::get_column_vector;

using echo::linear_algebra::make_matrix;
using echo::linear_algebra::make_submatrix;

using linear_algebra::make_matrix;
using linear_algebra::make_static_matrix;

using linear_algebra::make_vector;
using linear_algebra::make_static_vector;
using linear_algebra::make_row_vector;
using linear_algebra::make_static_row_vector;
using linear_algebra::make_column_vector;
using linear_algebra::make_static_column_vector;

using linear_algebra::make_vector_view;
using linear_algebra::make_vector_cview;

using linear_algebra::make_row_vector_view;
using linear_algebra::make_row_vector_cview;

using linear_algebra::make_column_vector_view;
using linear_algebra::make_column_vector_cview;
}
