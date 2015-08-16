#include <echo/linear_algebra.h>
#include <echo/numeric_array/test.h>
#include <echo/test.h>

using namespace echo;
using namespace echo::linear_algebra;

TEST_CASE("vector_construction") {
  Vector<double> v1;
  Vector<double, StaticIndex<7>> v2;
  Vector<double, ShapeC<3>> v3;

  RowVector<double> v4;
  RowVector<double, index_t> v5;
  RowVector<double, ShapeC<1, 3>> v6;

  ColumnVector<double> v7;
  ColumnVector<double, StaticIndex<5>> v8;
  ColumnVector<double, ShapeC<3, 1>> v9;
}

TEST_CASE("subvector") {
  Matrix<double, ShapeC<2, 3>> m1;
  m1 = {{1, 2, 3}, {4, 5, 6}};

  Matrix<double, ShapeC<3, 3>> m2;
  m2 = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};

  ARRAY_EQUAL(get_row(m1, 1), {4, 5, 6});
  ARRAY_EQUAL(get_row_vector(m1, 1), {{4, 5, 6}});

  ARRAY_EQUAL(get_column(m1, 0), {1, 4});
  ARRAY_EQUAL(get_column_vector(m1, 1), {{2}, {5}});

  auto d1 = get_diagonal(m2);
  ARRAY_EQUAL(d1, {1, 5, 9});

  auto d2 = get_diagonal_vector(m2);
  ARRAY_EQUAL(d2, {1, 5, 9});
}

TEST_CASE("make_vector") {
  auto d1 = make_vector<double, linear_algebra::structure::diagonal>(3_index);
  auto v1 = make_vector<double>(3_index);
  auto r1 = make_row_vector<double>(3);
  auto c1 = make_column_vector<double>(2_index);

  auto view1 = make_vector_view(v1);
  auto view2 = make_vector_cview(r1);

  auto rview1 = make_row_vector_view(v1);
  auto rview2 = make_row_vector_cview(c1);
}

TEST_CASE("make_vector_view") {
  auto v1 = make_vector<double>(6_index);
  v1 = {1, 2, 3, 4, 5, 6};
  auto c1 = make_column_vector<double>(6_index);
  c1 = {1, 2, 3, 4, 5, 6};
  auto r1 = make_row_vector<double>(6_index);
  r1 = {1, 2, 3, 4, 5, 6};

  auto v2 = make_numeric_array_view<decltype(v1)::structure>(
      v1.data(),
      make_subshape(make_dimensionality(3_index), make_strides(2_index)));
  auto c2 = make_numeric_array_view<decltype(c1)::structure>(
      c1.data(), make_subshape(make_dimensionality(3_index, 1_index),
                               make_strides(2_index, 6_index)));
  auto r2 = make_numeric_array_view<decltype(r1)::structure>(
      r1.data(), make_subshape(make_dimensionality(1_index, 3_index),
                               make_strides(1_index, 2_index)));
  SECTION("make_vector_view") {
    auto v3 = make_vector_view(v1);
    ARRAY_EQUAL(v3, {1, 2, 3, 4, 5, 6});
    auto v4 = make_vector_view(v2);
    ARRAY_EQUAL(v4, {1, 3, 5});

    auto v5 = make_vector_view(c1);
    ARRAY_EQUAL(v5, {1, 2, 3, 4, 5, 6});
    auto v6 = make_vector_view(c2);
    ARRAY_EQUAL(v6, {1, 3, 5});

    auto v7 = make_vector_view(r1);
    ARRAY_EQUAL(v7, {1, 2, 3, 4, 5, 6});
    auto v8 = make_vector_view(r2);
    ARRAY_EQUAL(v8, {1, 3, 5});
  }

  SECTION("make_row_vector_view") {
    auto v3 = make_row_vector_view(v1);
    ARRAY_EQUAL(v3, {{1, 2, 3, 4, 5, 6}});
    auto v4 = make_row_vector_view(v2);
    ARRAY_EQUAL(v4, {{1, 3, 5}});

    auto v5 = make_row_vector_view(c1);
    ARRAY_EQUAL(v5, {{1, 2, 3, 4, 5, 6}});
    auto v6 = make_row_vector_view(c2);
    ARRAY_EQUAL(v6, {{1, 3, 5}});

    auto v7 = make_row_vector_view(r1);
    ARRAY_EQUAL(v7, {{1, 2, 3, 4, 5, 6}});
    auto v8 = make_row_vector_view(r2);
    ARRAY_EQUAL(v8, {{1, 3, 5}});
  }

  SECTION("make_column_vector_view") {
    auto v3 = make_column_vector_view(v1);
    ARRAY_EQUAL(v3, {{1}, {2}, {3}, {4}, {5}, {6}});
    auto v4 = make_column_vector_view(v2);
    ARRAY_EQUAL(v4, {{1}, {3}, {5}});

    auto v5 = make_column_vector_view(c1);
    ARRAY_EQUAL(v5, {{1}, {2}, {3}, {4}, {5}, {6}});
    auto v6 = make_column_vector_view(c2);
    ARRAY_EQUAL(v6, {{1}, {3}, {5}});

    auto v7 = make_column_vector_view(r1);
    ARRAY_EQUAL(v7, {{1}, {2}, {3}, {4}, {5}, {6}});
    auto v8 = make_column_vector_view(r2);
    ARRAY_EQUAL(v8, {{1}, {3}, {5}});
  }
}

