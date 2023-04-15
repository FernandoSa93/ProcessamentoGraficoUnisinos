#pragma once
#include "TileMapView.h"

class DiamondView : public TileMapView {
public:
	DiamondView();

	void calculoPosicaoDesenho(int col, int row, const float tw, const float th, float &x, float &y) const override;
	void MouseMap(int &col, int &row, const float tw, const float th, const float mx, const float my) const override;
	void TileWalking(int &col, int &row, const int direction) const override;
};