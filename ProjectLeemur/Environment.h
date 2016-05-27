#pragma once

#include "Commons.h"
#include "BaseEntity.h"
#include "Window.h"

class Environment : public BaseEntity {
private:
	Window * window;

public:
	void onCreate() override;
	void onStart() override;
	void onRender() override;
	void onUpdate() override;
	void onDestroy() override;

	static UniquePointer<Environment> create(Window * ref);
};