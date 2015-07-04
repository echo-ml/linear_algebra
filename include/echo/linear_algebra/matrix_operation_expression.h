#pragma once

#include <echo/linear_algebra/operation_evaluator.h>
#include <echo/linear_algebra/matrix_operation_expression_fwd.h>
#include <echo/numeric_array.h>

namespace echo {
namespace linear_algebra {

//------------------------------------------------------------------------------
// MatrixOperationExpression
//------------------------------------------------------------------------------
template <class Pointer, class Dimensionality, class Structure,
          matrix_operation_t Operation>
class MatrixOperationExpression
    : htl::Pack<Dimensionality>,
      public expression_template::ExpressionTemplateConstAssignment<
          MatrixOperationExpression<Pointer, Dimensionality, Structure,
                                    Operation>,
          numeric_array::numeric_array_expression_tag,
          iterator_traits::value_type<Pointer> > {
 public:
  using expression_template_tag = numeric_array::numeric_array_expression_tag;
  using structure = operation_structure<Operation, Structure>;
  static constexpr matrix_operation_t operation = Operation;
  MatrixOperationExpression(Pointer data, const Dimensionality& dimensionality)
      : Dimensionality(dimensionality), _data(data) {}
  auto data() { return _data; }
  auto data() const { return _data; }
  const auto* const_data() const { return _data; }
  const auto& dimensionality() const {
    return htl::unpack<Dimensionality>(*this);
  }
  auto evaluator() const {
    auto expression =
        make_expression(numeric_array::numeric_array_expression_tag(),
                        numeric_array::make_numeric_array_view<Structure>(
                            _data, dimensionality()));
    return expression.evaluator();
  }

 private:
  Pointer _data;
};

//------------------------------------------------------------------------------
// make_matrix_operation_expression
//------------------------------------------------------------------------------
template <
    class Structure, matrix_operation_t Operation, class Pointer,
    class Dimensionality,
    CONCEPT_REQUIRES(execution_context::concept::structure<Structure>() &&
                     echo::concept::contiguous_iterator<Pointer>() &&
                     k_array::concept::dimensionality<2, Dimensionality>())>
auto make_matrix_operation_expression(Pointer data,
                                      const Dimensionality& dimensionality) {
  return MatrixOperationExpression<Pointer, Dimensionality, Structure,
                                   Operation>(data, dimensionality);
}
}

namespace numeric_array {

//------------------------------------------------------------------------------
// make_expression
//------------------------------------------------------------------------------
template <class Pointer, class Dimensionality, class Structure,
          linear_algebra::matrix_operation_t Operation>
auto make_expression(
    numeric_array_expression_tag,
    const linear_algebra::MatrixOperationExpression<
        Pointer, Dimensionality, Structure, Operation>& expression) {
  return make_numeric_array_expression<
      linear_algebra::operation_structure<Operation, Structure> >(
      expression.dimensionality(), expression.evaluator());
}
}
}
