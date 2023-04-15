#include "DiamondView.h"
#include "math.h"

DiamondView::DiamondView()
{
}

void DiamondView::calculoPosicaoDesenho(int col, int row, const float tw, const float th, float & x, float & y) const {
	x = (col * (tw / 2) + row * (tw / 2)) - 1.0f;  //- 1.0f serve para posicionar o desenho no canto da tela
	y = col * (th / 2) - row * (th / 2);
}

void DiamondView::MouseMap(int & col, int & row, const float tw, const float th, const float mx, const float my) const {
	col = ((2.0f * mx / tw) + (2.0f * my / th)) / 2.0f;
	row = (-2.0f * my / th + col) + 0.5f;
}

void DiamondView::TileWalking(int & col, int & row, const int direction) const {
	switch (direction) {
	case DIRECTION_NORTH: col++; row--; break;
	case DIRECTION_SOUTH: col--; row++; break;
	case DIRECTION_WEST: col--; row--; break;
	case DIRECTION_EAST: col++; row++; break;
	case DIRECTION_NORTHEAST: col++; break;
	case DIRECTION_SOUTHEAST: row++; break;
	case DIRECTION_NORTHWEST: row--; break;
	case DIRECTION_SOUTHWEST: col--; break;
	}
}