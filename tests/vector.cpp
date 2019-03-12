#include <type_traits>

#include <catch2/catch.hpp>

#include <ssm/vector.hpp>

template <std::size_t N> using Sz = std::integral_constant<std::size_t, N>;

template <std::size_t N> using I = ssm::vector<int, N>;
template <std::size_t N> using U = ssm::vector<unsigned, N>;
template <std::size_t N> using F = ssm::vector<float, N>;
template <std::size_t N> using D = ssm::vector<double, N>;

#define SSM_VEC_TEST_CASE(x)                                                   \
  TEMPLATE_TEST_CASE(x, "[template]", I<2>, I<3>, I<4>, U<2>, U<3>, U<4>,      \
                     F<2>, F<3>, F<4>, D<2>, D<3>, D<4>)
#define SSM_VEC_FLOAT_TEST_CASE(x)                                             \
  TEMPLATE_TEST_CASE(x, "[template]", F<2>, F<3>, F<4>, D<2>, D<3>, D<4>)

SSM_VEC_TEST_CASE("constructors") {
  using T = typename TestType::value_type;

  SECTION("default constructor") {
    TestType vec;
    for (std::size_t i = 0; i < TestType::size; ++i) {
      REQUIRE(vec[i] == T(0));
    }
  }

  SECTION("one-argument constructor") {
    T val = GENERATE(take(10, random(T(0), T(20))));
		CAPTURE(val);
    auto vec = TestType(val);
    for (std::size_t i = 0; i < TestType::size; ++i) {
      REQUIRE(vec[i] == val);
    }
  }

  SECTION("two-argument constructor") {
    T x = GENERATE(take(10, random(T(0), T(5))));
    T y = GENERATE(take(10, random(T(0), T(5))));
		CAPTURE(x, y);
    auto vec = ssm::vector<T, 2>(x, y);
    REQUIRE(vec.x == x);
    REQUIRE(vec.y == y);
  }

  SECTION("three-argument constructor") {
    T x = GENERATE(take(10, random(T(0), T(5))));
    T y = GENERATE(take(10, random(T(0), T(5))));
    T z = GENERATE(take(10, random(T(0), T(5))));
		CAPTURE(x, y, z);
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
		CAPTURE(x, y, z, w);
    auto vec = ssm::vector<T, 4>(x, y, z, w);
    REQUIRE(vec.x == x);
    REQUIRE(vec.y == y);
    REQUIRE(vec.z == z);
    REQUIRE(vec.w == w);
  }
}

TEST_CASE("member getters-setters", "[vector]") {
  ssm::vec4 vec(1.0f, 2.0f, 3.0f, 4.0f);
  REQUIRE(vec.x == 1.0f);
  REQUIRE(vec.y == 2.0f);
  REQUIRE(vec.z == 3.0f);
  REQUIRE(vec.w == 4.0f);
  vec.x = vec.y;
  REQUIRE(vec.x == 2.0f);
  REQUIRE(vec.y == 2.0f);
  vec.z = vec.w;
  REQUIRE(vec.z == 4.0f);
  REQUIRE(vec.w == 4.0f);
  vec.x = 5.0f;
  vec.y = 6.0f;
  vec.z = 7.0f;
  vec.w = 8.0f;
  REQUIRE(vec.x == 5.0f);
  REQUIRE(vec.y == 6.0f);
  REQUIRE(vec.z == 7.0f);
  REQUIRE(vec.w == 8.0f);
  ssm::vec4 vec_2(1.0f, 2.0f, 3.0f, 4.0f);
  vec.x = (float)vec_2.x;
  REQUIRE(vec.x == 1.0f);
  REQUIRE(vec.y == 6.0f);
  REQUIRE(vec.z == 7.0f);
  REQUIRE(vec.w == 8.0f);
}

SSM_VEC_TEST_CASE("equality comparison") {
  using T = typename TestType::value_type;
  TestType def(T(0));
  REQUIRE(def == def);
  T val = GENERATE(take(10, random(T(1), T(10))));
	CAPTURE(val);
  TestType vec(val);
  REQUIRE(def != vec);
  REQUIRE(vec == vec);

  def = vec;
  REQUIRE(def == vec);
  vec[0] = T(0);
  REQUIRE(def != vec);
}

SSM_VEC_FLOAT_TEST_CASE("vector operations") {
  using T = typename TestType::value_type;

  SECTION("length") {
    TestType vec;
    REQUIRE(ssm::sqlength(vec) == T(0));
    REQUIRE(ssm::length(vec) == T(0));
    T val = GENERATE(take(10, random(T(-100), T(100))));
		CAPTURE(val);
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
		CAPTURE(x, y);
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

SSM_VEC_TEST_CASE("arithmetic operations") {
  using T = typename TestType::value_type;

  SECTION("addition") {
    TestType a, b;
    REQUIRE(a + b == a);
		T x = GENERATE(take(10, random(T(0), T(10))));
		T y = GENERATE(take(10, random(T(0), T(10))));
    for (std::size_t i = 0; i < TestType::size; ++i) {
			CAPTURE(x, y);
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
			CAPTURE(x);
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
			CAPTURE(x, y);
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
			CAPTURE(x, y);
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
			CAPTURE(x, y);
      a[i] = x;
      b[i] = y;
			CAPTURE(a, b, a / b);
			CAPTURE((float)x, (float)y, (float)x / (float)y);
			CAPTURE((int)((float)x / (float)y));
      REQUIRE((a / b)[i] == (x / y));
    }
    REQUIRE(a / a == TestType(T(1)));
    REQUIRE(b / b == TestType(T(1)));
    auto c = a;
    c /= b;
    REQUIRE(c == a / b);
  }
}
