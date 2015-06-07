#pragma once

#include <echo/linear_algebra/structure.h>
#include <echo/numeric_array.h>

namespace echo {
namespace linear_algebra {

/////////////////////////////////////////////
// UpperSymmetricToLowerSymmetricEvaluator //
/////////////////////////////////////////////

template<class Evaluator>
class UpperSymmetricToLowerSymmetricEvaluator {
 public:
  UpperSymmetricToLowerSymmetricEvaluator(const Evaluator& evaluator)
    : _evaluator(evaluator)
  {}
  decltype(auto) operator()(index_t i, index_t m, index_t j, index_t n) const {
    return _evaluator(j, n, i, m);
  }
 private:
  Evaluator _evaluator;
};

//////////////////////////////////////
// LowerSymmetricToGeneralEvaluator //
//////////////////////////////////////

template<class Evaluator>
class LowerSymmetricToGeneralEvaluator {
};

//////////////////////////////////////
// UpperSymmetricToGeneralEvaluator //
//////////////////////////////////////

template<class Evaluator>
class UpperSymmetricToGeneralEvaluator {
};

///////////////////////////////////////
// LowerTriangularToGeneralEvaluator //
///////////////////////////////////////

template<class Evaluator>
class LowerTriangularToGeneralEvaluator {
 public:
  LowerTriangularToGeneralEvaluator(const Evaluator& evaluator)
    : _evaluator(evaluator) {}
  auto operator()(index_t i, index_t m, index_t j, index_t n) const {
    return (i<j) ? 0 : _evaluator(i, m, j, n);
  }
 private:
  Evaluator _evaluator;
};

///////////////////////////////////////
// UpperTriangularToGeneralEvaluator //
///////////////////////////////////////

template<class Evaluator>
class UpperTriangularToGeneralEvaluator {
 public:
  UpperTriangularToGeneralEvaluator(const Evaluator& evaluator)
    : _evaluator(evaluator) {}
  auto operator()(index_t i, index_t m, index_t j, index_t n) const {
    return (i>j) ? 0 : _evaluator(i, m, j, n);
  }
 private:
  Evaluator _evaluator;
};

///////////////////////////////
// make_conversion_evaluator //
///////////////////////////////

// diagonal
template <
    int K, class Structure1, class Structure2, class Evaluator,
    CONCEPT_REQUIRES(
        K == 2 && std::is_same<Structure1, structure::diagonal>::value &&
        execution_context::concept::structure<Structure2>() &&
        execution_context::concept::k_compatible_evaluator<2, Evaluator>())>
auto make_conversion_evaluator(Structure1, Structure2,
                               const Evaluator& evaluator) {
  return evaluator;
}

}
}
