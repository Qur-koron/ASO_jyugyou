#pragma once
#include <DxLib.h>

class Camera
{

public:

	// カメラモード
	enum class MODE
	{
	NONE, 
	FIXED_POINT,// 定点カメラ
	FREE,// フリーモード
	};

	Camera(void);
	~Camera(void);

	void Init(void);
	void Update(void);
	void SetBeforeDraw(void);
	void SetBeforeDrawFixedPoint(void);
	void SetBeforeDrawFree(void);
	void Draw(void);
	void Release(void);

	VECTOR GetPos(void) const;
	VECTOR GetAngles(void) const;

	// カメラモードの変更
	void ChangeMode(MODE mode);

private:

	// カメラモード
	MODE mode_;
	// カメラの位置
	VECTOR pos_;
	// カメラの角度(rad)
	VECTOR angles_;

	void MoveXYZDirection(void);

	void MoveXZAngle(void);

};

