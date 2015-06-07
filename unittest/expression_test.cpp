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

TEST_CASE("non-general expression") {
  SymmetricMatrix<double, KShape<2,2>> s1, s2;
  s1 = {{3, 7}, {7, 9}};
  executer(s2 = s1+5);
  ARRAY_EQUAL(s2, {{8, 12}, {12, 14}});
}

TEST_CASE("conversions") {
  SymmetricMatrix<double, KShape<2,2>> s1;
  LowerTriangularMatrix<double, KShape<2,2>> lt1;
  UpperTriangularMatrix<double, KShape<2,2>> ut1;
  Matrix<double, KShape<2,2>> m1, m2;

  s1 = {{3, 7}, {7, 9}};
  lt1 = {{9, 0}, {4, 8}};
  ut1 = {{5, 2}, {0, 7}};
  m1 = {{4, 5},{8, 9}};

  SECTION("symmetric -> general") {
    executer(m2 = m1 + s1);
    ARRAY_EQUAL(m2, {{7, 12}, {15, 18}});
  }
  SECTION("triangular -> general") {
    executer(m2 = lt1 + ut1);
    ARRAY_EQUAL(m2, {{14, 2}, {4, 15}});
  }
}
