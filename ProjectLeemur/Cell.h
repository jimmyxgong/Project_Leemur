#pragma once

#include "Commons.h"
#include "BaseEntity.h"
#include "Material.h"
#include "Mesh.h"

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
	void draw(Mesh &, Chunk &, Vector3f const &);

	bool operator==(Cell const &) const;
	unsigned int getType() const;
	bool isTransparent() const;
	
	/* Only one type of cell can be negligible: voidables */
	bool isNegligible() const;

public:
	static Cell Air;

};