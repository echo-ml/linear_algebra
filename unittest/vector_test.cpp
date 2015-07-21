#include <echo/linear_algebra.h>
#include <echo/numeric_array/test.h>
#include <echo/test.h>

using namespace echo;
using namespace echo::linear_algebra;

TEST_CASE("vector_construction") {
  Vector<double> v1;
  Vector<double, StaticIndex<7>> v2;
  Vector<double, ShapeC<3>> v3;

  RowVector<double> v4;
  RowVector<double, index_t> v5;
  RowVector<double, ShapeC<1, 3>> v6;

  ColumnVector<double> v7;
  ColumnVector<double, StaticIndex<5>> v8;
  ColumnVector<double, ShapeC<3, 1>> v9;
}

TEST_CASE("subvector") {
  Matrix<double, ShapeC<2, 3>> m1;
  m1 = {{1, 2, 3}, {4, 5, 6}};

  ARRAY_EQUAL(get_row(1, m1), {4, 5, 6});
  ARRAY_EQUAL(get_row_vector(1, m1), {{4, 5, 6}});

  ARRAY_EQUAL(get_column(0, m1), {1, 4});
  ARRAY_EQUAL(get_column_vector(1, m1), {{2}, {5}});
}
