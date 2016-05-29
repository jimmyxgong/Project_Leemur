#pragma once

#include "Commons.h"
#include "BaseEntity.h"
#include "Material.h"

class Cell : BaseEntity {
public:
	struct Data {
	public:
		unsigned int type = GRASS;
		bool transparent = false;
		bool voidable = false;
	} data;

public:
	void onRender() override;

public:

	unsigned int getType() const;
	bool isTransparent() const;
	
	/* Only one type of cell can be negligible: voidables */
	bool isNegligible() const;

};