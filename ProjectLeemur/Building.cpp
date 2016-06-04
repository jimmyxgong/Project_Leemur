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
Material * randomMaterial();

void Building::onCreate() {
	world = unique<Transform>();
    base_center = Vector3f(0, 0,0);
    top_height = 0;
    x_dim = 2.5;
    z_dim = 2.5;
}
/* adds a x_dim by z_dim by .1 box to building */
transform_group Building::CreateStep(Material* mat, int shape) {
    // if shape is 0 use cube cylenders else
    Component& cube = (shape ?
                             (ObjObject&) Resources::getEntity(CYL_OBJ) :
                             (ObjObject&) Resources::getEntity(CUB_OBJ));
    transform_group parent = share<Transform>();
    game_object step = share<GameObject>(&cube);
    step->setMaterial(mat);
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
transform_group Building::CreateBox(float height, Material* mat) {
    Component& cube = (ObjObject&) Resources::getEntity(CUB_OBJ);
    transform_group parent = share<Transform>();
    
    game_object box = share<GameObject>(&cube);
    box->setMaterial(mat);
    box->transform->scaleLocal(x_dim,height/2,z_dim).translateLocal(0, height/2, 0);
    parent->addChild(box);
    
    // translate this to the top of the most recent piece
    parent->translateLocal(0, top_height, 0);
    
    // the box increase the building the height by the prarmeter height
    top_height  += height;
    
    return parent;
}

/* adds a box to building with a specified height */
transform_group Building::CreateCyl(float height, Material* mat) {
    Component& cyl = (ObjObject&) Resources::getEntity(CYL_OBJ);
    transform_group parent = share<Transform>();
    
    game_object cylender = share<GameObject>(&cyl);
    cylender->setMaterial(mat);
    cylender->transform->scaleLocal(x_dim,height/2,z_dim).translateLocal(0, height/2, 0);
    parent->addChild(cylender);
    
    // translate this to the top of the most recent piece
    parent->translateLocal(0, top_height, 0);
    
    // the box increase the building the height by the prarmeter height
    top_height  += height;
    
    return parent;
}

/* adds four pillars to building with a specified height */
transform_group Building::CreatePillars(float height, Material* mat, int shape) {
    // if shape is 0 use cube cylenders else
    Component& cyl = (shape ?
                             (ObjObject&) Resources::getEntity(CYL_OBJ) :
                             (ObjObject&) Resources::getEntity(CUB_OBJ));
    transform_group parent = share<Transform>();
    
    game_object pillar = share<GameObject>(&cyl);
    pillar->setMaterial(mat);
    float pillar_radius = std::min(x_dim, z_dim)/4;
    pillar->transform->scaleLocal(pillar_radius, height/2, pillar_radius)
    .translateLocal(x_dim/2, height/2, z_dim/2);
    parent->addChild(pillar);
    
    game_object pillar1 = share<GameObject>(&cyl);
    pillar1->setMaterial(mat);
    pillar1->transform->scaleLocal(pillar_radius, height/2, pillar_radius)
    .translateLocal(-x_dim/2, height/2, z_dim/2);
    parent->addChild(pillar1);
    
    game_object pillar2 = share<GameObject>(&cyl);
    pillar2->setMaterial(mat);
    pillar2->transform->scaleLocal(pillar_radius, height/2, pillar_radius)
    .translateLocal(-x_dim/2, height/2, -z_dim/2);
    parent->addChild(pillar2);
    
    game_object pillar3 = share<GameObject>(&cyl);
    pillar3->setMaterial(mat);
    pillar3->transform->scaleLocal(pillar_radius,height/2,pillar_radius)
    .translateLocal(x_dim/2, height/2, -z_dim/2);
    parent->addChild(pillar3);
    
    // translate this to the top of the most recent piece
    parent->translateLocal(0, top_height, 0);
    
    // the pillairs increase the building the height by the prarmeter height
    top_height  += height;
    
    return parent;
}

transform_group Building::CreatePyramid(int steps, Material* mat, int shape) {
    transform_group parent = share<Transform>();

    for (int i = 0; i<steps; i++){
        // chck if there is enough space for another step
        if (x_dim < .1 && z_dim < .1)
            break;
        transform_group step = CreateStep(mat, shape);
        parent->addChild(step);
    }
    
    return parent;
}

transform_group Building::CreateDome(float height, Material* mat){
    Component& sph = (ObjObject&) Resources::getEntity(SPH_OBJ);
    transform_group parent = share<Transform>();
    
    game_object cylender = share<GameObject>(&sph);
    cylender->setMaterial(mat);
    cylender->transform->scaleLocal(x_dim,height/2,z_dim);
    //.translateLocal(0, height/2, 0); no translate because we want half the sphere hidden
    // in the previous object
    parent->addChild(cylender);
    
    // translate this to the top of the most recent piece
    parent->translateLocal(0, top_height, 0);
    
    // the box increase the building the height by the prarmeter height
    top_height  += height;
    return parent;
}


void Building::onStart() {
    srand ((int)time(NULL));
    /* flat layer of buildings */
    for ( int i = -5; i<5; i++) {
        for ( int j = -5; j<5; j++) {
            transform_group building = CreateRandomBuilding(rand()%4+1, rand()%4+1);
            building->translateLocal(8*i, 0, 8*j);
            world->addChild(building);
        }
    }

    
//    // sphere of buildings
//    int building_radius = 2;
//    int spacing = 2*building_radius;
//    int per_sphere = 10;
//    float sphere_radius = 40;
//    
//    for ( double i = 0; i<2*M_PI; i+=2*M_PI/per_sphere) {
//        for (double j = M_PI/per_sphere; j<M_PI; j+=M_PI/per_sphere){
////            if (rand() % 4)
////                continue;
//            
//            transform_group building = CreateRandomBuilding(rand()%building_radius+1,
//                                                            rand()%building_radius+1);
//            Vector3f location = Vector3f(sphere_radius*cos(i)*sin(j),
//                                         sphere_radius*sin(i)*sin(j),
//                                         sphere_radius*cos(j));
//            
//            Vector3f up = Vector3f(0,1,0);
//            Vector3f new_up = normalize(location);
//            Vector3f rotation_axis = cross(new_up, up);
//            float rotation_degree = acos(dot(up, new_up)/(length(up)*length(new_up))) * 360 / (2*M_PI);
//            building->rotateLocal(rotation_axis.x, rotation_axis.y, rotation_axis.z, rotation_degree);
//            transform_group rotated_building = share<Transform>();
//            rotated_building->addChild(building);
//            rotated_building->translateLocal(location);
//            world->addChild(rotated_building);
//            }
//    }
//    
//    Component& sph = (ObjObject&) Resources::getEntity(SPH_OBJ);
//    game_object planet = share<GameObject>(&sph);
//    planet->transform->scaleLocal(sphere_radius);
//    world->addChild(planet);
 
    
    CreateTower();
}

void Building::CreateTower() {
    x_dim = 10;
    z_dim = 8;
    top_height = 0;

    transform_group tower = share<Transform>();
    for (int z = 0; z <2; z++){
        transform_group a = CreatePyramid(10, randomMaterial(), 0);
        tower->addChild(a);

        transform_group b = CreatePillars(5, randomMaterial(), 0);
        tower->addChild(b);

        transform_group c = CreatePyramid(8, randomMaterial(), 1);
        tower->addChild(c);

        transform_group d = CreateCyl(7, randomMaterial());
        tower->addChild(d);

        transform_group e = CreatePillars(3, randomMaterial(), 1);
        tower->addChild(e);

        transform_group f = CreatePyramid(6, randomMaterial(), 0);
        tower->addChild(f);

        transform_group g = CreateBox(3, randomMaterial());
        tower->addChild(g);

        transform_group h = CreatePyramid(4, randomMaterial(), 1);
        tower->addChild(h);

        transform_group i = CreateCyl(2, randomMaterial());
        tower->addChild(i);
    }

    transform_group j = CreateDome(2, randomMaterial());
    tower->addChild(j);
    tower->translateLocal(0, 0, -50);
    world->addChild(tower);
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
        transform_group pyramid = CreatePyramid(random, randomMaterial(), random%2);
        building->addChild(pyramid);
        std::cout << "Base is pyramid with " << random << " steps" << std::endl;
    }
    else if (base == 1){
        random = rand() % 100 + 1;
        transform_group box = CreateBox(.5+2./random, randomMaterial());
        building->addChild(box);
        std::cout << "Base is box with " << .5+2./random << " height" << std::endl;
    }
    else if (base == 2){
        random = rand() % 100 + 1;
        transform_group pillars = CreatePillars(.5+2./random, randomMaterial(), random%2);
        building->addChild(pillars);
        std::cout << "Base is pillars with " << .5+2./random << " height" << std::endl;
    }
    // reduce the dimensions so things look like they fit on top
    x_dim -= .1;
    z_dim -= .1;
    
    /* a middle can be a
     * a box with height greater than 3 less than 8
     * pillars with height greater than 3 less than 8, and if base is not pillars
     * a cylender with height greater than 3 less than 8
     */
    int middle, middle_height = 0;
    for (int i = 0, loop = rand()%3+1; i < loop; i++) {  // up to three middle pieces
        middle = rand() % 3;
        random = rand() % 100 + 1;
        middle_height = 3+5./random;
        if (middle == 0){
            transform_group box = CreateBox(middle_height, randomMaterial());
            building->addChild(box);
            std::cout << "Middle is box with " << middle_height << " height" << std::endl;
        }
        else if (middle == 1){
            if (base == 2) {
                --i; //repeat this iteration
                continue;
            }
            random = rand() % 100 + 1;
            transform_group pillars = CreatePillars(middle_height, randomMaterial(), random%2);
            building->addChild(pillars);
            std::cout << "Middle is pillars with " << middle_height << " height" << std::endl;
        }
        else if (middle == 2){
            random = rand() % 100 + 1;
            transform_group cyl = CreateCyl(middle_height, randomMaterial());
            building->addChild(cyl);
            std::cout << "Middle is cylender with " << middle_height << " height" << std::endl;
        }
    }
    
    /* a top can be a
     * pyramid with more than 2 less than 10 steps
     * a box with a height greater than .5 and less than 1
     * a dome with a height of 1/2 the hieght of the last middle piece
     */
    // increase the dimensions so top goes beyond middle boundaries
    x_dim += .1;
    z_dim += .1;
    int top = rand() % 3;
    if (top == 0) {
        random = rand() % 8 + 3;
        transform_group pyramid = CreatePyramid(random, randomMaterial(), random%2);
        building->addChild(pyramid);
        std::cout << "Top is pyramid with " << random << " steps" << std::endl;
    }
    else if (top == 1){
        random = rand() % 100 + 1;
        transform_group box = CreateBox(.5+.5/random, randomMaterial());
        building->addChild(box);
        std::cout << "Top is box with " << .5+.5/random << " height" << std::endl;
    }
    else if (top == 2){
        x_dim -= .1; // dont want it to petrude
        z_dim -= .1;
        transform_group dome = CreateDome(middle_height, randomMaterial());
        building->addChild(dome);
        std::cout << "Top is dome with " << middle_height << " height" << std::endl;
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


Material * randomMaterial() {
    Material * toReturn;
    int random = rand() % 4;
    if (random == 0) toReturn =  &Material::RedPlastic;
    else if (random == 1) toReturn = &Material::Bronze;
    else if (random == 2) toReturn = &Material::Gold;
    else if (random == 3) toReturn = &Material::Silver;
    
    return toReturn;
}