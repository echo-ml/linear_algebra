#include <echo/linear_algebra/matrix.h>
#include <echo/linear_algebra/vector.h>
#include <echo/linear_algebra/conversion_evaluator.h>
#include <echo/linear_algebra/matrix_operation_expression.h>
#include <echo/linear_algebra/transpose.h>
#include <echo/linear_algebra/product.h>
#include <echo/tbb_expression_executer.h>
#include <echo/intel_execution_context.h>
#include <echo/test.h>
#include <numeric>
using namespace echo;
using namespace echo::linear_algebra;

// const execution_context::intel_tbb::ExpressionExecuter executer{};
const execution_context::intel::ExecutionContext executer{};

TEST_CASE("matrix") {
  Matrix<double, StaticIndex<3>, StaticIndex<2>> m1;  
  m1(0, 0) = 7;
  Matrix<double, KShape<3,2>> m2;  
  m2(0, 0) = 7;

  SymmetricMatrix<float, StaticIndex<5>> s1;
  s1(1, 0) = 12;
}

TEST_CASE("expression") {
  Matrix<double, KShape<2,3>> m1, m2, m3;
  std::iota(all_begin(m1), all_end(m1), 0);
  std::fill(all_begin(m2), all_end(m2), 1);
  auto expr = m3 = m1 + m2;
  executer(expr);
  CHECK(m3(0, 0) == 1);
  CHECK(m3(1, 0) == 2);
  CHECK(m3(0, 1) == 3);
}

TEST_CASE("transpose") {
  Matrix<double, KShape<2,3>> m1, m3;
  Matrix<double, KShape<3,2>> m2;
  std::iota(all_begin(m1), all_end(m1), 0);
  std::iota(all_begin(m2), all_end(m2), 1);
  auto expr = m3 = m1 + transpose(m2);
  executer(expr);
  CHECK(m3(0, 0) == 1);
  CHECK(m3(1, 0) == 5);
}

TEST_CASE("diagonal") {
  Matrix<double, KShape<3,3>> m1, m2;
  std::iota(all_begin(m1), all_end(m1), 0);
  DiagonalMatrix<double, KShape<3>> d1;
  d1 = {1, 2, 3};
  auto expr = m2 = m1 * d1;
  executer(expr);
  CHECK(m2(0, 0) ==  0);
  CHECK(m2(1, 0) ==  0);

  CHECK(m2(1, 1) ==  8);
  CHECK(m2(2, 2) ==  24);
}
