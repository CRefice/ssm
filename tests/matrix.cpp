#undef NDEBUG
#include <chrono>
#include <iostream>

#include <ssm/matrix.hpp>
#include <ssm/simd/types.hpp>

int main() {
	ssm::matrix<float, 4, 4> mat4;
	mat4[0].x = 3.0f;
	mat4[1].y = 3.0f;
	mat4[2].z = 3.0f;
	mat4[3].w = 3.0f;

	ssm::vector<float, 4> vec(1.0f);
	vec *= mat4;
	assert(vec.x == 3.0f);
}
