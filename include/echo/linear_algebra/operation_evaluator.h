#pragma once

#include <echo/linear_algebra/operation.h>
#include <echo/numeric_array.h>
#include <echo/contract.h>

namespace echo {
namespace linear_algebra {

//------------------------------------------------------------------------------
// OperationEvaluator
//------------------------------------------------------------------------------
template <class Evaluator, matrix_operation_t Operation>
class OperationEvaluator {};

template <class Evaluator>
class OperationEvaluator<Evaluator, matrix_operation_t::transpose> {
 public:
  OperationEvaluator(const Evaluator& evaluator) : _evaluator(evaluator) {}
  decltype(auto) operator()(index_t i, index_t num_rows, index_t j,
                            index_t num_columns) const {
    CONTRACT_EXPECT {
      CONTRACT_ASSERT(
          numeric_array::valid_evaluation(i, num_rows, j, num_columns));
    };
    return _evaluator(j, num_columns, i, num_rows);
  }

 private:
  Evaluator _evaluator;
};

//------------------------------------------------------------------------------
// make_operation_evaluator
//------------------------------------------------------------------------------
template <
    matrix_operation_t Operation, class Evaluator,
    CONCEPT_REQUIRES(execution_context::concept::k_evaluator<2, Evaluator>())>
auto make_operation_evaluator(const Evaluator& evaluator) {
  return OperationEvaluator<Evaluator, Operation>(evaluator);
}
}
}
