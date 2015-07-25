#include <echo/linear_algebra.h>
#include <echo/numeric_array/test.h>
#include <echo/test.h>

using namespace echo;
using namespace echo::linear_algebra;

TEST_CASE("make_submatrix") {
  Matrix<double> m1(2, 3);
  m1 = {{1, 2, 3}, {4, 5, 6}};
  auto s1 = make_submatrix(m1, slice::all, slice::counted_range(1, 2));
  ARRAY_EQUAL(s1, {{2, 3}, {5, 6}});
}
