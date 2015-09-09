#include <echo/linear_algebra/singular_value_decomposition.h>
#include <echo/linear_algebra/product.h>
#include <echo/linear_algebra/matrix.h>
#include <echo/linear_algebra/transpose.h>
#include <echo/intel_execution_context.h>
#include <echo/numeric_array/test.h>
#include <echo/test.h>

using namespace echo;
using namespace echo::linear_algebra;

static const execution_context::intel::ExecutionContext executer{};

TEST_CASE("singular_value_decomposition1") {
  auto a = make_matrix<double>(6_index, 5_index);
  a = {{8.79, 9.93, 9.83, 5.45, 3.16},
       {6.11, 6.91, 5.04, -0.27, 7.98},
       {-9.15, -7.93, 4.86, 4.85, 3.01},
       {9.57, 1.64, 8.83, 0.74, 5.80},
       {-3.49, 4.02, 9.80, 10.00, 4.27},
       {9.84, 0.15, -8.99, -6.02, -5.31}};
  auto svd = singular_value_decompose(executer, a);
  const auto& u = std::get<0>(svd);
  const auto& s = std::get<1>(svd);
  const auto& vt = std::get<2>(svd);

  auto sigma = make_matrix<double>(5_index, 5_index);
  auto diagonal_expression =
      [&](index_t i, index_t j) -> double { return i == j ? s(i) : 0; };
  executer(sigma = numeric_array::map_indexes(diagonal_expression, 5_index,
                                              5_index));
  auto a_prime = product(executer, u, product(executer, sigma, vt));
  ARRAY_EQUAL(a, a_prime, 0.01);
}

TEST_CASE("singular_value_decomposition2") {
  auto a = make_matrix<double>(6_index, 5_index);
  a = {{8.79, 9.93, 9.83, 5.45, 3.16},
       {6.11, 6.91, 5.04, -0.27, 7.98},
       {-9.15, -7.93, 4.86, 4.85, 3.01},
       {9.57, 1.64, 8.83, 0.74, 5.80},
       {-3.49, 4.02, 9.80, 10.00, 4.27},
       {9.84, 0.15, -8.99, -6.02, -5.31}};
  auto at = make_matrix<double>(5_index, 6_index);
  executer(at = transpose(a));
  auto svd = singular_value_decompose(executer, at);
  const auto& u = std::get<0>(svd);
  const auto& s = std::get<1>(svd);
  const auto& vt = std::get<2>(svd);

  auto sigma = make_matrix<double>(5_index, 5_index);
  auto diagonal_expression =
      [&](index_t i, index_t j) -> double { return i == j ? s(i) : 0; };
  executer(sigma = numeric_array::map_indexes(diagonal_expression, 5_index,
                                              5_index));
  auto at_prime = product(executer, u, product(executer, sigma, vt));
  ARRAY_EQUAL(at, at_prime, 0.01);
}
