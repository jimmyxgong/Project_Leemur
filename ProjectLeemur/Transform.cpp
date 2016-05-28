#include "Transform.h"
#include <math.h>



Vector3f Transform::getLocalScale() const {
	return localScale;
}

Vector3f Transform::getLocalPosition() const {
	return localPosition;
}

Quaternion Transform::getLocalRotation() const {
	return localRotation;
}

Vector3f Transform::getPosition() const {
	return position;
}

Quaternion Transform::getRotation() const {
	return rotation;
}




Transform & Transform::setLocalScale(Vector3f const & val) {
	localScale = val;
	return *this;
}

Transform & Transform::setLocalPosition(Vector3f const & val) {
	localPosition = val;
	return *this;
}

Transform & Transform::setLocalRotation(Quaternion const & val) {
	localRotation = val;
	return *this;
}

Transform & Transform::setPosition(const Vector3f & pos) {
	position = pos;
	return *this;
}

Transform & Transform::setRotation(const Quaternion & rot) {
	rotation = rot;
	return *this;
}




Transform& Transform::scaleLocal(float xyz) {
	return scaleLocal(xyz, xyz, xyz);
}

Transform& Transform::scaleLocal(float x, float y, float z) {
	localScale = Vector3f(Scale(x, y, z) * Vector4f(localScale, 1.0f));
	return *this;
}

Transform& Transform::scaleLocal(Vector3f const & value) {
	return scaleLocal(value.x, value.y, value.z);
}

Transform& Transform::rotateLocalX(float deg) {
	return rotateLocal(1.f, 0.f, 0.f, deg);
}

Transform& Transform::rotateLocalY(float deg) {
	return rotateLocal(0.f, 1.f, 0.f, deg);
}

Transform& Transform::rotateLocalZ(float deg) {
	return rotateLocal(0.f, 0.f, 1.f, deg);
}

Transform& Transform::rotateLocal(float x, float y, float z, float deg) {
	return rotateLocal(Quaternion::FromAxis(x, y, z, deg));
}

Transform& Transform::rotateLocal(Quaternion & other) {
	localRotation = other * localRotation;
	return *this;
}

Transform& Transform::translateLocal(float x, float y, float z) {
	localPosition = Vector3f(Translate(x, y, z) * Vector4f(localPosition, 1.0f));
	return *this;
}

Transform& Transform::translateLocal(const Vector3f & value) {
	return translateLocal(value.x, value.y, value.z);
}







void Transform::evaluateMatrix() {
	// TODO: trickle down transform from parent
	localToWorldMatrix = localRotation * Translate(localPosition) * Scale(localScale);
}


/* Copy Constructor */
Transform::Transform(const Transform & transform) {
	localScale = transform.getLocalScale();
	localPosition = transform.getLocalPosition();
	localRotation = transform.getLocalRotation();

	evaluateMatrix();
}

Transform::Transform() : 
	localPosition(0.0f, 0.0f, 0.0f),
	localScale(1.0f),
	position(0.0f, 0.0f, 0.0f),
	scaling(1.0f)
{}

Transform::~Transform() {}









///////// functions past here are deprecated except for the static functions

void Transform::rotate(float x, float y, float z) {
	worldMatrix = Quaternion(x, y, z, 1.0f) * worldMatrix;
}

void Transform::rotate(Quaternion& quat) {
	worldMatrix = quat * worldMatrix;
}

void Transform::rotate(Vector3f& v) {
	rotate(v.x, v.y, v.z);
}

void Transform::translate(float x, float y, float z) {

	Matrix4f translation;
	Transform::Translate(translation, x, y, z);

	worldMatrix = translation * worldMatrix;
}

void Transform::translate(Vector3f& v) {
	translate(v.x, v.y, v.z);
}

void Transform::scale(float x) {
	Matrix4f scale = Transform::ScaleMatrix(x);
	worldMatrix = worldMatrix * scale;
}






void Transform::updateWorldMatrix() {
	Matrix4f translate;
	Transform::Translate(translate, position.x, position.y, position.z);
	worldMatrix = rotation * translate * worldMatrix;
}

void Transform::updateLocalWorldMatrix() {
	//localWorldMatrix = localRotation * Translate(localPosition) * Scale(localScale);
}

Matrix4f Transform::asMatrix() {
	return localRotation * Translate(localPosition) * Scale(localScale);
}



void Transform::destroy() {
	delete this;
}







Matrix4f Transform::ScaleMatrix(float scale) {
	return { {scale, 0, 0, 0}, 
			 {0, scale, 0, 0}, 
			 {0, 0, scale, 0}, 
			 {0, 0, 0, 1} };
}

Matrix4f Transform::RotateX(Matrix4f& t, float deg) {
	float rad = glm::radians(deg);
	Matrix4f m4 = { 
					 {1, 0, 0, 0}, 
					 {0, cos(rad), sin(rad), 0}, 
					 {0, -sin(rad), cos(rad), 0}, 
					 {0, 0, 0, 1} 
				  };

	return t = m4 * t;
}

Matrix4f Transform::RotateY(Matrix4f& t, float deg) {
	float rad = glm::radians(deg);
	Matrix4f m4 = { 
					 { cos(rad), 0, -sin(rad), 0},
					 { 0, 1, 0, 0},
					 { sin(rad), 0, cos(rad), 0},
					 { 0, 0, 0, 1}
				  };

	return t = m4 * t;
}

Matrix4f Transform::RotateZ(Matrix4f& t, float deg) {
	float rad = glm::radians(deg);
	Matrix4f m4 = {
					 { cos(rad), sin(rad), 0, 0},
					 { -sin(rad), cos(rad), 0, 0},
					 { 0, 0, 1, 0},
					 { 0, 0, 0, 1} 
				  };

	return t = m4 * t;
}

Matrix4f Transform::Translate(Matrix4f& t, float x, float y, float z) {
	Matrix4f translation = { 
							  {1, 0, 0, 0},
							  {0, 1, 0, 0}, 
							  {0, 0, 1, 0}, 
							  {x, y, z, 1} 
						   };
	return t = translation * t;
}



const Matrix4f Transform::Translate(float x, float y, float z) {
	return { 
		{ 1, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 1, 0 },
		{ x, y, z, 1 } 
	};
}

const Matrix4f Transform::Translate(Vector3f& value) {
	return std::move(Translate(value.x, value.y, value.z));
}

const Matrix4f Transform::Scale(float x, float y, float z) {
	return { 
		{ x, 0, 0, 0 },
		{ 0, y, 0, 0 },
		{ 0, 0, z, 0 },
		{ 0, 0, 0, 1 } 
	};
}

const Matrix4f Transform::Scale(Vector3f& value) {
	return std::move(Scale(value.x, value.y, value.z));
}

const Matrix4f Transform::StripTranslation(Matrix4f const & matrix) {
	return (StripTranslation(matrix, 0.0f));
}

const Matrix4f Transform::StripTranslation(Matrix4f const & matrix, float val) {
	Matrix4f mat = matrix;
	mat[3] = { val, val, val, 1.0f };
	return mat;
}