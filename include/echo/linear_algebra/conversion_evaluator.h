#pragma once

#include <echo/linear_algebra/structure.h>
#include <echo/numeric_array.h>

namespace echo {
namespace linear_algebra {

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
