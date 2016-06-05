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

/*selects a random material of a set ofspecific materials */
Material * randomMaterial();

void Building::onCreate() {
	world = unique<Transform>();
    
    base_center = Vector3f(0,0,0);
    up = Vector3f(0,1,0);
    x_dim = 1.5;
    z_dim = 1.5;
    y_dim = 5;
    
    /* used while building */
    top_height = 0;
    current_x_dim = x_dim;
    current_z_dim = z_dim;
    prev_block_height = -1;
    prev_shape = -1;
    prev_type = -1;
    
}

void Building::onStart() {
    srand ((int)time(NULL));
    /* flat layer of buildings */
//    for ( int i = -5; i<5; i++) {
//        for ( int j = -5; j<5; j++) {
//            // reset to add many buildings
//            top_height = 0;
//            current_x_dim = x_dim;
//            current_z_dim = z_dim;
//            prev_block_height = -1;
//            prev_shape = -1;
//            prev_type = -1;
//            transform_group building = CreateRandomBuilding();
//            building->translateLocal(10*i, 0, 10*j);
//            world->addChild(building);
//            std::cout << x_dim << std::endl;
//        }
//    }

    
    // sphere of buildings
    int building_radius = 2;
    int spacing = 2*building_radius;
    int per_sphere = 10;
    float sphere_radius = 40;
    
    for ( double i = 0; i<2*M_PI; i+=2*M_PI/per_sphere) {
        for (double j = M_PI/per_sphere; j<M_PI; j+=M_PI/per_sphere){
//            if (rand() % 4)
//                continue;
            // reset to add many buildings
            top_height = 0;
            current_x_dim = x_dim;
            current_z_dim = z_dim;
            prev_block_height = -1;
            prev_shape = -1;
            prev_type = -1;
            transform_group building = CreateRandomBuilding();
            Vector3f location = Vector3f(sphere_radius*cos(i)*sin(j),
                                         sphere_radius*sin(i)*sin(j),
                                         sphere_radius*cos(j));
            
            transform_group rotated_building = rotatePieceFromUp(building, location);
            rotated_building->translateLocal(location);
            world->addChild(rotated_building);
            }
    }
    
    Component& sph = (ObjObject&) Resources::getEntity(SPH_OBJ);
    game_object planet = share<GameObject>(&sph);
    planet->transform->scaleLocal(sphere_radius);
    world->addChild(planet);
    
    
//    CreateTower();

}

void Building::CreateTower() {
    x_dim = 10;
    z_dim = 8;
    top_height = 0;

    transform_group tower = share<Transform>();
    transform_group a = CreatePyramid(10, randomMaterial(), CUBE);
    tower->addChild(a);

    transform_group b = CreatePillarsVertical(5, randomMaterial(), CUBE);
    tower->addChild(b);

    transform_group c = CreatePyramid(8, randomMaterial(), CYLINDER);
    tower->addChild(c);

    transform_group d = CreateBlock(7, randomMaterial(), CYLINDER);
    tower->addChild(d);
    
    transform_group d1 = CreatePillarsHorizontal(randomMaterial(), CUBE);
    tower->addChild(d1);
    transform_group d2 = CreatePillarsHorizontal(randomMaterial(), CYLINDER);
    tower->addChild(d2);
    transform_group d3 = CreateBlock(2, randomMaterial(), CYLINDER);
    tower->addChild(d3);

    transform_group e = CreatePillarsVertical(3, randomMaterial(), CYLINDER);
    tower->addChild(e);

    transform_group f = CreatePyramid(4, randomMaterial(), CUBE);
    tower->addChild(f);

    transform_group g = CreateBlock(3, randomMaterial(), CUBE);
    tower->addChild(g);
    transform_group g1 = CreatePillarsHorizontal(randomMaterial(), CYLINDER);
    tower->addChild(g1);
    transform_group g2 = CreatePillarsHorizontal(randomMaterial(), CYLINDER);
    tower->addChild(g2);

    transform_group h = CreatePyramid(15, randomMaterial(), CYLINDER);
    tower->addChild(h);

    transform_group i = CreateBlock(2, randomMaterial(), CYLINDER);
    tower->addChild(i);

    transform_group j = CreateDome(randomMaterial());
    tower->addChild(j);
    tower->translateLocal(0, 0, -50);
    world->addChild(tower);
}

