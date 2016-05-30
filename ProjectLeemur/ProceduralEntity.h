#pragma once

#include "Component.h"

class ProceduralEntity : public Component {
public:
	Transform transform;

public:
	virtual void onCreate() override;
	virtual void onStart() override;
};