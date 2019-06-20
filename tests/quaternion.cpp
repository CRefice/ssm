#include <catch2/catch.hpp>

#include <ssm/quaternion.hpp>

TEST_CASE("operations", "[quaternion]") {
  ssm::quat q;
  ssm::dquat dq;

  SECTION("equality comparisons") {
    REQUIRE(q == q);
    ssm::quat q2(10.0f);
    REQUIRE(q != q2);
    REQUIRE(q2 == q2);

    q2 = q;
    REQUIRE(q == q2);
    q2.w = 5.0f;
    REQUIRE(q != q2);
  }

  SECTION("norm") {
    ssm::quat q;

    q = ssm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    dq = ssm::dquat(1.0, 0.0, 0.0, 0.0);
    REQUIRE(ssm::sqnorm(q) == 1.0f);
    REQUIRE(ssm::sqnorm(dq) == 1.0);
    REQUIRE(ssm::norm(dq) == 1.0);
    q = ssm::quat(1.0f, 1.0f, 0.0f, 0.0f);
    REQUIRE(ssm::sqnorm(q) == Approx(2.0f));
    REQUIRE(ssm::norm(q) == Approx(std::sqrt(2.0f)));
    q = ssm::quat(1.0f, 1.0f, 1.0f, 0.0f);
    REQUIRE(ssm::sqnorm(q) == Approx(3.0f));
    REQUIRE(ssm::norm(q) == Approx(std::sqrt(3.0f)));
    q = ssm::quat(1.0f, 1.0f, 1.0f, 1.0f);
    REQUIRE(ssm::sqnorm(q) == Approx(4.0f));
    REQUIRE(ssm::norm(q) == Approx(2.0f));
    q = ssm::quat(1.0f, 2.0f, 3.0f, 4.0f);
    REQUIRE(ssm::sqnorm(q) == Approx(30.0f));
    REQUIRE(ssm::norm(q) == Approx(std::sqrt(30.0f)));
  }

  SECTION("conjugate") {
    auto q = ssm::quat(5.0f);
    q = ssm::conjugate(q);
    REQUIRE(q.x == -5.0f);
    REQUIRE(q.y == -5.0f);
    REQUIRE(q.z == -5.0f);
    REQUIRE(q.w == 5.0f);
  }

  SECTION("inverse") {
    auto q = ssm::quat(5.0f);
    auto q2 = ssm::inverse(q);
    q = ssm::conjugate(q);
    auto norm = ssm::sqnorm(q);
    REQUIRE(q2 == q / norm);

    auto uq = ssm::normalize(q);
    auto uq2 = ssm::inverse(uq);
    uq = ssm::conjugate(uq);
    auto unorm = ssm::sqnorm(uq); // Should be one
    REQUIRE(uq2 == uq / unorm);
  }

  SECTION("normalization") {
    q = ssm::quat(1.0f);
    auto uq = ssm::normalize(q);
    REQUIRE(ssm::norm(uq) == Approx(1.0f));
    REQUIRE(uq.x == Approx(0.5f));
    REQUIRE(uq.y == Approx(0.5f));
    REQUIRE(uq.z == Approx(0.5f));
    REQUIRE(uq.w == Approx(0.5f));
  }

  SECTION("arithmetic operators") {
    q = ssm::quat(1.0f, 2.0f, 3.0f, 4.0f);
    auto q2 = ssm::quat(4.0f, 3.0f, 2.0f, 1.0f);

    REQUIRE(q + q2 == q2 + q);
    auto q3 = q;
    q3 += q2;
    REQUIRE(q3 == q + q2);
    REQUIRE(q3.x == Approx(5.0f));
    REQUIRE(q3.y == Approx(5.0f));
    REQUIRE(q3.z == Approx(5.0f));
    REQUIRE(q3.w == Approx(5.0f));

    q3 = q;
    q3 -= q2;
    REQUIRE(q3 == q - q2);
    REQUIRE(q3.x == Approx(-3.0f));
    REQUIRE(q3.y == Approx(-1.0f));
    REQUIRE(q3.z == Approx(1.0f));
    REQUIRE(q3.w == Approx(3.0f));

    REQUIRE(q * 3.0f == 3.0f * q);
    q3 = q;
    q3 *= 3.0f;
    REQUIRE(q3 == q * 3.0f);
    REQUIRE(q3.x == Approx(3.0f));
    REQUIRE(q3.y == Approx(6.0f));
    REQUIRE(q3.z == Approx(9.0f));
    REQUIRE(q3.w == Approx(12.0f));

    q3 = q;
    q3 /= 3.0f;
    REQUIRE(q3 == q / 3.0f);
    REQUIRE(q3.x == Approx(1.0f / 3.0f));
    REQUIRE(q3.y == Approx(2.0f / 3.0f));
    REQUIRE(q3.z == Approx(1.0f));
    REQUIRE(q3.w == Approx(4.0f / 3.0f));

    q3 = -q;
    REQUIRE(q3.x == -1.0f);
    REQUIRE(q3.y == -2.0f);
    REQUIRE(q3.z == -3.0f);
    REQUIRE(q3.w == -4.0f);
  }

  SECTION("multiplication") {
    ssm::quat q(-2.0f, 3.0f, 2.0f, 5.0f);
    ssm::quat q2(5.0f, 7.0f, 9.0f, 3.0f);
    REQUIRE(q * q2 != q2 * q);
    ssm::quat q3 = q;
    q3 *= q2;
    REQUIRE(q3 == q * q2);
    REQUIRE(q3.x == Approx(32.0f));
    REQUIRE(q3.y == Approx(72.0f));
    REQUIRE(q3.z == Approx(22.0f));
    REQUIRE(q3.w == Approx(-14.0f));
  }
}
