#pragma once

#include "Commons.h"
#include "BaseEntity.h"
#include "Camera.h"
#include "Transform.h"

class Building : public BaseEntity {
private:
	Transform transform;

	UniquePointer<Transform> world;
   
    Vector3f base_center;
    float top_height;
    float x_dim;
    float z_dim;
    
    SharedPointer<Transform> CreateStep();
    SharedPointer<Transform> CreatePyramid(int);
    SharedPointer<Transform> CreatePillars(float);
    SharedPointer<Transform> CreateBox(float);
    SharedPointer<Transform> CreateCyl(float);
    SharedPointer<Transform> CreateRandomBuilding(float, float);

    SharedPointer<Transform> CreateMiddle();
    SharedPointer<Transform> CreateTop();


public:

	void onCreate() override;
	void onStart() override;
	void onRender() override;
	void onUpdate() override;

private:
	void onDestroy() override;

public:
	Building();
};