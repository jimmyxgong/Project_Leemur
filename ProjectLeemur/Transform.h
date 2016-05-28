#pragma once

#include "Commons.h"
#include "Quaternion.h"
#include "Entity.h"

#include <list>

class Transform {
private:
	/* Orientation relative to a parent's world matrix */
	Vector3f    localScale;
	Vector3f	localPosition;
	Quaternion  localRotation;

	/* Orientation in world matrix */
	Vector3f	scaling;
	Vector3f	position;
	Quaternion	rotation;

	/* Point in each respective space to another space*/
	Matrix4f	localToWorldMatrix;
	Matrix4f	worldToLocalMatrix;

	/* Children used in Scene Graph */
	std::list<Transform *> children;
	Transform * parent;
	Entity * component;

public:	/* Getters and Setters */

	Matrix4f    worldMatrix;

	Vector3f getLocalScale() const;
	Vector3f getLocalPosition() const;
	Quaternion getLocalRotation() const;

	Vector3f getPosition() const;
	Quaternion getRotation() const;

	Transform& setLocalScale(const Vector3f & val);
	Transform& setLocalPosition(const Vector3f & val);
	Transform& setLocalRotation(const Quaternion & val);

	Transform& setPosition(const Vector3f & val);
	Transform& setRotation(const Quaternion & val);

public: /* Transformations on current orientation */

	Transform& scaleLocal(float xyz);
	Transform& scaleLocal(float x, float y, float z);
	Transform& scaleLocal(const Vector3f & val);

	Transform& rotateLocalX(float deg);
	Transform& rotateLocalY(float deg);
	Transform& rotateLocalZ(float deg);
	Transform& rotateLocal(float x, float y, float z, float deg);
	Transform& rotateLocal(Quaternion & val);

	Transform& translateLocal(float x, float y, float z);
	Transform& translateLocal(const Vector3f & val);


public: /* Constructors */
	/* Copy Constructor */
	Transform(const Transform &);	
	Transform();
	~Transform();

public: /* Scene Graph */

	// Get's the parent's localToWorldMatrix and update recursively
	// itself and all of its children just like a regular scene graph
	// node would.
	Transform& locallyUpdate(const Matrix4f & val = Matrix4f(1.0f));

	Transform& resetScale();
	Transform& resetPosition();
	Transform& resetRotation();

	// Reset orientation to base orientation
	Transform& reset();

	// The corresponding Entity that this Transform is representing.
	Transform& attachEntity(Entity * entity);

	// Add a child to the Transform's children.
	Transform& addChild(Transform * transform);

public:
	bool hasChanged();


	void rotate(float x, float y, float z);
	void rotate(Quaternion& quat);
	void rotate(Vector3f& vec);

	void translate(float x, float y, float z);
	void translate(Vector3f& vec);

	void scale(float x);
	






	void updateWorldMatrix();
	void updateLocalWorldMatrix();
	Matrix4f asMatrix();


	//Matrix4f getLocalToWorldMatrix();
	Vector3f getScale();


public:





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