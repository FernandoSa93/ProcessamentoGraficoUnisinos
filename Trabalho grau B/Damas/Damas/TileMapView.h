#pragma once
#define DIRECTION_NORTH 1
#define DIRECTION_SOUTH 2
#define DIRECTION_EAST 3
#define DIRECTION_WEST 4
#define DIRECTION_NORTHEAST 5
#define DIRECTION_NORTHWEST 6
#define DIRECTION_SOUTHEAST 7
#define DIRECTION_SOUTHWEST 8

class TileMapView {
public:
	virtual void calculoPosicaoDesenho(int col, int row, const float tw, const float th, float &x, float &y) const = 0;
	virtual void MouseMap(int &col, int &row, const float tw, const float th, const float mx, const float my) const = 0;
	virtual void TileWalking(int &col, int &row, const int direction) const = 0;
};


