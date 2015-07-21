#include <echo/linear_algebra/least_squares.h>
#include <echo/linear_algebra/matrix.h>
#include <echo/linear_algebra/transpose.h>
#include <echo/intel_execution_context.h>
#include <echo/numeric_array/test.h>
#include <echo/test.h>

using namespace echo;
using namespace echo::linear_algebra;

static const execution_context::intel::ExecutionContext executer{};

TEST_CASE("least_squares") {
  Matrix<double> a1(6, 4), b1(6, 2), a2(4, 6);
  a1 = {{1.44, -7.84, -4.39, 4.53},
        {-9.96, -0.28, -3.24, 3.83},
        {-7.55, 3.24, 6.27, -6.64},
        {8.34, 8.09, 5.28, 2.06},
        {7.08, 2.52, 0.74, -2.47},
        {-5.45, -5.70, -1.19, 4.70}};
  b1 = {{8.58, 9.35},
        {8.26, -4.43},
        {8.48, -0.70},
        {-5.28, -0.26},
        {5.72, -7.36},
        {8.93, -2.52}};
  executer(a2 = transpose(a1));

  Matrix<double> solution1(4, 2);
  solution1 = {{-0.45, 0.25}, {-0.85, -0.90}, {0.71, 0.63}, {0.13, 0.14}};

  Matrix<double> a3(3, 2), b2(3, 1);
  a3 = {{0, 0}, {0, 0}, {0, 0}};
  b2 = {{3}, {1}, {8}};

  SECTION("emplace_compute_least_squares") {
    auto x1 = emplace_compute_least_squares(executer, a1, b1);
    CHECK(x1);
    ARRAY_EQUAL(*x1, solution1, .01);
  }

  SECTION("emplace_compute_least_squares2") {
    auto x1 = emplace_compute_least_squares(executer, transpose(a2), b1);
    CHECK(x1);
    ARRAY_EQUAL(*x1, solution1, .01);
  }

  SECTION("compute_least_squares") {
    auto x1 = compute_least_squares(executer, a1, b1);
    ARRAY_EQUAL(*x1, solution1, .01);
  }

  SECTION("compute_least_squares") {
    auto x2 = compute_least_squares(executer, a3, b2);
    // why doesn't this result in an error?
    // CHECK(!x2);
  }
}