/* creates a building with a base, center and a top 
 * the rules:
 * Bot can be type of {BLOCK, PILLARS_V PYRAMID} 1/10 of y_dim
 * Mid can be type of {BLOCK, PILLAR_V, PYRAMID, PILLAR_H} up to 4 middle piece spanning 7/10 of y_dim,
 *  (mid can have multiple mid pieces)
 * Top can be type of {BLOCK, ELLIPSE, PYRAMID} 2/10 of y_dim 
 *  (top may inrease the current x_dim and z_dim to create a lip of a roof)
 *
 * no PILLARS_V can immediately succede PILLARS_V
 * no PILLARS_V can immediately succede PILLARS_H
 * no BLOCK of shape CUBE can immediately succede PYRAMID of shape CYLINDER
 * no BLOCK of shape CUBE can immediately succede BLOCK of shape CYLINDER
 * no PILLAR_H can immediately succede PYRAMID
 * no PILLAR_H can immediately succede PILLAR_V
 * no PYRAMID of shape CUBE can immediately succede PYRAMID of shape CYLINDER
 * no PYRAMID of shape CUBE can immediately succede BLOCK of shape CYLINDER
 * no ELLIPSE can immediately succede PILLARS_H
 *
 * pieces of type of {BLOCK, PILLARS_V, PILLARS_H, PYRAMID} can be shape {CYLINDER, CUBE}
 * 
 *
 *
 * notes:
 * buildings are always built from the bottom up.
 * each Create call builds on top of the next pieces (or on the sides of with PILLARS_H)
 *  and modifies the dimensions of the building
 */
