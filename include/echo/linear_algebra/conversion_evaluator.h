#pragma once

#include <echo/linear_algebra/structure.h>
#include <echo/numeric_array.h>
#include <echo/contract.h>

namespace echo {
namespace linear_algebra {

//------------------------------------------------------------------------------
// UpperSymmetricToLowerSymmetricEvaluator
//------------------------------------------------------------------------------
template <class Evaluator>
class UpperSymmetricToLowerSymmetricEvaluator {
 public:
  UpperSymmetricToLowerSymmetricEvaluator(const Evaluator& evaluator)
      : _evaluator(evaluator) {}
  decltype(auto) operator()(index_t i, index_t m, index_t j, index_t n) const {
    CONTRACT_EXPECT {
      CONTRACT_ASSERT(numeric_array::valid_evaluation(i, m, j, n));
      CONTRACT_ASSERT(m == n);
    };
    return _evaluator(j, n, i, m);
  }

 private:
  Evaluator _evaluator;
};

//------------------------------------------------------------------------------
// LowerSymmetricToGeneralEvaluator
//------------------------------------------------------------------------------
template <class Evaluator>
class LowerSymmetricToGeneralEvaluator {
 public:
  LowerSymmetricToGeneralEvaluator(const Evaluator& evaluator)
      : _evaluator(evaluator) {}
  decltype(auto) operator()(index_t i, index_t m, index_t j, index_t n) const {
    CONTRACT_EXPECT {
      CONTRACT_ASSERT(numeric_array::valid_evaluation(i, m, j, n));
      CONTRACT_ASSERT(m == n);
    };
    return _evaluator(std::max(i, j), m, std::min(i, j), m);
  }

 private:
  Evaluator _evaluator;
};

//------------------------------------------------------------------------------
// UpperSymmetricToGeneralEvaluator
//------------------------------------------------------------------------------
template <class Evaluator>
class UpperSymmetricToGeneralEvaluator {
 public:
  UpperSymmetricToGeneralEvaluator(const Evaluator& evaluator)
      : _evaluator(evaluator) {}
  decltype(auto) operator()(index_t i, index_t m, index_t j, index_t n) const {
    CONTRACT_EXPECT {
      CONTRACT_ASSERT(numeric_array::valid_evaluation(i, m, j, n));
      CONTRACT_ASSERT(m == n);
    };
    return _evaluator(std::min(i, j), m, std::max(i, j), m);
  }

 private:
  Evaluator _evaluator;
};

//------------------------------------------------------------------------------
// LowerTriangularToGeneralEvaluator
//------------------------------------------------------------------------------
template <class Evaluator>
class LowerTriangularToGeneralEvaluator {
 public:
  LowerTriangularToGeneralEvaluator(const Evaluator& evaluator)
      : _evaluator(evaluator) {}
  auto operator()(index_t i, index_t m, index_t j, index_t n) const {
    CONTRACT_EXPECT {
      CONTRACT_ASSERT(numeric_array::valid_evaluation(i, m, j, n));
      CONTRACT_ASSERT(m == n);
    };
    return (i < j) ? 0 : _evaluator(i, m, j, n);
  }

 private:
  Evaluator _evaluator;
};

//------------------------------------------------------------------------------
// UpperTriangularToGeneralEvaluator
//------------------------------------------------------------------------------
template <class Evaluator>
class UpperTriangularToGeneralEvaluator {
 public:
  UpperTriangularToGeneralEvaluator(const Evaluator& evaluator)
      : _evaluator(evaluator) {}
  auto operator()(index_t i, index_t m, index_t j, index_t n) const {
    CONTRACT_EXPECT {
      CONTRACT_ASSERT(numeric_array::valid_evaluation(i, m, j, n));
      CONTRACT_ASSERT(m == n);
    };
    return (i > j) ? 0 : _evaluator(i, m, j, n);
  }

 private:
  Evaluator _evaluator;
};

//------------------------------------------------------------------------------
// make_conversion_evaluator
//------------------------------------------------------------------------------
// upper symmetric -> lower symmetric
template <
    int K, class Structure1, class Structure2, class Evaluator,
    CONCEPT_REQUIRES(
        K == 2 && std::is_same<Structure1, structure::upper_symmetric>::value &&
        std::is_same<Structure2, structure::lower_symmetric>::value &&
        execution_context::concept::k_compatible_evaluator<2, Evaluator>())>
auto make_conversion_evaluator(Structure1, Structure2,
                               const Evaluator& evaluator) {
  auto shaped_evaluator = numeric_array::make_k_evaluator<2>(evaluator);
  return UpperSymmetricToLowerSymmetricEvaluator<decltype(shaped_evaluator)>(
      shaped_evaluator);
}

// lower symmetric -> matrix_general
template <
    int K, class Structure1, class Structure2, class Evaluator,
    CONCEPT_REQUIRES(
        K == 2 && std::is_same<Structure1, structure::lower_symmetric>::value &&
        std::is_same<Structure2, structure::matrix_general>::value &&
        execution_context::concept::k_compatible_evaluator<2, Evaluator>())>
auto make_conversion_evaluator(Structure1, Structure2,
                               const Evaluator& evaluator) {
  auto shaped_evaluator = numeric_array::make_k_evaluator<2>(evaluator);
  return LowerSymmetricToGeneralEvaluator<decltype(shaped_evaluator)>(
      shaped_evaluator);
}

// upper symmetric -> matrix_general
template <
    int K, class Structure1, class Structure2, class Evaluator,
    CONCEPT_REQUIRES(
        K == 2 && std::is_same<Structure1, structure::upper_symmetric>::value &&
        std::is_same<Structure2, structure::matrix_general>::value &&
        execution_context::concept::k_compatible_evaluator<2, Evaluator>())>
auto make_conversion_evaluator(Structure1, Structure2,
                               const Evaluator& evaluator) {
  auto shaped_evaluator = numeric_array::make_k_evaluator<2>(evaluator);
  return UpperSymmetricToGeneralEvaluator<decltype(shaped_evaluator)>(
      shaped_evaluator);
}

// lower triangular -> matrix_general
template <
    int K, class Structure1, class Structure2, class Evaluator,
    CONCEPT_REQUIRES(
        K == 2 &&
        std::is_same<Structure1, structure::lower_triangular>::value &&
        std::is_same<Structure2, structure::matrix_general>::value &&
        execution_context::concept::k_compatible_evaluator<2, Evaluator>())>
auto make_conversion_evaluator(Structure1, Structure2,
                               const Evaluator& evaluator) {
  auto shaped_evaluator = numeric_array::make_k_evaluator<2>(evaluator);
  return LowerTriangularToGeneralEvaluator<decltype(shaped_evaluator)>(
      shaped_evaluator);
}

// upper triangular -> matrix_general
template <
    int K, class Structure1, class Structure2, class Evaluator,
    CONCEPT_REQUIRES(
        K == 2 &&
        std::is_same<Structure1, structure::upper_triangular>::value &&
        std::is_same<Structure2, structure::matrix_general>::value &&
        execution_context::concept::k_compatible_evaluator<2, Evaluator>())>
auto make_conversion_evaluator(Structure1, Structure2,
                               const Evaluator& evaluator) {
  auto shaped_evaluator = numeric_array::make_k_evaluator<2>(evaluator);
  return UpperTriangularToGeneralEvaluator<decltype(shaped_evaluator)>(
      shaped_evaluator);
}

// diagonal -> *
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
