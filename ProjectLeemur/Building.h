#pragma once

#include "Commons.h"
#include "BaseEntity.h"
#include "Camera.h"
#include "Transform.h"
#include "Material.h"

class Building : public BaseEntity {
private:
	Transform transform;

	UniquePointer<Transform> world;
   
    Vector3f base_center;
    float top_height;
    float x_dim;
    float z_dim;
    
    SharedPointer<Transform> CreateStep(Material*);
    SharedPointer<Transform> CreatePyramid(int, Material*);
    SharedPointer<Transform> CreatePillars(float, Material*);
    SharedPointer<Transform> CreateBox(float, Material*);
    SharedPointer<Transform> CreateCyl(float, Material*);
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