transform_group Building::CreateRandomBuilding() {
    transform_group building = share<Transform>();

    int bot_type = rand()%3;
    int bot_shape = rand()%2;
    float bot_height = y_dim/10;
    
    transform_group bot;
    if (bot_type == 0){ //BLOCK
        bot = CreateBlock(bot_height, randomMaterial(), bot_shape);
    }
    else if (bot_type == 1){ //PILLARS_V
        bot = CreatePillarsVertical(bot_height, randomMaterial(), bot_shape);

    }
    else if (bot_type == 2){ //PYRAMID
        int steps = (int)(bot_height/pyramid_step_height); //height to steps conversion, # of steps rounded down
        bot = CreatePyramid(steps, randomMaterial(), bot_shape);
    }
    building->addChild(bot);

    
    int mid_types[] = {BLOCK, PILLARS_V, PYRAMID, PILLARS_H,PILLARS_H,PILLARS_H,PILLARS_H};
    for (int i = 0, loop = rand()%4+1, remaining_mid_space = 7;
         i<loop && remaining_mid_space > 0;
         i++) {
        int mid_type = rand()%7;
        int mid_shape = rand()%2;
        if (!isValidPlacement(mid_types[mid_type], mid_types[mid_shape])) {
            --i; //repeat the iteration
            continue;
        }
        int how_many_tenths = rand() % remaining_mid_space + 1;;
        int mid_height = y_dim/10*how_many_tenths;
        if (i == loop-1)
            mid_height = y_dim/10*remaining_mid_space;
        
        transform_group mid;
        if (mid_type == 0){ //BLOCK
            mid = CreateBlock(mid_height, randomMaterial(), mid_shape);
        }
        else if (mid_type == 1){ //PILLARS_V
            mid = CreatePillarsVertical(mid_height, randomMaterial(), mid_shape);
            
        }
        else if (mid_type == 2){ //PYRAMID
            int steps = (int)(mid_height/pyramid_step_height); //height to steps conversion, # of steps rounded down
            if (mid_height-steps*pyramid_step_height < 1) {
                --i; //repeat the iteration
                continue;
            }
            mid = CreatePyramid(steps, randomMaterial(), mid_shape);
        }
        else if (mid_type >= 3){ //PILLARS_H
            if (i == loop-1) {
                --i; //repeat the iteration
                continue;
            }
            remaining_mid_space += how_many_tenths; // add the height chang b/c no height change
            mid = CreatePillarsHorizontal(randomMaterial(), mid_shape);
        }
        
        remaining_mid_space -= how_many_tenths;
        building->addChild(mid);
    }
    
    
    int top_type, top_shape;
    int top_types[4] = {BLOCK, ELLIPSE, PYRAMID};

    float top_height = 2*y_dim/10;
    do {
        top_type = rand()%3;
        top_shape = rand()%2;
    } while (!isValidPlacement(top_types[top_type], top_shape));
    
    int lip = rand()%2;
    if (lip && top_type != 1){
        current_x_dim += pyramid_step_height;
        current_z_dim += pyramid_step_height;
    }
    
    transform_group top;
    if (top_type == 0){ //BLOCK
        top = CreateBlock(top_height, randomMaterial(), top_type);
    }
    else if (top_type == 1){ //ELLIPSE
        top = CreateDome(randomMaterial());
        
    }
    else if (top_type == 2){ //PYRAMID
        int steps = (int)(top_height/pyramid_step_height); //height to steps conversion, # of steps rounded down
        top = CreatePyramid(steps, randomMaterial(), top_type);
    }
    building->addChild(top);
    
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


/* adds a x_dim by z_dim by .1 box to building */
transform_group Building::CreateStep(Material* mat, int shape) {
    // if shape is 0 use cube cylinders else
    Component& cube = (shape ?
                       (ObjObject&) Resources::getEntity(CYL_OBJ) :
                       (ObjObject&) Resources::getEntity(CUB_OBJ));
    transform_group parent = share<Transform>();
    game_object step = share<GameObject>(&cube);
    step->setMaterial(mat);
    step->transform->scaleLocal(current_x_dim, pyramid_step_height/2, current_z_dim).translateLocal(0, pyramid_step_height/2, 0);
    parent->addChild(step);
    
    // translate this to the top of the most recent piece
    parent->translateLocal(0, top_height, 0);
    
    // the stairs decreae building space by .1 and increased the height by .2
    top_height          += pyramid_step_height;
    current_x_dim       -= pyramid_step_height/2;
    current_z_dim       -= pyramid_step_height/2;
    
    prev_block_height    = pyramid_step_height;
    prev_shape          = shape;
    prev_type           = PYRAMID;
    
    return parent;
}

/* adds a box to building with a specified height */
transform_group Building::CreateBlock(float height, Material* mat, int shape) {
    // if shape is 0 use cube cylinders else
    Component& block_shape = (shape ?
                              (ObjObject&) Resources::getEntity(CYL_OBJ) :
                              (ObjObject&) Resources::getEntity(CUB_OBJ));    transform_group parent = share<Transform>();
    
    game_object block = share<GameObject>(&block_shape);
    block->setMaterial(mat);
    block->transform->scaleLocal(current_x_dim,height/2,current_z_dim).translateLocal(0, height/2, 0);
    parent->addChild(block);
    
    // translate this to the top of the most recent piece
    parent->translateLocal(0, top_height, 0);
    
    // the box increase the building the height by the prarmeter height
    top_height          += height;
    
    prev_block_height    = height;
    prev_shape          = shape;
    prev_type           = BLOCK;
    return parent;
}

/* adds four pillars to building with a specified height */
transform_group Building::CreatePillarsVertical(float height, Material* mat, int shape) {
    // if shape is 0 use cube cylenders else
    Component& cyl = (shape ?
                      (ObjObject&) Resources::getEntity(CYL_OBJ) :
                      (ObjObject&) Resources::getEntity(CUB_OBJ));
    transform_group parent = share<Transform>();
    
    game_object pillar = share<GameObject>(&cyl);
    pillar->setMaterial(mat);
    float pillar_radius = std::min(current_x_dim, current_z_dim)/4;
    pillar->transform->scaleLocal(pillar_radius, height/2, pillar_radius)
    .translateLocal(current_x_dim/2, height/2, current_z_dim/2);
    parent->addChild(pillar);
    
    game_object pillar1 = share<GameObject>(&cyl);
    pillar1->setMaterial(mat);
    pillar1->transform->scaleLocal(pillar_radius, height/2, pillar_radius)
    .translateLocal(-current_x_dim/2, height/2, current_z_dim/2);
    parent->addChild(pillar1);
    
    game_object pillar2 = share<GameObject>(&cyl);
    pillar2->setMaterial(mat);
    pillar2->transform->scaleLocal(pillar_radius, height/2, pillar_radius)
    .translateLocal(-current_x_dim/2, height/2, -current_z_dim/2);
    parent->addChild(pillar2);
    
    game_object pillar3 = share<GameObject>(&cyl);
    pillar3->setMaterial(mat);
    pillar3->transform->scaleLocal(pillar_radius,height/2,pillar_radius)
    .translateLocal(current_x_dim/2, height/2, -current_z_dim/2);
    parent->addChild(pillar3);
    
    // translate this to the top of the most recent piece
    parent->translateLocal(0, top_height, 0);
    
    // the pillairs increase the building the height by the prarmeter height
    top_height          += height;
    
    prev_block_height    = height;
    prev_shape          = shape;
    prev_type           = PILLARS_V;
    
    return parent;
}

/* makes horizontal pillars extend from sides of current block increasing xdim and ydim
 * change in xdim and ydim are 1/2 the height_of_prev
 * the height_of_prev is the height of the most recent piece
 */
// NOTE: this can violate and cause the rest of the building to violate the specified x_dim and z_dim
// safeties can be added but without them buildings are more interesting
SharedPointer<Transform> Building::CreatePillarsHorizontal(Material* mat, int shape) {
    // if shape is 0 use cube cylenders else
    Component& cyl = (shape ?
                      (ObjObject&) Resources::getEntity(CYL_OBJ) :
                      (ObjObject&) Resources::getEntity(CUB_OBJ));
    
    float pillar_diameter = std::min(std::min(prev_block_height/2,current_x_dim/2), current_z_dim/2);
    
    float x_dim_translation = current_x_dim+pillar_diameter/2;
    float z_dim_translation = current_z_dim+pillar_diameter/2;
    float y_dim_translation = top_height-pillar_diameter/2;
    
    float xz_scale = pillar_diameter/2;
    float y_scale = pillar_diameter/2;
    
    transform_group parent = share<Transform>();
    
    // pillars on the z axis
    game_object pillar_z0 = share<GameObject>(&cyl);
    pillar_z0->setMaterial(mat);
    pillar_z0->transform->scaleLocal(xz_scale, y_scale, xz_scale);
    transform_group rotated_pillar_z0 = rotatePieceFromUp(pillar_z0, Vector3f(0,0,1));
    rotated_pillar_z0->translateLocal(0, y_dim_translation, z_dim_translation);
    parent->addChild(rotated_pillar_z0);
    
    game_object pillar_z1 = share<GameObject>(&cyl);
    pillar_z1->setMaterial(mat);
    pillar_z1->transform->scaleLocal(xz_scale, y_scale, xz_scale);
    transform_group rotated_pillar_z1 = rotatePieceFromUp(pillar_z1, Vector3f(0,0,-1));
    rotated_pillar_z1->translateLocal(0, y_dim_translation, -z_dim_translation);
    parent->addChild(rotated_pillar_z1);
    
    // pillars on the z axis
    game_object pillar_x0 = share<GameObject>(&cyl);
    pillar_x0->setMaterial(mat);
    pillar_x0->transform->scaleLocal(xz_scale, y_scale, xz_scale);
    transform_group rotated_pillar_x0 = rotatePieceFromUp(pillar_x0, Vector3f(1,0,0));
    rotated_pillar_x0->translateLocal(x_dim_translation, y_dim_translation, 0);
    parent->addChild(rotated_pillar_x0);
    
    game_object pillar_x1 = share<GameObject>(&cyl);
    pillar_x1->setMaterial(mat);
    pillar_x1->transform->scaleLocal(xz_scale, y_scale, xz_scale);
    transform_group rotated_pillar_x1 = rotatePieceFromUp(pillar_x1, Vector3f(-1,0, 0));
    rotated_pillar_x1->translateLocal(-x_dim_translation, y_dim_translation, 0);
    parent->addChild(rotated_pillar_x1);
    
    
    current_x_dim       += pillar_diameter;
    current_z_dim       += pillar_diameter;
    
    prev_block_height    = pillar_diameter;
    prev_shape          = shape;
    prev_type           = PILLARS_H;
    
    return parent;
}

transform_group Building::CreatePyramid(int steps, Material* mat, int shape) {
    transform_group parent = share<Transform>();
    
    for (int i = 0; i<steps; i++){
        // chck if there is enough space for another step
        if (current_x_dim < pyramid_step_height/2 && current_z_dim < pyramid_step_height/2)
            break;
        transform_group step = CreateStep(mat, shape);
        parent->addChild(step);
    }
    
    // building variables set within each step
    prev_block_height   = steps*pyramid_step_height;
    prev_shape          = shape;
    prev_type           = PYRAMID;
    return parent;
}

transform_group Building::CreateDome(Material* mat){
    Component& sph = (ObjObject&) Resources::getEntity(SPH_OBJ);
    transform_group parent = share<Transform>();
    
    game_object cylender = share<GameObject>(&sph);
    cylender->setMaterial(mat);
    cylender->transform->scaleLocal(current_x_dim,prev_block_height/2,current_z_dim);
    //.translateLocal(0, height/2, 0); no translate because we want half the sphere hidden
    // in the previous object
    parent->addChild(cylender);
    
    // translate this to the top of the most recent piece
    parent->translateLocal(0, top_height, 0);
    
    // the box increase the building the height by the prarmeter height
    top_height          += prev_block_height/2;
    
    prev_shape          = SPHERE;
    prev_type           = ELLIPSE;
    return parent;
}


transform_group Building::rotatePieceFromUp(transform_group piece, Vector3f new_up) {
    new_up = normalize(new_up);
    Vector3f rotation_axis = cross(new_up, up);
    float rotation_degree = acos(dot(up, new_up)/(length(up)*length(new_up))) * 360 / (2*M_PI);
    
    transform_group unrotated_piece = share<Transform>();
    unrotated_piece->addChild(piece);
    unrotated_piece->rotateLocal(rotation_axis.x, rotation_axis.y, rotation_axis.z, rotation_degree);
    
    transform_group rotated_piece = share<Transform>();
    rotated_piece->addChild(unrotated_piece);
    return rotated_piece;
}

transform_group Building::rotatePieceFromUp(game_object piece, Vector3f new_up) {
    new_up = normalize(new_up);
    Vector3f rotation_axis = cross(new_up, up);
    float rotation_degree = acos(dot(up, new_up)/(length(up)*length(new_up))) * 360 / (2*M_PI);
    
    transform_group unrotated_piece = share<Transform>();
    unrotated_piece->addChild(piece);
    unrotated_piece->rotateLocal(rotation_axis.x, rotation_axis.y, rotation_axis.z, rotation_degree);
    
    transform_group rotated_piece = share<Transform>();
    rotated_piece->addChild(unrotated_piece);
    return rotated_piece;
}

/* checks these
 * no PILLARS_V can immediately succede PILLARS_V
 * no PILLARS_V can immediately succede PILLARS_H
 * no BLOCK of shape CUBE can immediately succede PYRAMID of shape CYLINDER
 * no BLOCK of shape CUBE can immediately succede BLOCK of shape CYLINDER
 * no PILLAR_H can immediately succede PYRAMID
 * no PILLAR_H can immediately succede PILLAR_V
 * no PYRAMID of shape CUBE can immediately succede PYRAMID of shape CYLINDER
 * no PYRAMID of shape CUBE can immediately succede BLOCK of shape CYLINDER
 * no ELLIPSE can immediately succede PILLARS_H
 */
bool Building::isValidPlacement(int next_type,int next_shape) {
    if (next_type == PILLARS_V && prev_type == PILLARS_V)
        return false;
    else if (next_type == PILLARS_V && prev_type == PILLARS_H)
        return false;
    else if (next_type == BLOCK && next_shape == CUBE &&
             prev_type == PYRAMID && prev_shape == CYLINDER)
        return false;
    else if (next_type == BLOCK && next_shape == CUBE &&
             prev_type == BLOCK && prev_shape == CYLINDER)
        return false;
    else if (next_type == PILLARS_H && prev_type == PYRAMID)
        return false;
    else if (next_type == PILLARS_H && prev_type == PILLARS_V)
        return false;
    else if (next_type == PYRAMID && next_shape == CUBE &&
             prev_type == PYRAMID && prev_shape == CYLINDER)
        return false;
    else if (next_type == PYRAMID && next_shape == CUBE &&
             prev_type == BLOCK && prev_shape == CYLINDER)
        return false;
    else if (next_type == ELLIPSE && prev_type == PILLARS_H)
        return false;
    else
        return true;
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