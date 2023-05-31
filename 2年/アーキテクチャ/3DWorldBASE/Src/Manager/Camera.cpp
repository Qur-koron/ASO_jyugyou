#include "../Utility/AsoUtility.h"
#include "../Manager/InputManager.h"
#include "Camera.h"

Camera::Camera(void)
{
	mode_ = MODE::NONE;
	pos_ = { 0.0f, 0.0f, 0.0f };
	angles_ = { 0.0f, 0.0f, 0.0f };
}

Camera::~Camera(void)
{
}

void Camera::Init(void)
{
	// カメラの初期位置
	pos_ = { 0.0f, 200.0f, -500.0f };

	// カメラの初期角度
	angles_ = { 0.0f, 0.0f, 0.0f };
	angles_.x = AsoUtility::Deg2RadF(30.0f);

}

void Camera::Update(void)
{
}

void Camera::SetBeforeDraw(void)
{
	// クリップ距離を設定する(SetDrawScreenでリセットされる)
	SetCameraNearFar(10.0f, 30000.0f);
	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		SetBeforeDrawFixedPoint();
		break;
	case Camera::MODE::FREE:
		SetBeforeDrawFree();
		break;
	}
	
	// カメラの設定(位置と角度による制御)
	SetCameraPositionAndAngle(pos_,angles_.x,angles_.y,angles_.z);

}

void Camera::SetBeforeDrawFixedPoint(void) 
{
	// 何もしない
}

void Camera::SetBeforeDrawFree(void)
{
	MoveXYZDirection();
	
	MoveXZAngle();

	//auto& ins = InputManager::GetInstance();
	//// WASDでカメラの位置を変える
	//float movePow = 3.0f;
	//if (ins.IsNew(KEY_INPUT_W))
	//{
	//	pos_.z += movePow;
	//}
	//if (ins.IsNew(KEY_INPUT_A)) 
	//{
	//	pos_.x -= movePow;
	//}
	//if (ins.IsNew(KEY_INPUT_S))
	//{
	//	pos_.z -= movePow;
	//}
	//if (ins.IsNew(KEY_INPUT_D))
	//{
	//	pos_.x += movePow;
	//}
	//if (ins.IsNew(KEY_INPUT_Q))
	//{
	//	pos_.y += movePow;
	//}
	//if (ins.IsNew(KEY_INPUT_E))
	//{
	//	pos_.y -= movePow;
	//}
	//// 矢印キーでカメラの角度を変える
	//float rotPow = 1.0f * DX_PI_F / 180.0f;
	//if (ins.IsNew(KEY_INPUT_DOWN))
	//{
	//	angles_.x += rotPow;
	//}
	//if (ins.IsNew(KEY_INPUT_UP))
	//{
	//	angles_.x -= rotPow;
	//}
	//if (ins.IsNew(KEY_INPUT_RIGHT))
	//{
	//	angles_.y += rotPow;
	//}
	//if (ins.IsNew(KEY_INPUT_LEFT))
	//{
	//	angles_.y -= rotPow; 
	//}

}

void Camera::Draw(void)
{
}

void Camera::Release(void)
{
}

VECTOR Camera::GetPos(void) const
{
	return pos_;
}

VECTOR Camera::GetAngles(void) const
{
	return angles_;
}

void Camera::ChangeMode(MODE mode)
{
	// カメラモードの変更
	mode_ = mode;
	// 変更時の初期化処理
	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		break;
	case Camera::MODE::FREE:
		break;
	}
}

void Camera::MoveXYZDirection(void)
{
	auto& ins = InputManager::GetInstance();

	float movePow = 3.0f;

	VECTOR dir = AsoUtility::VECTOR_ZERO;

	// WASDでカメラの位置を変える
	if (ins.IsNew(KEY_INPUT_W))
	{
		dir = { 0.0f, 0.0f, 1.0f };
	}
	if (ins.IsNew(KEY_INPUT_A))
	{
		dir = { -1.0f, 0.0f, 0.0f };
	}
	if (ins.IsNew(KEY_INPUT_S))
	{
		dir = { 0.0f, 0.0f, -1.0f };
	}
	if (ins.IsNew(KEY_INPUT_D))
	{
		dir = { 1.0f, 0.0f, 0.0f };
	}
	if (!AsoUtility::EqualsVZero(dir))
	{
		// XYZの回転行列
		// XZ平面移動にする場合は、XZの回転を考慮しないようにする
		MATRIX mat = MGetIdent();
		//mat = MMult(mat, MGetRotX(angles_.x));
		mat = MMult(mat, MGetRotY(angles_.y));
		//mat = MMult(mat, MGetRotZ(angles_.z));

		// 回転行列を使用して、ベクトルを回転させる
		VECTOR moveDir = VTransform(dir, mat);

		// 方向×スピードで移動量を作って、座標に足して移動
		pos_ = VAdd(pos_, VScale(moveDir, movePow));

	}

	if (ins.IsNew(KEY_INPUT_Q))
	{
		pos_.y += movePow;
	}
	if (ins.IsNew(KEY_INPUT_E))
	{
		pos_.y -= movePow;
	}

	// 矢印キーでカメラの角度を変える
	float rotPow = 1.0f * DX_PI_F / 180.0f;
	if (ins.IsNew(KEY_INPUT_DOWN))
	{
		angles_.x += rotPow;
	}
	if (ins.IsNew(KEY_INPUT_UP))
	{
		angles_.x -= rotPow;
	}
	if (ins.IsNew(KEY_INPUT_RIGHT))
	{
		angles_.y += rotPow;
	}
	if (ins.IsNew(KEY_INPUT_LEFT))
	{
		angles_.y -= rotPow;
	}

}

void Camera::MoveXZAngle(void)
{

}
