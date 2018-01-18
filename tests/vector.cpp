#include <chrono>
#include <iostream>

#include <ssm/vector.hpp>
#include <ssm/simd/types.hpp>

int main() {
	ssm::vec4 vec{};
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

	auto norm = ssm::normalize(vec);

	ssm::ivec2 ivec{ 5, 10 };
	for (int i = 0; i < 5; ++i) {
		ivec += ssm::ivec2{ 2, 2 };
	}

	std::cout << ssm::dot(norm, norm) << std::endl;
	return 0;
}
