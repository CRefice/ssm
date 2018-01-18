#include <chrono>
#include <iostream>

#include <ssm/vector.hpp>

int main() {
	ssm::vec4 vec;
	assert(vec.x == 0.0f);
	assert(vec.y == 0.0f);
	assert(vec.z == 0.0f);
	assert(vec.w == 0.0f);

	vec = ssm::vec4(4.0f, 3.0f, 2.0f, 1.0f);
	assert(vec.x == 4.0f);
	assert(vec.y == 3.0f);
	assert(vec.z == 2.0f);
	assert(vec.w == 1.0f);
	vec = -vec;
	assert(vec.x == -4.0f);
	assert(vec.y == -3.0f);
	assert(vec.z == -2.0f);
	assert(vec.w == -1.0f);
	vec *= ssm::vec4(-2.0f, -2.5f, -4.0f, -8.0f);
	assert(vec.x == 8.0f);
	assert(vec.y == (-3.0f * -2.5f));
	assert(vec.z == 8.0f);
	assert(vec.w == 8.0f);

	assert(vec == vec);

	vec = ssm::vec4(5.0f);
	vec /= 5.0f;

	assert(vec.x == 1.0f);
	assert(vec.y == 1.0f);
	assert(vec.z == 1.0f);
	assert(vec.w == 1.0f);

	auto norm = ssm::normalize(vec);
	assert(ssm::dot(norm, norm) == 1.0f);

	return 0;
}
