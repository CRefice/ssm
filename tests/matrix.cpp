#include <catch2/catch.hpp>

#include <ssm/matrix.hpp>

template <typename T>
using tmat1 = ssm::matrix<T, 1>;
template <typename T>
using tmat2 = ssm::matrix<T, 2>;
template <typename T>
using tmat3 = ssm::matrix<T, 3>;
template <typename T>
using tmat4 = ssm::matrix<T, 4>;
template <typename T>
using tmat1x4 = ssm::matrix<T, 1, 4>;
template <typename T>
using tmat4x1 = ssm::matrix<T, 4, 1>;
template <typename T>
using tmat2x3 = ssm::matrix<T, 2, 3>;
template <typename T>
using tmat3x2 = ssm::matrix<T, 3, 2>;

TEMPLATE_PRODUCT_TEST_CASE("constructors", "[template][product]",
                           (tmat1, tmat2, tmat3, tmat4, tmat1x4, tmat4x1,
                            tmat2x3, tmat3x2),
                           (float, double, int, unsigned)) {
  using VecType = typename TestType::column_type;
  using T = typename VecType::value_type;

  SECTION("default constructor") {
    TestType mat;
    for (auto elem : mat) {
      REQUIRE(elem == T(0));
    }
  }

  SECTION("one-argument constructor") {
    T val = GENERATE(take(10, random(T(0), T(20))));
    auto mat = TestType(val);
    for (auto elem : mat) {
      REQUIRE(elem == val);
    }
  }
}

TEMPLATE_PRODUCT_TEST_CASE("equality comparisons", "[template][product]",
                           (tmat1, tmat2, tmat3, tmat4, tmat1x4, tmat4x1,
                            tmat2x3, tmat3x2),
                           (float, double, int, unsigned)) {
  using VecType = typename TestType::column_type;
  using T = typename VecType::value_type;

  TestType def(T(0));
  REQUIRE(def == def);
  T val = GENERATE(take(10, random(T(1), T(10))));
  TestType mat(val);
  REQUIRE(def != mat);
  REQUIRE(mat == mat);

  def = mat;
  REQUIRE(def == mat);
  mat[0] = VecType(T(0));
  REQUIRE(def != mat);
}

TEMPLATE_PRODUCT_TEST_CASE("matrix operations", "[template][product]",
                           (tmat1, tmat2, tmat3, tmat4, tmat1x4, tmat4x1,
                            tmat2x3, tmat3x2),
                           (float, double, int, unsigned)) {
  using VecType = typename TestType::column_type;
  using T = typename VecType::value_type;

  SECTION("addition") {
    TestType a(T(0)), b(T(0));
    REQUIRE(a + b == a);
    T x = GENERATE(take(10, random(T(0), T(10))));
    T y = GENERATE(take(10, random(T(0), T(10))));
    for (std::size_t i = 0; i < TestType::width; ++i) {
      for (std::size_t j = 0; j < TestType::height; ++j) {
        a[i][j] = x;
        b[i][j] = y;
        REQUIRE((a + b)[i][j] == x + y);
      }
    }
    REQUIRE(a + b == b + a);
    auto c = a;
    c += b;
    REQUIRE(c == a + b);
  }

  SECTION("negation") {
    TestType a;
    REQUIRE(a + (-a) == a - a);
    T x = GENERATE(take(10, random(T(0), T(10))));
    for (std::size_t i = 0; i < TestType::width; ++i) {
      for (std::size_t j = 0; j < TestType::height; ++j) {
        a[i][j] = x;
        REQUIRE((-a)[i][j] == -x);
      }
    }
  }

  SECTION("subtraction") {
    TestType a, b;
    REQUIRE(a - b == a);
    T x = GENERATE(take(10, random(T(0), T(10))));
    T y = GENERATE(take(10, random(T(0), T(10))));
    for (std::size_t i = 0; i < TestType::width; ++i) {
      for (std::size_t j = 0; j < TestType::height; ++j) {
        a[i][j] = x;
        b[i][j] = y;
        REQUIRE((a - b)[i][j] == x - y);
      }
    }
    REQUIRE(a - a == TestType(T(0)));
    REQUIRE(a - b == -(b - a));
    auto c = a;
    c -= b;
    REQUIRE(c == a - b);
  }

  SECTION("division") {
    T val = GENERATE(take(10, random(T(1), T(10))));
    TestType a(val);
    T div = GENERATE(take(10, random(T(1), T(10))));
    REQUIRE(a / div == TestType(T(val / div)));
    REQUIRE(a / val == TestType(T(1)));
    auto c = a;
    c /= div;
    REQUIRE(c == a / div);
  }
}

TEMPLATE_PRODUCT_TEST_CASE("matrix multiplication", "[template][product]",
                           (tmat4), (float, double, int, unsigned)) {
  using VecType = typename TestType::column_type;
  using T = typename VecType::value_type;

  TestType mat;
  mat[0] = {T(1), T(0), T(0), T(0)};
  mat[1] = {T(0), T(1), T(0), T(0)};
  mat[2] = {T(0), T(0), T(1), T(0)};
  mat[3] = {T(0), T(0), T(0), T(1)};

  VecType vec(T(1), T(2), T(3), T(4));
  REQUIRE((vec * mat) == (mat * vec));
  auto vec_2 = vec;
  vec_2 *= mat;
  REQUIRE(vec_2 == mat * vec);
  REQUIRE(vec_2 == vec);

  mat = TestType(T(2));
  vec_2 = vec;
  vec_2 *= mat;
  REQUIRE(vec_2 == mat * vec);
  REQUIRE(vec_2.x == Approx(20.0f));
  REQUIRE(vec_2.y == Approx(20.0f));
  REQUIRE(vec_2.z == Approx(20.0f));
  REQUIRE(vec_2.w == Approx(20.0f));

  TestType mat_2;
  mat_2[0] = {T(1), T(2), T(3), T(4)};
  mat_2[1] = {T(5), T(6), T(7), T(8)};
  mat_2[2] = {T(9), T(10), T(11), T(12)};
  mat_2[3] = {T(13), T(14), T(15), T(16)};

  REQUIRE(mat * mat_2 != mat_2 * mat);
  auto mat_3 = mat_2;
  mat_3 *= mat;
  REQUIRE(mat_3 == mat_2 * mat);

  TestType result;
  result[0] = {T(56), T(64), T(72), T(80)};
  result[1] = {T(56), T(64), T(72), T(80)};
  result[2] = {T(56), T(64), T(72), T(80)};
  result[3] = {T(56), T(64), T(72), T(80)};
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      REQUIRE(mat_3[i][j] == Approx(result[i][j]));
    }
  }
}
