#include <catch2/catch.hpp>

#include <ssm/vector.hpp>

template <typename T>
using tvec1 = ssm::vector<T, 1>;
template <typename T>
using tvec2 = ssm::vector<T, 2>;
template <typename T>
using tvec3 = ssm::vector<T, 3>;
template <typename T>
using tvec4 = ssm::vector<T, 4>;
template <typename T>
using tvec5 = ssm::vector<T, 5>;
template <typename T>
using tvec10 = ssm::vector<T, 10>;

TEMPLATE_PRODUCT_TEST_CASE("constructors", "[template][product]",
                           (tvec1, tvec2, tvec3, tvec4, tvec5, tvec10),
                           (float, double, int, unsigned)) {
  using T = typename TestType::value_type;

  SECTION("default constructor") {
    TestType vec;
    for (std::size_t i = 0; i < TestType::size; ++i) {
      REQUIRE(vec[i] == T(0));
    }
  }

  SECTION("one-argument constructor") {
    T val = GENERATE(take(10, random(T(0), T(20))));
    auto vec = TestType(val);
    for (std::size_t i = 0; i < TestType::size; ++i) {
      REQUIRE(vec[i] == val);
    }
  }

  SECTION("two-argument constructor") {
    T x = GENERATE(take(10, random(T(0), T(5))));
    T y = GENERATE(take(10, random(T(0), T(5))));
    auto vec = ssm::vector<T, 2>(x, y);
    REQUIRE(vec.x == x);
    REQUIRE(vec.y == y);
  }

  SECTION("three-argument constructor") {
    T x = GENERATE(take(10, random(T(0), T(5))));
    T y = GENERATE(take(10, random(T(0), T(5))));
    T z = GENERATE(take(10, random(T(0), T(5))));
    auto vec = ssm::vector<T, 3>(x, y, z);
    REQUIRE(vec.x == x);
    REQUIRE(vec.y == y);
    REQUIRE(vec.z == z);
  }

  SECTION("four-argument constructor") {
    T x = GENERATE(take(10, random(T(0), T(5))));
    T y = GENERATE(take(10, random(T(0), T(5))));
    T z = GENERATE(take(10, random(T(0), T(5))));
    T w = GENERATE(take(10, random(T(0), T(5))));
    auto vec = ssm::vector<T, 4>(x, y, z, w);
    REQUIRE(vec.x == x);
    REQUIRE(vec.y == y);
    REQUIRE(vec.z == z);
    REQUIRE(vec.w == w);
  }
}

TEMPLATE_TEST_CASE("member getters-setters", "[template]", ssm::vec4, ssm::dvec4, ssm::ivec4, ssm::uvec4) {
	using T = typename TestType::value_type;

  TestType vec(T(1), T(2), T(3), T(4));
  REQUIRE(vec.x == T(1));
  REQUIRE(vec.y == T(2));
  REQUIRE(vec.z == T(3));
  REQUIRE(vec.w == T(4));
  vec.x = vec.y;
  REQUIRE(vec.x == T(2));
  REQUIRE(vec.y == T(2));
  vec.z = vec.w;
  REQUIRE(vec.z == T(4));
  REQUIRE(vec.w == T(4));
  vec.x = T(5);
  vec.y = T(6);
  vec.z = T(7);
  vec.w = T(8);
  REQUIRE(vec.x == T(5));
  REQUIRE(vec.y == T(6));
  REQUIRE(vec.z == T(7));
  REQUIRE(vec.w == T(8));
  TestType vec_2(T(1), T(2), T(3), T(4));
  vec.x = (T)vec_2.x;
  REQUIRE(vec.x == T(1));
  REQUIRE(vec.y == T(6));
  REQUIRE(vec.z == T(7));
  REQUIRE(vec.w == T(8));
}

TEMPLATE_PRODUCT_TEST_CASE("equality comparisons", "[template][product]",
                           (tvec1, tvec2, tvec3, tvec4, tvec5, tvec10),
                           (float, double, int, unsigned)) {
  using T = typename TestType::value_type;
  TestType def(T(0));
  REQUIRE(def == def);
  T val = GENERATE(take(10, random(T(1), T(10))));
  TestType vec(val);
  REQUIRE(def != vec);
  REQUIRE(vec == vec);

  def = vec;
  REQUIRE(def == vec);
  vec[0] = T(0);
  REQUIRE(def != vec);
}

