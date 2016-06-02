#include "Building.h"
#include "Environment.h"
#include "GameObject.h"
#include "Transform.h"
#include "Quaternion.h"
#include "Time.h"
#include "Light.h"

#include "Resources.h"
#include "ObjObject.h"

#include <random>

typedef SharedPointer<Transform>		transform_group;
typedef SharedPointer<GameObject>		game_object;

void Building::onCreate() {
	world = unique<Transform>();
    base_center = Vector3f(0, 0,0);
    top_height = 0;
    x_dim = 2.5;
    z_dim = 2.5;
}
/* adds a x_dim by z_dim by .1 box to building */
transform_group Building::CreateStep() {
    static Component& cube = (ObjObject&) Resources::getEntity(CUB_OBJ);
    transform_group parent = share<Transform>();
    game_object step = share<GameObject>(&cube);
    step->transform->scaleLocal(x_dim, .1, z_dim).translateLocal(0, .1, 0);
    parent->addChild(step);
    
    // translate this to the top of the most recent piece
    parent->translateLocal(0, top_height, 0);
    
    // the stairs decreae building space by .1 and increased the height by .2
    x_dim       -= .1;
    z_dim       -= .1;
    top_height  += .2;
    
    return parent;
}

/* adds a box to building with a specified height */
transform_group Building::CreateBox(float height) {
    static Component& cube = (ObjObject&) Resources::getEntity(CUB_OBJ);
    transform_group parent = share<Transform>();
    
    game_object box = share<GameObject>(&cube);
    box->transform->scaleLocal(x_dim,height/2,x_dim).translateLocal(0, height/2, 0);
    parent->addChild(box);
    
    // translate this to the top of the most recent piece
    parent->translateLocal(0, top_height, 0);
    
    // the box increase the building the height by the prarmeter height
    top_height  += height;
    
    return parent;
}

/* adds a box to building with a specified height */
transform_group Building::CreateCyl(float height) {
    static Component& cyl = (ObjObject&) Resources::getEntity(CYL_OBJ);
    transform_group parent = share<Transform>();
    
    game_object cylender = share<GameObject>(&cyl);
    cylender->transform->scaleLocal(x_dim,height/2,x_dim).translateLocal(0, height/2, 0);
    parent->addChild(cylender);
    
    // translate this to the top of the most recent piece
    parent->translateLocal(0, top_height, 0);
    
    // the box increase the building the height by the prarmeter height
    top_height  += height;
    
    return parent;
}

/* adds four pillars to building with a specified height */
transform_group Building::CreatePillars(float height) {
    static Component& cyl = (ObjObject&) Resources::getEntity(CYL_OBJ);
    transform_group parent = share<Transform>();
    
    game_object pillar = share<GameObject>(&cyl);
    pillar->transform->scaleLocal(x_dim/4,height/2,x_dim/4).translateLocal(x_dim/2, height/2, z_dim/2);
    parent->addChild(pillar);
    game_object pillar1 = share<GameObject>(&cyl);
    pillar1->transform->scaleLocal(x_dim/4,height/2,x_dim/4).translateLocal(-x_dim/2, height/2, z_dim/2);
    parent->addChild(pillar1);
    game_object pillar2 = share<GameObject>(&cyl);
    pillar2->transform->scaleLocal(x_dim/4,height/2,x_dim/4).translateLocal(-x_dim/2, height/2, -z_dim/2);
    parent->addChild(pillar2);
    game_object pillar3 = share<GameObject>(&cyl);
    pillar3->transform->scaleLocal(x_dim/4,height/2,x_dim/4).translateLocal(x_dim/2, height/2, -z_dim/2);
    parent->addChild(pillar3);
    
    // translate this to the top of the most recent piece
    parent->translateLocal(0, top_height, 0);
    
    // the pillairs increase the building the height by the prarmeter height
    top_height  += height;
    
    return parent;
}

