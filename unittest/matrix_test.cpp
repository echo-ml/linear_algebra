#include <echo/linear_algebra/matrix.h>
#include <echo/linear_algebra/vector.h>
#include <echo/linear_algebra/conversion_evaluator.h>
#include <echo/linear_algebra/matrix_operation_expression.h>
#include <echo/linear_algebra/transpose.h>
#include <echo/linear_algebra/product.h>
#include <echo/numeric_array/test.h>
#include <echo/tbb_expression_executer.h>
#include <echo/intel_execution_context.h>
#include <echo/test.h>
#include <numeric>
using namespace echo;
using namespace echo::linear_algebra;

const execution_context::intel::ExecutionContext executer{};

TEST_CASE("matrix") {
  Matrix<double, StaticIndex<3>, StaticIndex<2>> m1;
  m1(0, 0) = 7;

  Matrix<double, Shape<StaticIndex<3>, StaticIndex<2>>> m2;
  m2(0, 0) = 7;

  SymmetricMatrix<float, StaticIndex<5>> s1;
  s1(1, 0) = 12;
}

TEST_CASE("vector") {
  SECTION("row_vector") {
    RowVector<double, StaticIndex<3>> v1;
    type_equal<decltype(v1),
               NumericArray<double, ShapeC<1, 3>,
                            linear_algebra::structure::matrix_general>>();
    RowVector<double, ShapeC<1, 3>> v2;
    type_equal<decltype(v2), decltype(v1)>();
  }
  SECTION("column_vector") {
    ColumnVector<double, StaticIndex<3>> v1;
    type_equal<decltype(v1),
               NumericArray<double, ShapeC<3, 1>,
                            linear_algebra::structure::matrix_general>>();
    ColumnVector<double, ShapeC<3, 1>> v2;
    type_equal<decltype(v2), decltype(v1)>();
  }
}

TEST_CASE("expression") {
  Matrix<double, ShapeC<2, 3>> m1, m2, m3;
  std::iota(all_begin(m1), all_end(m1), 0);
  std::fill(all_begin(m2), all_end(m2), 1);
  auto expr = m3 = m1 + m2;
  executer(expr);
  CHECK(m3(0, 0) == 1);
  CHECK(m3(1, 0) == 2);
  CHECK(m3(0, 1) == 3);
}

TEST_CASE("transpose") {
  Matrix<double, ShapeC<2, 3>> m1, m3;
  Matrix<double, ShapeC<3, 2>> m2;
  std::iota(all_begin(m1), all_end(m1), 0);
  std::iota(all_begin(m2), all_end(m2), 1);
  auto expr = m3 = m1 + transpose(m2);
  executer(expr);
  CHECK(m3(0, 0) == 1);
  CHECK(m3(1, 0) == 5);
}

TEST_CASE("diagonal") {
  Matrix<double, ShapeC<3, 3>> m1, m2;
  std::iota(all_begin(m1), all_end(m1), 0);
  DiagonalMatrix<double, ShapeC<3>> d1;
  d1 = {1, 2, 3};
  auto expr = m2 = m1 * d1;
  executer(expr);
  CHECK(m2(0, 0) == 0);
  CHECK(m2(1, 0) == 0);

  CHECK(m2(1, 1) == 8);
  CHECK(m2(2, 2) == 24);
}

TEST_CASE("symmetric_initialization") {
  SymmetricMatrix<double, ShapeC<2, 2>> m;

  SECTION("valid initialization") {
    m = {{1, 2}, {2, 1}};
    ARRAY_EQUAL(m, {{1, 2}, {2, 1}});
  }
  SECTION("invalid initialization") {
    CHECK_THROWS_AS((m = {{1, 3}, {2, 1}}), numeric_array::InitializationError);
  }
}

TEST_CASE("triangular_initialization") {
  LowerTriangularMatrix<double, ShapeC<2, 2>> lt;
  UpperTriangularMatrix<double, ShapeC<2, 2>> ut;

  SECTION("lower_triangular valid initialization") {
    lt = {{1, 0}, {3, 4}};
    ARRAY_EQUAL(lt, {{1, 0}, {3, 4}});
  }
  SECTION("lower_triangular invalid initialization") {
    CHECK_THROWS_AS((lt = {{1, 7}, {3, 4}}),
                    numeric_array::InitializationError);
  }
  SECTION("upper_triangular valid initialization") {
    ut = {{1, 7}, {0, 4}};
    ARRAY_EQUAL(ut, {{1, 7}, {0, 4}});
  }
  SECTION("upper_triangular invalid initialization") {
    CHECK_THROWS_AS((ut = {{1, 7}, {3, 4}}),
                    numeric_array::InitializationError);
  }
}
