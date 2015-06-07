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

TEST_CASE("general arithmetic") {
  Matrix<double, KShape<2, 3>> m1, m2, m3;
  m1 = {{1, 2, 3}, {4, 5, 6}};
  m2 = {{7, 1, 8}, {9, 11, 7}};
  executer(m3 = m1 + m2);

  ARRAY_EQUAL(m3, {{8, 3, 11}, {13, 16, 13}});
}

TEST_CASE("general structures") {
  Matrix<double, KShape<2, 3>> m1, m2, m3;
  m1 = {{1, 2, 3}, {4, 5, 6}};

  auto expr = m1 + numeric_array::map_indexes([](index_t i, index_t j) {
    return (i + 1) * (j + 1);
  }, 2_index, 3_index);
  executer(m2 = expr);
  ARRAY_EQUAL(m2, {{2, 4, 6}, {6, 9, 12}});
}