TEMPLATE_PRODUCT_TEST_CASE("vector operations", "[template][product]",
                           (tvec1, tvec2, tvec3, tvec4, tvec5, tvec10),
                           (float, double)) {
  using T = typename TestType::value_type;

  SECTION("length") {
    TestType vec;
    REQUIRE(ssm::sqlength(vec) == T(0));
    REQUIRE(ssm::length(vec) == T(0));
    T val = GENERATE(take(10, random(T(-100), T(100))));
    T exp_sq_length = T(0);
    for (std::size_t i = 0; i < TestType::size; ++i) {
      REQUIRE(ssm::sqlength(vec) == Approx(exp_sq_length));
      REQUIRE(ssm::length(vec) == Approx(std::sqrt(exp_sq_length)));
      vec[i] = val;
      exp_sq_length += val * val;
    }
  }

  SECTION("dot product") {
    TestType a, b;
    T exp_dot = T(0);
    T x = GENERATE(take(10, random(T(-10), T(10))));
    T y = GENERATE(take(10, random(T(-10), T(10))));
    for (std::size_t i = 0; i < TestType::size; ++i) {
      REQUIRE(ssm::dot(a, b) == Approx(exp_dot));
      a[i] = x;
      b[i] = y;
      exp_dot += x * y;
    }
  }

  SECTION("distance") {
    TestType a; // TODO: generate vec
    TestType b;
    REQUIRE(ssm::distance(a, a) == Approx(T(0)));
    REQUIRE(ssm::distance(a, b) == Approx(ssm::length(a)));
    b = -a;
    REQUIRE(ssm::distance(a, b) == Approx(T(2) * ssm::length(a)));
  }

  SECTION("normalization") {
    TestType vec(T(1)); // TODO: generate vec
    auto norm = ssm::normalize(vec);
    REQUIRE(ssm::length(norm) == Approx(T(1)));
  }
}

TEMPLATE_TEST_CASE("cross product", "ssm::vec3<[template]>", int, unsigned,
                   float, double) {
  using T = TestType;
  using Vec = ssm::vector<T, 3>;
  Vec a = Vec(T(1), T(0), T(0));
  Vec b = Vec(T(0), T(1), T(0));
  auto cross = ssm::cross(a, b);
  REQUIRE(cross.x == Approx(T(0)));
  REQUIRE(cross.y == Approx(T(0)));
  REQUIRE(cross.z == Approx(T(1)));

  a = Vec(T(1), T(2), T(3));
  b = Vec(T(1), T(5), T(7));
  cross = ssm::cross(a, b);
  REQUIRE(cross.x == Approx(T(-1)));
  REQUIRE(cross.y == Approx(T(-4)));
  REQUIRE(cross.z == Approx(T(3)));
}

TEMPLATE_PRODUCT_TEST_CASE("arithmetic operations", "[template][product]",
                           (tvec1, tvec2, tvec3, tvec4, tvec5, tvec10),
                           (float, double, int, unsigned)) {
  using T = typename TestType::value_type;

  SECTION("addition") {
    TestType a, b;
    REQUIRE(a + b == a);
    T x = GENERATE(take(10, random(T(0), T(10))));
    T y = GENERATE(take(10, random(T(0), T(10))));
    for (std::size_t i = 0; i < TestType::size; ++i) {
      a[i] = x;
      b[i] = y;
      REQUIRE((a + b)[i] == x + y);
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
    for (std::size_t i = 0; i < TestType::size; ++i) {
      a[i] = x;
      REQUIRE((-a)[i] == -x);
    }
  }

  SECTION("subtraction") {
    TestType a, b;
    REQUIRE(a - b == a);
    T x = GENERATE(take(10, random(T(0), T(10))));
    T y = GENERATE(take(10, random(T(0), T(10))));
    for (std::size_t i = 0; i < TestType::size; ++i) {
      a[i] = x;
      b[i] = y;
      REQUIRE((a - b)[i] == x - y);
    }
    REQUIRE(a - a == TestType(T(0)));
    REQUIRE(a - b == -(b - a));
    auto c = a;
    c -= b;
    REQUIRE(c == a - b);
  }

  SECTION("pairwise multiplication") {
    TestType a;
    TestType b(T(1));
    REQUIRE(a * a == a);
    REQUIRE(a * b == a);
    REQUIRE(b * b == b);
    T x = GENERATE(take(10, random(T(0), T(10))));
    T y = GENERATE(take(10, random(T(0), T(10))));
    for (std::size_t i = 0; i < TestType::size; ++i) {
      a[i] = x;
      b[i] = y;
      REQUIRE((a * b)[i] == x * y);
    }
    REQUIRE(a * b == b * a);
    auto c = a;
    c *= b;
    REQUIRE(c == a * b);
  }

  SECTION("pairwise division") {
    TestType a;
    TestType b(T(1));
    REQUIRE(a / b == a);
    REQUIRE(b / b == b);
    T x = GENERATE(take(10, random(T(1), T(10))));
    T y = GENERATE(take(10, random(T(1), T(10))));
    for (std::size_t i = 0; i < TestType::size; ++i) {
      a[i] = x;
      b[i] = y;
      REQUIRE((a / b)[i] == (x / y));
    }
    REQUIRE(a / a == TestType(T(1)));
    REQUIRE(b / b == TestType(T(1)));
    auto c = a;
    c /= b;
    REQUIRE(c == a / b);
  }
}

TEMPLATE_PRODUCT_TEST_CASE("vector conversions", "[template][product]",
                           (tvec1, tvec2, tvec3, tvec4, tvec5, tvec10),
                           (float, double)) {
	using T = typename TestType::value_type;
	using CvtType = ssm::vector<int, TestType::size>;

	TestType vec(T(1.5));
	REQUIRE((CvtType)vec == CvtType(1));
}
