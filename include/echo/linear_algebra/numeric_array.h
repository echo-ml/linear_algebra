#pragma once
//
// #include <echo/numeric_array.h>
// #include <echo/linear_algebra/structure.h>
//
// namespace echo {
// namespace linear_algebra {
//
// namespace detail {
// namespace numeric_array {
//
// template <class Scalar, class Shape, class Structure, class Allocator>
// class MatrixBase
//     : public KArray<Scalar, Shape, Allocator>,
//       public expression_template::ExpressionTemplateAssignment<
//           Matrix<Scalar, Shape, Structure, Allocator>, Structure, Scalar>,
//       public KArrayAssignment<Matrix<Scalar, Shape, Structure, Allocator>,
//                               Scalar> {
//   using BaseKArray = KArray<Scalar, Shape, Allocator>;
//   using AssignmentBase =
//       KArrayAssignment<Matrix<Scalar, Shape, Structure, Allocator>, Scalar>;
//   using BaseExpressionTemplateAssignment =
//       expression_template::ExpressionTemplateAssignment<
//           Matrix<Scalar, Shape, Structure, Allocator>, Structure, Scalar>;
//
//  public:
//   using structure = Structure;
//
//   explicit MatrixBase(const Shape& shape,
//                       const Allocator& allocator = Allocator())
//       : BaseKArray(shape, allocator) {}
//
//   MatrixBase(shape_traits::extent_type<0, Shape> extent1,
//              shape_traits::extent_type<1, Shape> extent2,
//              const Allocator& allocator = Allocator())
//       : BaseKArray(Shape(extent1, extent2), allocator) {}
//
//   MatrixBase() {}
// };
//
// template <class Scalar, class Shape, class Structure, class Allocator>
// class SquareMatrixBase
//     : public MatrixBase<Scalar, Shape, Structure, Allocator> {
//  public:
//   using structure = Structure;
//
//   explicit SquareMatrixBase(const Shape& shape,
//                             const Allocator& allocator = Allocator())
//       : BaseKArray(shape, allocator) {}
//
//   explicit SquareMatrixBase(shape_traits::extent_type<0, Shape> extent,
//                             const Allocator& allocator = Allocator())
//       : BaseKArray(Shape(extent, extent), allocator) {}
//
//   SquareMatrixBase() {}
// };
// }
// }
// }
// }
//
// //////////////////
// // NumericArray //
// //////////////////
//
// namespace echo {
// namespace numeric_array {
//
// template <class Scalar, class Shape, class Allocator>
// class NumericArray<Scalar, Shape, linear_algebra::structure::general,
//                    Allocator> {};
// }
// }
