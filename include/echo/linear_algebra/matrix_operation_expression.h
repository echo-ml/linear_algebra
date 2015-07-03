#pragma once

#include <echo/linear_algebra/operation_evaluator.h>
#include <echo/numeric_array.h>

// namespace echo {
// namespace linear_algebra {
//
// ///////////////////////////////
// // MatrixOperationExpression //
// ///////////////////////////////
//
// template <class Pointer, class Shape, class Structure, matrix_operation_t Operation>
// class MatrixOperationExpression
//     : Shape,
//       public expression_template::ExpressionTemplateConstAssignment<
//           MatrixOperationExpression<Pointer, Shape, Structure, Operation>,
//           numeric_array::numeric_array_expression_tag,
//           iterator_traits::value_type<Pointer> > {
//  public:
//   using expression_template_tag = numeric_array::numeric_array_expression_tag;
//   using structure = operation_structure<Operation, Structure>;
//   static constexpr matrix_operation_t operation = Operation;
//   MatrixOperationExpression(Pointer data, const Shape& shape)
//       : Shape(shape), _data(data) {}
//   auto data() { return _data; }
//   auto data() const { return _data; }
//   const auto* const_data() const { return _data; }
//   const auto& shape() const { return static_cast<const Shape&>(*this); }
//   auto evaluator() const {
//     auto expression = make_expression(
//         numeric_array::numeric_array_expression_tag(),
//         numeric_array::make_numeric_array_view<Structure>(_data, shape()));
//     return expression.evaluator();
//   }
//
//  private:
//   Pointer _data;
// };
//
// //////////////////////////////////////
// // make_matrix_operation_expression //
// //////////////////////////////////////
//
// template <class Structure, matrix_operation_t Operation, class Pointer, class Shape,
//           CONCEPT_REQUIRES(execution_context::concept::structure<Structure>() &&
//                            echo::concept::contiguous_iterator<Pointer>() &&
//                            k_array::concept::shape<Shape>())>
// auto make_matrix_operation_expression(Pointer data, const Shape& shape) {
//   return MatrixOperationExpression<Pointer, Shape, Structure, Operation>(data,
//                                                                          shape);
// }
// }
//
// namespace numeric_array {
//
// /////////////////////
// // make_expression //
// /////////////////////
//
// template <class Pointer, class Shape, class Structure,
//           linear_algebra::matrix_operation_t Operation>
// auto make_expression(numeric_array_expression_tag,
//                      const linear_algebra::MatrixOperationExpression<
//                          Pointer, Shape, Structure, Operation>& expression) {
//   return make_numeric_array_expression<
//       linear_algebra::operation_structure<Operation, Structure> >(
//       get_extent_shape(expression.shape()), expression.evaluator());
// }
// }
// }
