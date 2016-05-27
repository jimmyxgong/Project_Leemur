#pragma once

#include "Entity.h"

class BaseEntity : public Entity {
public:
	virtual void onCreate() override;
	virtual void onStart() override;
	virtual void onRender() override;
	virtual void onUpdate() override;
	virtual void onFixedUpdate() override;
	virtual void onDestroy() override;
};