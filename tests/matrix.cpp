#undef NDEBUG
#include <chrono>
#include <iostream>

#include <ssm/matrix.hpp>
#include <ssm/simd/types.hpp>

int main() {
	ssm::mat4 mat;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; i < 4; ++i) {
			assert(mat[i][j] == 0.0f);
		}
	}

	mat = ssm::mat4(5.0f);
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; i < 4; ++i) {
			assert(mat[i][j] == 5.0f);
		}
	}

	mat += ssm::mat4(5.0f);
	assert(mat == ssm::mat4(10.0f));

	mat -= ssm::mat4(10.0f);
	assert(mat == ssm::mat4(0.0f));

	mat[0].x = 3.0f;
	mat[1].y = 3.0f;
	mat[2].z = 3.0f;
	mat[3].w = 3.0f;

	ssm::vec4 vec(1.0f);
	vec *= mat;
	assert(vec.x == 3.0f);
}
