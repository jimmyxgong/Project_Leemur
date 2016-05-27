#include "Quaternion.h"

const Quaternion Quaternion::identity = { 0.0f, 0.0f, 0.0f, 1.0f };

Quaternion::Quaternion(float x, float y, float z, float w) 
	: x(x), y(y), z(z), w(w) {}

Quaternion::Quaternion() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}


Quaternion Quaternion::normalize() {
	const float n = 1.0f / sqrt(x*x + y*y + z*z + w*w);
	x *= n;
	y *= n;
	z *= n;
	w *= n;

	return std::move(*this);
}


void Quaternion::print() {

	std::cout << x << " | " << y << " | " << z << " | " << w <<std::endl;

}






Quaternion Quaternion::operator*(Quaternion& q1) {
	Quaternion quat;
	quat.x = (w * q1.x) + (x * q1.w) + (y * q1.z) - (z * q1.y);
	quat.y = (w * q1.y) - (x * q1.z) + (y * q1.w) + (z * q1.x);
	quat.z = (w * q1.z) + (x * q1.y) - (y * q1.x) + (z * q1.w);
	quat.w = (w * q1.w) - (x * q1.x) - (y * q1.y) - (z * q1.z);

	return quat;
}

Matrix4f Quaternion::operator*(Matrix4f const & m) {
	return toMatrix() * m;
}

Vector4f Quaternion::operator*(Vector4f& in) {
	return in * asVector();
}

Quaternion Quaternion::operator+(Quaternion& q) {
	Quaternion quat;
	Vector3f v1(x, y, z);
	Vector3f v2(q.x, q.y, q.z);
	Vector3f v3 = cross(v1, v2);
	
	Vector3f final = (v1 * q.w) + (v2 * w) + v3;
	quat.x = final.x;
	quat.y = final.y;
	quat.z = final.z;
	quat.w = w * q.w - dot(v1, v2);

	return quat;
}


Vector4f Quaternion::asVector() {
	return{ x, y, z, w };
}

Matrix4f Quaternion::toMatrix() {
	Matrix4f output;
	toMatrix(*this, output);
	return output;
}



void Quaternion::Conjugate(Quaternion& quat, Vector4f& out) {
	out.x = -quat.x;
	out.y = -quat.y;
	out.z = -quat.z;
	out.w = quat.w;
}

void Quaternion::toMatrix(Quaternion& quat, Matrix4f& matrix) {
	float x = quat.x;
	float y = quat.y;
	float z = quat.z;
	float w = quat.w;
	
	float xy = 2.0f * x * y;
	float xz = 2.0f * x * z;
	float yz = 2.0f * y * z;

	float wx = 2.0f * x * w;
	float wy = 2.0f * y * w;
	float wz = 2.0f * z * w;

	matrix = {
		{ 1 - (2.0f * y*y) - (2.0f * z*z), xy - wz, xz + wy, 0},
		{ xy + wz, 1 - (2.0f * x * x) - (2.0f * z * z), yz - wx, 0 },
		{ xz - wy, yz + wx, 1 - (2.0f * x * x) - (2.0f * y * y), 0 },
		{ 0, 0, 0, 1 }
	};
}

Quaternion Quaternion::FromAxis(float x, float y, float z, float angle) {
	angle = glm::radians(angle);
	float f = sin(angle / 2.0f);

	float xx = x * f;
	float yy = y * f;
	float zz = z * f;
	float ww = cos(angle / 2.0f);

	return std::move(Quaternion( xx, yy, zz, ww ).normalize());
}

void Quaternion::FromToRotation(Vector3f& from, Vector3f& to, Quaternion& out) {
	



}


