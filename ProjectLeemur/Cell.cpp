#include "Cell.h"
#include "TransparentCell.h"
#include "Chunk.h"

const Cell Cell::Air = TransparentCell();

void Cell::onRender() {
	// locked. Don't use.
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