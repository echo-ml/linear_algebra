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

  Matrix<double, ShapeC<3,3>> m2;
  m2 = {{1,2,3}, {4,5,6}, {7,8,9}};

  ARRAY_EQUAL(get_row(m1, 1), {4, 5, 6});
  ARRAY_EQUAL(get_row_vector(m1, 1), {{4, 5, 6}});

  ARRAY_EQUAL(get_column(m1, 0), {1, 4});
  ARRAY_EQUAL(get_column_vector(m1, 1), {{2}, {5}});

  auto d1 = get_diagonal(m2);
  ARRAY_EQUAL(d1, {1, 5, 9});

  auto d2 = get_diagonal_vector(m2);
  ARRAY_EQUAL(d2, {1, 5, 9});
}
