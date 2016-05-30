#include "BaseEntity.h"


void BaseEntity::onCreate() {}
void BaseEntity::onStart() {}
void BaseEntity::onRender() {}
void BaseEntity::onUpdate() {}
void BaseEntity::onFixedUpdate() {}
void BaseEntity::onDestroy() {
	active = false;
}

BaseEntity::~BaseEntity() {
	// If onDestroy is called before, don't
	// call it again.
	if (isActive()) onDestroy();
}
