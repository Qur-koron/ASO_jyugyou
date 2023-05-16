#include<DxLib.h>
#include<math.h>
#include"Geometry.h"

/// <summary>
/// 2つの矩形が重なってるか銅貨を判定する
/// </summary>
/// <param name="rcA">矩形A</param>
/// <param name="rcB">矩形B</param>
/// <returns>true：重なってる / false：離れている</returns>
/// 
bool IsHit(const Rect& rcA, const Rect& rcB)
{
	// x方向が離れている
	return!(fabsf(rcA.pos.x - rcB.pos.x) > static_cast<float>(rcA.size.w + rcB.size.w) ||
		fabsf(rcA.pos.y - rcB.pos.y) > static_cast<float>(rcA.size.h + rcB.size.h));

	//// y方向が離れている
	//if (fabsf(rcA.pos.y - rcB.pos.y) > static_cast<float>(rcA.size.h + rcB.size.h)) {
	//	return false;
	//}
	//return true;

}

bool IsHit(const Circle& cA, const Circle& cB)
{
	auto vec = cB.pos - cA.pos;
	float len = vec.Length();
	float radius2 = cA.radius + cB.radius;

	return len <= radius2;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	SetWindowText(L"2216009_徳安歩美");//GitHub GPIChact
	ChangeWindowMode(true);
	if (DxLib_Init() == -1)
	{
		return -1;
	}

	SetDrawScreen(DX_SCREEN_BACK);

	//Vector2 a = { 3,5 };
	//Vector2 b = { 4,9 };
	//auto c = a + b;
	//auto d = a - b;
	//auto e = a * 2 + b * 3;
	//auto f = a / 2.0f;
	Rect pop = { 100,200,50,50 };
	Rect pip = Rect::CreateRect(250, 250, 100, 200); 

	char ketatate[256];
	constexpr float speed = 2.0f;

	while (ProcessMessage() != -1) {
		GetHitKeyStateAll(ketatate);
		ClearDrawScreen();

		if (ketatate[KEY_INPUT_LEFT]) {
			pop.pos.x -= speed;
		}
		if (ketatate[KEY_INPUT_RIGHT]) {
			pop.pos.x += speed;
		}
		if (ketatate[KEY_INPUT_UP]) {
			pop.pos.y -= speed;
		}
		if (ketatate[KEY_INPUT_DOWN]) {
			pop.pos.y += speed;
		}

		unsigned int color = 0xffffffff;	// 白

		if (IsHit(pop, pip)) 
		{
			color = 0xff8888;	// ピンク	
			// めり込み量の計算
			float diffX = (pip.pos.x - pop.pos.x);
			float signX = diffX / fabsf(diffX);

			float diffY = (pip.pos.y - pop.pos.y);
			float signY = diffY / fabsf(diffY);

			float overlapX = (pop.size.w * pip.size.w) - fabs(diffX);
			float overlapY = (pop.size.h * pip.size.w) - fabs(diffX);

			if (fabsf(diffY) < fabs(diffX)) 
			{
				pop.pos.x -= ((pop.size.w + pip.size.w) - fabs(diffX)) * signX;
			}
			else
			{
				pop.pos.y -= ((pop.size.h + pip.size.h) - fabs(diffY)) * signY;
			}
	
		}

		pop.Draw(color);
		pip.Draw(color);

		ScreenFlip();

	}
	DxLib_End();
	return 0;

}

