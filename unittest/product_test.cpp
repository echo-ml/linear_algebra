#include <echo/numeric_array/test.h>
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
using namespace echo::numeric_array;
using namespace echo::linear_algebra;

const execution_context::intel::ExecutionContext executer{};

TEST_CASE("product") {
  Matrix<double, KShape<2, 3>> m1;
  Matrix<double, KShape<3, 4>> m2;
  Matrix<double, KShape<2, 4>> m3;
  Matrix<double, KShape<4, 2>> m4;
  Matrix<double, KShape<2, 2>> m5;
  std::iota(all_begin(m1), all_end(m1), 0);
  std::iota(all_begin(m2), all_end(m2), 1);

  auto v2 = 
    make_numeric_array_view<linear_algebra::structure::matrix_general>(
        m2.data(),
        slice(m2.shape(), 2_index, 3_index) 
    );
  
  SECTION("normal product") {
    emplace_product(executer, m1, m2, m3);
    ARRAY_EQUAL(m3, {{16, 34, 52, 70}, {22, 49, 76, 103}});
  }
  
  SECTION("transposed product") {
    emplace_product(executer, transpose(m2), transpose(m1), m4);
    ARRAY_EQUAL(m4, {{16, 22}, {34, 49}, {52, 76}, {70, 103}});
  }

  SECTION("strided product") {
    emplace_product(executer, v2, transpose(m1), m5);
    ARRAY_EQUAL(m5, {{36, 48}, {42, 57}});
  }

}
