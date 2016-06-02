#include "Cell.h"
#include "TransparentCell.h"
#include "Chunk.h"

Cell Cell::Air = TransparentCell();

//const Vector3f 

void Cell::onRender() {
	// locked. Don't use.
}

void Cell::draw(Mesh & mesh, Chunk & chunk, Vector3f const & pos) {
	if (isTransparent()) return;

	if (chunk.isTransparentAt(pos.x, pos.y - 1, pos.z)) {

	}

}


bool Cell::operator==(Cell const & other) const {
	return getType() == other.getType();
}

unsigned int Cell::getType() const {
	return data.type;
}

bool Cell::isTransparent() const {
	return data.transparent;
}

bool Cell::isNegligible() const {
	return data.voidable;
}