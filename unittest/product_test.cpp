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

TEST_CASE("product") {
  Matrix<double, KShape<2, 3>> m1;
  Matrix<double, KShape<3, 4>> m2;
  Matrix<double, KShape<2, 4>> m3;
  std::iota(all_begin(m1), all_end(m1), 0);
  std::iota(all_begin(m2), all_end(m2), 1);
  emplace_product(executer, m1, m2, m3);
}
