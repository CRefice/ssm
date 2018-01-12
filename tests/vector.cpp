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

	auto b4 = std::chrono::high_resolution_clock::now();
	ssm::vec4 vec3 = vec;
	for (int i = 0; i < 10000; ++i) {
		vec3 += ssm::vec4{ 0.1f, 0.1f, 0.1f, 0.1f };
	}
	auto after = std::chrono::high_resolution_clock::now();
	std::cout << (after - b4).count() << std::endl;

	assert(vec == vec);

	auto norm = ssm::normalize(vec);

	ssm::vec4 vec2 = { -1, -1, -1, -1 };
	auto now = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 100000; ++i) {
		vec2 *= ssm::vec4{ 2, 2, 2, 2 };
		vec2 += (ssm::vec4)norm;
	}
	auto dur = std::chrono::high_resolution_clock::now() - now;
	std::cout << dur.count() << std::endl;

	ssm::ivec2 ivec{ 5, 10 };
	for (int i = 0; i < 5; ++i) {
		ivec += ssm::ivec2{ 2, 2 };
	}

	std::cout << ssm::detail::vec_impl<float, 4>::dot(vec, vec) << std::endl;
	std::cout << ssm::length(vec) << std::endl;

	//std::cout << ssm::length(ivec) << std::endl;
	return 0;
}
