#include <cmath>

#include "intrin\loads.hpp"
#include "intrin\geometric.hpp"
#include "intrin\cache.hpp"

namespace ssm
{
inline mat4 translate(const vec3& pos) {
	mat4 retval;
	const f128 col3 = sse_add(retval[3].vec_data, pos.vec_data);
	retval[3].vec_data = col3;
	return retval;
}

inline mat4 rotate(const quat& rot) {
	mat4 result;

	const f128 wwww = sse_shuffle<3, 3, 3, 3>(rot.vec_data);
	const f128 xyzw = rot.vec_data;
	const f128 zxyw = sse_shuffle<3, 1, 0, 2>(rot.vec_data);
	const f128 yzxw = sse_shuffle<3, 0, 2, 1>(rot.vec_data);

	const f128 xyzw2 = sse_add(xyzw, xyzw);
	const f128 zxyw2 = sse_shuffle<3, 1, 0, 2>(xyzw2);
	const f128 yzxw2 = sse_shuffle<3, 0, 2, 1>(xyzw2);
	
	f128 tmp0 = sse_sub(sse_load_broad(1.f), sse_mul(yzxw2, yzxw));
	tmp0 = sse_sub(tmp0, sse_mul(zxyw2, zxyw));

	f128 tmp1 = sse_mul(yzxw2, xyzw);
	tmp1 = sse_add(tmp1, sse_mul(zxyw2, wwww));

	f128 tmp2 = sse_mul(zxyw2, xyzw);
	tmp2 = sse_sub(tmp2, sse_mul(yzxw2, wwww));

	// There's probably a better, more politically correct way of doing this...
	result[0].vec_data = sse_load(
			0.0f,
			reinterpret_cast<float*>(&tmp2)[0],
			reinterpret_cast<float*>(&tmp1)[0],
			reinterpret_cast<float*>(&tmp0)[0]);

	result[1].vec_data = sse_load(
			0.0f,
			reinterpret_cast<float*>(&tmp1)[1],
			reinterpret_cast<float*>(&tmp0)[1],
			reinterpret_cast<float*>(&tmp2)[1]);

	result[2].vec_data = sse_load(
			0.0f,
			reinterpret_cast<float*>(&tmp0)[2],
			reinterpret_cast<float*>(&tmp2)[2],
			reinterpret_cast<float*>(&tmp1)[2]);

	return result;
}

inline mat4 scale(const vec3& scale) {
	mat4 retval;
	retval[0].vec_data = sse_load(0.f, 0.f, 0.f, scale.x);
	retval[1].vec_data = sse_load(0.f, 0.f, scale.y, 0.f);
	retval[2].vec_data = sse_load(0.f, scale.z, 0.f, 0.f);
	return retval;
}

inline mat4 perspective(float fovy, float aspect, float znear, float zfar) {
	const float halftan = std::tan(fovy / 2.f);

	mat4 retval;
	retval[0].vec_data = sse_load(0.f, 0.f, 0.f, 1.f / aspect * halftan);
	retval[1].vec_data = sse_load(0.f, 0.f, 1.f / halftan, 0.f);
	retval[2].vec_data = sse_load(-1.f, -(zfar + znear) / (zfar - znear), 0.f, 0.f);
	retval[3].vec_data = sse_load(0.f, -2 * (zfar * znear) / (zfar - znear), 0.f, 0.f);
	return retval;
}

//It's like writing assembly...
inline vec4 operator*(const mat4& mat, const vec4& vec) {
	const f128 v0 = vec.vec_data;

	f128 vx = sse_shuffle<0, 0, 0, 0>(v0);
	f128 vy = sse_shuffle<1, 1, 1, 1>(v0);
	f128 vz = sse_shuffle<2, 2, 2, 2>(v0);
	f128 vw = sse_shuffle<3, 3, 3, 3>(v0);

	vx = sse_mul(vx, mat[0].vec_data);
	vy = sse_mul(vy, mat[1].vec_data);
	vz = sse_mul(vz, mat[2].vec_data);
	vw = sse_mul(vw, mat[3].vec_data);

	const f128 sum1 = sse_add(vx, vy);
	const f128 sum2 = sse_add(vz, vw);
	return vec4(sse_add(sum1, sum2));
}

inline mat4 operator*(const mat4& lhs, const mat4& rhs) {
	mat4 retval;
	batch_transform(lhs, (vec4*)rhs.data(), (vec4*)retval.data(), 4);
	return retval;
}

inline void batch_transform(const mat4& mat, vec4* vin, vec4* vout, unsigned count) {
	const f128 m1 = mat[0].vec_data;
	const f128 m2 = mat[1].vec_data;
	const f128 m3 = mat[2].vec_data;
	const f128 m4 = mat[3].vec_data;

	//We're processing two vectors at a time.
	//Remember, count should be even!
	count >>= 1;

	while(count--) {	
		f128 tot1, tot2;
		const f128 v1 = vin[0].vec_data;
		const f128 v2 = vin[1].vec_data;
		sse_prefetch(vin + 0x30);
		sse_prefetch(vout + 0x30);

		f128 x1 = sse_shuffle<0, 0, 0, 0>(v1);
		f128 x2 = sse_shuffle<0, 0, 0, 0>(v2);
		x1 = sse_mul(x1, m1);
		x2 = sse_mul(x2, m1);

		f128 y1 = sse_shuffle<1, 1, 1, 1>(v1);
		f128 y2 = sse_shuffle<1, 1, 1, 1>(v2);
		y1 = sse_mul(y1, m2);
		y2 = sse_mul(y2, m2);
		tot1 = sse_add(x1, y1);
		tot2 = sse_add(x2, y2);

		f128 z1 = sse_shuffle<2, 2, 2, 2>(v1);
		f128 z2 = sse_shuffle<2, 2, 2, 2>(v2);
		z1 = sse_mul(z1, m3);
		z2 = sse_mul(z2, m3);
		tot1 = sse_add(tot1, z1);
		tot2 = sse_add(tot2, z2);

		f128 w1 = sse_shuffle<3, 3, 3, 3>(v1);
		f128 w2 = sse_shuffle<3, 3, 3, 3>(v2);
		w1 = sse_mul(w1, m4);
		w2 = sse_mul(w2, m4);
		tot1 = sse_add(tot1, w1);
		tot2 = sse_add(tot2, w2);

		sse_store(tot1, (float*)(vout));
		sse_store(tot2, (float*)(vout + 1));
		vin += 2;
		vout += 2;
	}
}
}
