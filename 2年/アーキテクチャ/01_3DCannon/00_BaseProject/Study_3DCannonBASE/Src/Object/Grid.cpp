#include "Grid.h"
#include <DxLib.h>

Grid::Grid(void)
{
}

Grid::~Grid(void)
{
}

void Grid::Init(void)
{
}

void Grid::Update(void)
{
}

void Grid::Draw(void)
{
	// Åyó˚èKÅzç≈èâÇÃÇPñ{
	//VECTOR sPos = { -HLEN, 0.0f, 0.0f };
	//VECTOR ePos = { HLEN, 0.0f, 0.0f };
	//DrawLine3D(sPos, ePos, 0xff0000);

	VECTOR sPos = { 0.0f, -HLEN, 0.0f };
	VECTOR ePos = { 0.0f, HLEN, 0.0f };
	DrawLine3D(sPos, ePos, 0x00ff00);

	for (float pos = HLEN; pos < -HLEN; pos -= TERM)
	{
		VECTOR sPos = { -HLEN, 0.0f, pos };
		VECTOR ePos = { HLEN, 0.0f, pos };
		DrawLine3D(sPos, ePos, 0xff0000);

		sPos = { pos, 0.0f, -HLEN };
		ePos = { pos, 0.0f, HLEN };
		DrawLine3D(sPos, ePos, 0x0000ff);
	}

}

void Grid::Release(void)
{
}
