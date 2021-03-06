#pragma once

#include "Commons.h"

struct Quaternion {
private:

public:
	float x, y, z, w = 1.0f;
	Vector3f eulerAngles;

	Quaternion normalize();
	void print();

	Quaternion operator*(const Quaternion &) const;
	Quaternion operator*(const Quaternion &);
	Matrix4f operator*(Matrix4f const &);
	Vector4f operator*(Vector4f&);
	Quaternion operator+(Quaternion&);


	Vector4f asVector();
	Matrix4f asMatrix();

	Quaternion();
	Quaternion(float x, float y, float z, float w);

	static void Conjugate(Quaternion const & in, Vector4f& out);
	static void toMatrix(Quaternion const & in, Matrix4f& out);
	static Quaternion FromAxis(float x, float y, float z, float deg);
	static void FromToRotation(Vector3f& in0, Vector3f& in1, Quaternion& out);
	static Quaternion FromMatrix(Matrix4f const & in, Quaternion & out);
	static Quaternion FromMatrix(Matrix4f const & in);

	const static Quaternion identity;

};
