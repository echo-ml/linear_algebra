#include <echo/linear_algebra/cholesky.h>
#include <echo/linear_algebra/matrix.h>
#include <echo/intel_execution_context.h>
#include <echo/test.h>

using namespace echo;
using namespace echo::linear_algebra;

static const execution_context::intel::ExecutionContext executer{};

TEST_CASE("potrf") {
  SymmetricMatrix<double> s1(3);

  s1 = {
    {2, -1, 0},
    {-1, 2, -1},
    {0, -1, 2}};

  auto result = emplace_cholesky_factorize(executer, s1);

  // LapackBackend lapack_backend;

  // std::cout << MatrixForm() << m1;
  // std::cout << '\n';
  // auto v1 = cholesky_factorize(lapack_backend, m1);
  // REQUIRE(bool(v1));
  // std::cout << MatrixForm() << v1;

  // auto m1_inverse = cholesky_invert(lapack_backend, *v1);
  // std::cout << MatrixForm() << m1_inverse << '\n';
}
