#include <vector.hpp>
//#include <simd/types.hpp>

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

	assert(vec == vec);

	auto norm = ssm::normalize(vec);
	printf("%f\n", (double)ssm::length<float, 4>(norm));
	return 0;
}
