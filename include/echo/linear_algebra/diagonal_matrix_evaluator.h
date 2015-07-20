#pragma once

#include <echo/numeric_array.h>
#include <echo/contract.h>

namespace echo {
namespace linear_algebra {

//------------------------------------------------------------------------------
// DiagonalMatrixEvaluator
//------------------------------------------------------------------------------
template <class Evaluator>
class DiagonalMatrixEvaluator {
 public:
  DiagonalMatrixEvaluator(const Evaluator& evaluator) : _evaluator(evaluator) {}
  decltype(auto) operator()(index_t i, index_t m, index_t j, index_t n) const {
    CONTRACT_EXPECT {
      CONTRACT_ASSERT(numeric_array::valid_evaluation(i, m, j, n));
      CONTRACT_ASSERT(m == n);
    };
    return (i == j) * _evaluator(i);
  }

 private:
  Evaluator _evaluator;
};

//------------------------------------------------------------------------------
// make_diagonal_matrix_evaluator
//------------------------------------------------------------------------------
template <
    class Evaluator,
    CONCEPT_REQUIRES(execution_context::concept::flat_evaluator<Evaluator>())>
auto make_diagonal_matrix_evaluator(const Evaluator& evaluator) {
  return DiagonalMatrixEvaluator<Evaluator>(evaluator);
}
}
}
