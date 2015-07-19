#include <echo/linear_algebra/cholesky.h>
#include <echo/linear_algebra/matrix.h>
#include <echo/intel_execution_context.h>
#include <echo/numeric_array/test.h>
#include <echo/test.h>

using namespace echo;
using namespace echo::linear_algebra;

static const execution_context::intel::ExecutionContext executer{};

TEST_CASE("potrf") {
  SymmetricMatrix<double> s1(3);
  LowerTriangularMatrix<double> r1(3);

  s1 = {{2, -1, 0}, {-1, 2, -1}, {0, -1, 2}};

  SECTION("emplace_cholesky_factorize1") {
    auto result = emplace_cholesky_factorize(executer, s1);
    CHECK(result);
    ARRAY_EQUAL(
        *result,
        {{1.41421, 0., 0.}, {-0.707107, 1.22474, 0.}, {0., -0.816497, 1.1547}},
        .01);
  }
  SECTION("emplace_cholesky_factorize1") {
    auto result = emplace_cholesky_factorize(executer, s1, r1);
    CHECK(result);
    ARRAY_EQUAL(
        r1,
        {{1.41421, 0., 0.}, {-0.707107, 1.22474, 0.}, {0., -0.816497, 1.1547}},
        .01);
  }
  SECTION("emplace_cholesky_factorize1") {
    auto result = emplace_cholesky_factorize(executer, 1.0, s1, r1);
    CHECK(result);
    ARRAY_EQUAL(
        r1,
        {{1.41421, 0., 0.}, {-0.707107, 1.22474, 0.}, {0., -0.816497, 1.1547}},
        .01);
  }
}
