#ifndef TRANSFORM_H
#define TRANSFORM_H


#include "Commons.h"
#include "Quaternion.h"

class Transform {
	
private:
	

public:	
	Vector3f    localScale;
	Vector3f    localPosition;
	Quaternion  localRotation;
	Matrix4f    localWorldMatrix;

	Vector3f    scaling;
	Vector3f    position;
	Matrix4f    rotation;
	Matrix4f    worldMatrix;

public:


	void rotate(float x, float y, float z);
	void rotate(Quaternion& quat);
	void rotate(Vector3f& vec);

	void translate(float x, float y, float z);
	void translate(Vector3f& vec);

	void scale(float x);

	Transform& scaleLocal(float xyz);
	Transform& scaleLocal(float x, float y, float z);
	Transform& scaleLocal(Vector3f const &);

	Transform& rotateLocal(float x, float y, float z, float deg);
	Transform& rotateLocal(Quaternion&);
	//Transform& setRotation(Matrix4f const &);

	Transform& translateLocal(float x, float y, float z);
	Transform& translateLocal(Vector3f const &);

	void updateWorldMatrix();
	void updateLocalWorldMatrix();
	Matrix4f asMatrix();



public:
	Transform()
		: 
	      localPosition(0.0f, 0.0f, 0.0f),
		  localScale(1.0f),
		  position(0.0f, 0.0f, 0.0f),
		  localWorldMatrix(1.0f) {}
	~Transform();

	void destroy();






	/**
	* Creates and returns the viewport matrix provided with sizes of the screen.
	*
	* originX = base origin coordinate x of the viewport
	* originY = base origin coordinate y of the viewport
	* width = width of the viewport
	* height = height of the viewport
	*/
	static Matrix4f Viewport(int originX, int originY, int width, int height);

	static Matrix4f Orthographic(int, int, int, int, int, int);

	// The View matrix in the ModelView 
	static Matrix4f CameraMatrix(float, Vector3f, Vector3f);

	// Perspective frustum
	static Matrix4f Frustum(float fov, float ratio, float near, float far);
	static Matrix4f Frustum(int, int, int, int, int, int);

	// Create a scalable matrix
	static Matrix4f ScaleMatrix(float);

	// Apply a scaled matrix to input matrix with provided float
	static Matrix4f Scale(Matrix4f& input, float);
	
	// Rotate input matrix about the pitch axis
	static Matrix4f RotateX(Matrix4f&, float);

	// Rotate input matrix about the yaw axis
	static Matrix4f RotateY(Matrix4f&, float);

	// Rotate input matrix about the roll axis
	static Matrix4f RotateZ(Matrix4f&, float);

	// Translate a given matrix by x, y, z
	static Matrix4f Translate(Matrix4f&, float x, float y, float z);


	const static Matrix4f Translate(float x, float y, float z);
	const static Matrix4f Translate(Vector3f&);

	const static Matrix4f Scale(float x, float y, float z);
	const static Matrix4f Scale(Vector3f&);

	const static Matrix4f StripTranslation(Matrix4f const &);
	const static Matrix4f StripTranslation(Matrix4f const &, float val);
};

#endif // TRANSFORM_H