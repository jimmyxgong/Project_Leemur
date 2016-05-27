#ifndef ENTITY_H
#define ENTITY_H

class Entity {
public:
	bool active = false;
	virtual bool isActive() { return active; }

	// Initialize objects here. Called onCreation of the object
	virtual void onCreate() = 0;

	// Utilizes functions after the object has been successfully created.
	virtual void onStart() = 0;

	// Method to render the object onto the screen.
	virtual void onRender() = 0;

	// Called every frame. A frame can take any number of time before they can
	// be processed. 
	virtual void onUpdate() = 0;
	
	// Called on a fixed cycle of 60x per second.
	virtual void onFixedUpdate() = 0;
	
	// Destroy the object.
	virtual void onDestroy() = 0;

};

#endif // !ENTITY_H