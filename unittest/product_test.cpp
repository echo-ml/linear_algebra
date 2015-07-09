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
using namespace echo::numeric_array;
using namespace echo::linear_algebra;

const execution_context::intel::ExecutionContext executer{};

TEST_CASE("general product") {
  Matrix<double, ShapeC<2, 3>> m1;
  Matrix<double, ShapeC<3, 4>> m2;
  Matrix<double, ShapeC<2, 4>> m3;
  Matrix<double, ShapeC<4, 2>> m4;
  Matrix<double, ShapeC<2, 2>> m5;
  Matrix<double, ShapeC<3, 1>> v1;
  m1 = {{0, 2, 4}, {1, 3, 5}};
  m2 = {{1, 4, 7, 10}, {2, 5, 8, 11}, {3, 6, 9, 12}};
  m3 = {{2, 3, 4, 5}, {7, 8, 9, 10}};
  v1 = {9, 5, 1};

  auto v2 = make_numeric_array_view<linear_algebra::structure::matrix_general>(
      m2.data(), make_subshape(m2.shape(), slice::counted_range(0, 2_index),
                               slice::counted_range(0, 3_index)));

  auto v3 = make_numeric_array_view<linear_algebra::structure::matrix_general>(
      m2.data(), make_subshape(make_dimensionality(3_index, 1_index),
                               make_strides(2_index, 1_index)));

  SECTION("normal emplace product") {
    emplace_product(executer, m1, m2, m3);
    ARRAY_EQUAL(m3, {{16, 34, 52, 70}, {22, 49, 76, 103}});
  }
  SECTION("matrix-vector product") {
    auto v = product(executer, m1, v1);
    ARRAY_EQUAL(v, {{14}, {29}});
  }
  SECTION("matrix-vector product2") {
    auto v = product(executer, m1, v3);
    ARRAY_EQUAL(v, {{26}, {35}});
  }
  SECTION("normal product") {
    auto m = product(executer, m1, m2);
    ARRAY_EQUAL(m, {{16, 34, 52, 70}, {22, 49, 76, 103}});
  }
  SECTION("product add") {
    auto m = product(executer, m1, m2, 1, m3);
    ARRAY_EQUAL(m, {{18, 37, 56, 75}, {29, 57, 85, 113}});
  }
  SECTION("product add2") {
    auto m = product(executer, 2, m1, m2, 1, m3);
    ARRAY_EQUAL(m, {{34, 71, 108, 145}, {51, 106, 161, 216}});
  }
  SECTION("transposed emplace product") {
    emplace_product(executer, transpose(m2), transpose(m1), m4);
    ARRAY_EQUAL(m4, {{16, 22}, {34, 49}, {52, 76}, {70, 103}});
  }
  SECTION("strided emplace product") {
    emplace_product(executer, v2, transpose(m1), m5);
    ARRAY_EQUAL(m5, {{36, 48}, {42, 57}});
  }
}

TEST_CASE("symmetric product") {
  SymmetricMatrix<double, ShapeC<3, 3>> s1;
  Matrix<double, ShapeC<3, 3>> m1, m2;
  Matrix<double, ShapeC<3, 1>> v1;
  s1 = {{1, 2, 3}, {2, 5, 8}, {3, 8, 9}};
  m1 = {{3, 1, 2}, {4, 6, 5}, {7, 8, 12}};
  m2 = {{9, 3, 1}, {9, 5, 6}, {9, 3, 10}};

  v1 = {{1}, {3}, {2}};
  auto v2 = make_numeric_array_view<linear_algebra::structure::matrix_general>(
      m1.data(), make_subshape(make_dimensionality(3_index, 1_index),
                               make_strides(3_index, 1_index)));

  SECTION("left_product") {
    auto m = product(executer, s1, m1);
    ARRAY_EQUAL(m, {{32, 37, 48}, {82, 96, 125}, {104, 123, 154}});
  }
  SECTION("right_product") {
    auto m = product(executer, m1, s1);
    ARRAY_EQUAL(m, {{11, 27, 35}, {31, 78, 105}, {59, 150, 193}});
  }
  SECTION("emplace_product") {
    emplace_product(executer, m1, s1, m2);
    ARRAY_EQUAL(m2, {{11, 27, 35}, {31, 78, 105}, {59, 150, 193}});
  }
  SECTION("symv_product") {
    auto v = product(executer, s1, v1);
    ARRAY_EQUAL(v, {{13}, {33}, {45}});
  }
  SECTION("symv_product2") {
    auto v = product(executer, s1, v2);
    ARRAY_EQUAL(v, {{11}, {27}, {35}});
  }
}