transform_group Building::CreatePyramid(int steps) {
    transform_group parent = share<Transform>();

    for (int i = 0; i<steps; i++){
        // chck if there is enough space for another step
        if (x_dim < .1 && z_dim < .1)
            break;
        transform_group step = CreateStep();
        parent->addChild(step);
    }
    
    return parent;
}


void Building::onStart() {
    srand ((int)time(NULL));
    for ( int i = -5; i<5; i++) {
        for ( int j = -5; j<5; j++) {
            transform_group building = CreateRandomBuilding(1, 1);
            building->translateLocal(5*i, 0, 5*j);
            world->addChild(building);
        }
    }
}
/* creates a building with a base, center and a top */
transform_group Building::CreateRandomBuilding(float x_radius, float z_radius) {
    // zero height
    top_height = 0;
    // set the boundaries
    x_dim = x_radius;
    z_dim = z_radius;
    transform_group building = share<Transform>();

    /* a bottom can be a
     * pyramid with less than more than 2 but less than 5 steps
     * a box with height more .5 less than 2.5;
     * pillars with heigh more .5 less than 2.5;
     */
    int base = rand() % 3;
    int random;
    if (base == 0) {
        random = rand() % 3 + 3;
        transform_group pyramid = CreatePyramid(random);
        building->addChild(pyramid);
        std::cout << "Base is pyramid with " << random << " steps" << std::endl;
    }
    else if (base == 1){
        random = rand() % 100 + 1;
        transform_group box = CreateBox(.5+2./random);
        building->addChild(box);
        std::cout << "Base is box with " << .5+2./random << " height" << std::endl;
    }
    else if (base == 2){
        random = rand() % 100 + 1;
        transform_group pillars = CreatePillars(.5+2./random);
        building->addChild(pillars);
        std::cout << "Base is pillars with " << .5+2./random << " height" << std::endl;
    }
    // reduce the dimensions so things look like they fit on top
    x_dim -= .1;
    z_dim -= .1;
    
    /* a middle can be a
     * a box with height greater than 1 less than 6
     * pillars with height greater than 1 less than 6, and if base is not pillars
     * a cylender with height greater than 1 less than 6
     */
    for (int i = 0, loop = rand()%3+1; i < loop; i++) {  // up to three middle pieces
        int middle = rand() % 3;
        if (middle == 0){
            random = rand() % 100 + 1;
            transform_group box = CreateBox(1+5./random);
            building->addChild(box);
            std::cout << "Middle is box with " << 1+5./random << " height" << std::endl;
        }
        else if (middle == 1){
            if (base == 2) {
                --i; //repeat this iteration
                continue;
            }
            random = rand() % 100 + 1;
            transform_group pillars = CreatePillars(1+5./random);
            building->addChild(pillars);
            std::cout << "Middle is pillars with " << 1+5./random << " height" << std::endl;
        }
        else if (middle == 2){
            random = rand() % 100 + 1;
            transform_group cyl = CreateCyl(1+5./random);
            building->addChild(cyl);
            std::cout << "Middle is cylender with " << 1+5./random << " height" << std::endl;
        }
    }
    
    /* a top can be a
     * pyramid with more than 2 less than 10 steps
     * a box with a height greater than .5 and less than 1
     */
    // increase the dimensions so top goes beyond middle boundaries
    x_dim += .1;
    z_dim += .1;
    int top = rand() % 2;
    if (top == 0) {
        random = rand() % 8 + 3;
        transform_group pyramid = CreatePyramid(random);
        building->addChild(pyramid);
        std::cout << "Top is pyramid with " << random << " steps" << std::endl;
    }
    else if (top == 1){
        random = rand() % 100 + 1;
        transform_group box = CreateBox(.5+.5/random);
        building->addChild(box);
        std::cout << "Top is box with " << .5+.5/random << " height" << std::endl;

    }
    
    return building;
}

void Building::onRender() {
	Light::Directional.loadToShader();
	world->renderAll();
}

void Building::onUpdate() {
	world->updateAll();
}


void Building::onDestroy() {
	
}

Building::Building() {
	onCreate();
}