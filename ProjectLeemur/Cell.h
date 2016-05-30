#pragma once

#include "Commons.h"
#include "BaseEntity.h"
#include "Material.h"

class Chunk;

class Cell : BaseEntity {
public:


public:
	struct Data {
	public:
		unsigned int type = GRASS;
		bool transparent = false;
		bool voidable = false;
	} data;

	Chunk * chunk;

public:
	void onRender() override;

	bool operator==(Cell const &) const;
	unsigned int getType() const;
	bool isTransparent() const;
	
	/* Only one type of cell can be negligible: voidables */
	bool isNegligible() const;

public:
	static Cell Air;

};