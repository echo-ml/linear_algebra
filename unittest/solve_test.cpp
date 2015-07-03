#include <echo/linear_algebra/solve.h>
#include <echo/linear_algebra/matrix.h>
#include <echo/linear_algebra/vector.h>
#include <echo/linear_algebra/transpose.h>
#include <echo/numeric_array/test.h>
#include <echo/intel_execution_context.h>
#include <echo/test.h>

using namespace echo;
using namespace echo::numeric_array;
using namespace echo::linear_algebra;

const execution_context::intel::ExecutionContext executer{};

// TEST_CASE("solve") {
//   Matrix<double, KShape<3, 2>> m1;
//   m1 = {{1, 2}, {3, 4}, {5, 6}};
//   LowerTriangularMatrix<double, KShape<3, 3>> t1;
//   t1 = {{9, 0, 0}, {1, 2, 0}, {3, 8, 1}};
//
//   Matrix<double, KShape<3, 1>> v1;
//   v1 = {{2}, {7}, {4}};
//
//   SECTION("emplace_left_solve") {
//     emplace_left_solve(executer, t1, m1);
//     ARRAY_EQUAL(
//         m1, {{0.111111, 0.222222}, {1.44444, 1.88889}, {-6.88889, -9.77778}},
//         .01);
//   }
//
//   SECTION("left_solve") {
//     auto m = left_solve(executer, t1, m1);
//     ARRAY_EQUAL(
//         m, {{0.111111, 0.222222}, {1.44444, 1.88889}, {-6.88889, -9.77778}},
//         .01);
//   }
//
//   SECTION("vector solve") {
//     auto v = left_solve(executer, t1, v1);
//     ARRAY_EQUAL(v, {{0.222222}, {3.38889}, {-23.7778}}, .01);
//   }
//
//   SECTION("transpose emplace_left_solve") {
//     emplace_left_solve(executer, transpose(t1), m1);
//     ARRAY_EQUAL(m1, {{0.5, 0.666667}, {-18.5, -22.}, {5., 6.}}, .01);
//   }
//
//   SECTION("multiplier emplace_left_solve") {
//     emplace_left_solve(executer, t1, 2, m1);
//     ARRAY_EQUAL(
//         m1, {{0.222222, 0.444444}, {2.88889, 3.77778}, {-13.7778, -19.5556}},
//         .01);
//   }
//
//   SECTION("multiplier left_solve") {
//     auto m = left_solve(executer, t1, 2, m1);
//     ARRAY_EQUAL(
//         m, {{0.222222, 0.444444}, {2.88889, 3.77778}, {-13.7778, -19.5556}},
//         .01);
//   }
// }
