#pragma once

#include "Commons.h"
#include "BaseEntity.h"
#include "Camera.h"
#include "Transform.h"
#include "Material.h"

//shapes
#define CUBE 0
#define CYLINDER 1
#define SPHERE 2

//types
#define BLOCK 0
#define ELLIPSE 1
#define PILLARS_V 2
#define PILLARS_H 3
#define PYRAMID 4


class Building : public BaseEntity {
private:
	Transform transform;

	UniquePointer<Transform> world;
   
    Vector3f base_center, up;
    float x_dim, z_dim, y_dim; //general dimensions of building
    
    //used for building
    float top_height, current_x_dim, current_z_dim;
    float prev_block_height;
    int prev_shape;
    int prev_type;
    float pyramid_step_height = .1;

    
    SharedPointer<Transform> CreateStep(Material*, int shape);
    SharedPointer<Transform> CreatePyramid(int steps, Material*, int shape);
    SharedPointer<Transform> CreatePillarsVertical(float height, Material*, int shape);
    SharedPointer<Transform> CreateBlock(float height, Material*, int shape);
    SharedPointer<Transform> CreateDome(Material*);
    SharedPointer<Transform> CreatePillarsHorizontal(Material*, int shape);

    SharedPointer<Transform> CreateRandomBuilding();
    void CreateTower();

    SharedPointer<Transform> rotatePieceFromUp(SharedPointer<Transform>, Vector3f);
    SharedPointer<Transform> rotatePieceFromUp(SharedPointer<GameObject>, Vector3f);
    bool isValidPlacement(int,int);
    
public:

	void onCreate() override;
	void onStart() override;
	void onRender() override;
	void onUpdate() override;

private:
	void onDestroy() override;

public:
	Building();
    Building(Vector3f pos, Vector3f dim);
};