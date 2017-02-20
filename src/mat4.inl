namespace ssm
{
inline mat4::mat4() {
	cols[0].vec_data = sse_load(0.f, 0.f, 0.f, 1.f);
	cols[1].vec_data = sse_load(0.f, 0.f, 1.f, 0.f);
	cols[2].vec_data = sse_load(0.f, 1.f, 0.f, 0.f);
	cols[3].vec_data = sse_load(1.f, 0.f, 0.f, 0.f);
}
inline mat4::mat4(vec4* data) : cols{ data[0], data[1], data[2], data[3] } {}
inline mat4::mat4(float val) : cols{ vec4(val) } {}
inline mat4::mat4(f128* data) : cols{ vec4(data[0]), vec4(data[1]), vec4(data[2]), vec4(data[3]) } {}

inline vec4 mat4::slice_horiz(int row) const {
	return vec4(cols[0][row], cols[1][row], cols[2][row], cols[3][row]);
}

inline vec4 mat4::slice_vert(int col) const {
	return cols[col];
}

inline float* mat4::data() {
	return cols[0].data();
}

inline vec4* mat4::begin() {
	return (vec4*)data();
}

inline vec4* mat4::end() {
	return (vec4*)(data() + 4);
}

inline const float* mat4::data() const {
	return cols[0].data();
}

inline const vec4* mat4::begin() const {
	return (vec4*)(data());
}

inline const vec4* mat4::end() const {
	return (vec4*)(data() + 4);
}

inline vec4& mat4::operator[](int index) {
	return cols[index];
}

inline const vec4& mat4::operator[](int index) const {
	return cols[index];
}

inline mat4& mat4::operator+=(const mat4& rhs) {
	cols[0] += rhs.cols[0];
	cols[1] += rhs.cols[1];
	cols[2] += rhs.cols[2];
	cols[3] += rhs.cols[3];
}

inline mat4& mat4::operator-=(const mat4& rhs) {
	cols[0] -= rhs.cols[0];
	cols[1] -= rhs.cols[1];
	cols[2] -= rhs.cols[2];
	cols[3] -= rhs.cols[3];
}

inline mat4& mat4::operator*=(float scale) {
	cols[0] *= scale;
	cols[1] *= scale;
	cols[2] *= scale;
	cols[3] *= scale;
}

inline mat4 operator+(mat4 lhs, const mat4& rhs) {
	return lhs += rhs;
}

inline mat4 operator-(mat4 lhs, const mat4& rhs) {
	return lhs -= rhs;
}

inline mat4 operator*(mat4 lhs, float scale) {
	return lhs *= scale;
}
}
