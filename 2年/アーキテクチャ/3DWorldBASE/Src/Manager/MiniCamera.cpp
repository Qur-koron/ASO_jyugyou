#include "../Application.h"
#include "Camera.h"
#include "MiniCamera.h"

MiniCamera::MiniCamera(Camera* camera)
{
	camera_ = camera;
	size_ = Vector2(SIZE_X, SIZE_Y);
	miniScreen_ = MakeScreen(size_.x, size_.y);
	modelId_ = -1;
	pos_ = { 0.0f, 0.0f, 0.0f };
}

MiniCamera::~MiniCamera(void)
{
}

void MiniCamera::Init(void)
{

	// ミニカメラモデルの読み込み
	modelId_ = MV1LoadModel((Application::PATH_MODEL + "Camera.mv1").c_str());

	// ミニカメラモデルの大きさを設定
	MV1SetScale(modelId_, { 3.0f, 3.0f, 3.0f });

	// ミニカメラモデルの位置を設定
	pos_ = { 0.0f, 0.0f, 0.0f };
	MV1SetPosition(modelId_, pos_);

}

void MiniCamera::Update(void)
{
	// 使用しているカメラの角度に常に合わせる
	MV1SetRotationXYZ(modelId_, camera_->GetAngles());
}

void MiniCamera::DrawScreen(void)
{

	// 描画領域を変更
	SetDrawScreen(miniScreen_);

	// 画面を初期化
	ClearDrawScreen();

	// ミニカメラを写すカメラの設定
	VECTOR pos = pos_;

	// ミニカメラモデルの手前
	pos.z -= 500.0f;

	// カメラ位置をセット
	SetCameraPositionAndAngle(
		pos,
		0.0f,
		0.0f,
		0.0f
	);

	// ライトの設定
	ChangeLightTypeDir({ 0.0f, -1.0f, 0.0f });

	// ミニカメラモデルを描画
	MV1DrawModel(modelId_);

	// ライトの設定
	ChangeLightTypeDir({ 0.3f, -0.7f, 0.8f });

}

void MiniCamera::Draw(void)
{

	// 画面右上に枠表示
	int width = 10;
	DrawBox(
		Application::SCREEN_SIZE_X - size_.x - (width * 2),
		0,
		Application::SCREEN_SIZE_X,
		size_.y + (width * 2),
		0x000000,
		true
	);

	// 枠内にミニカメラを写した画像を描画
	DrawGraph(Application::SCREEN_SIZE_X - size_.x - width, width, miniScreen_, false);

}

void MiniCamera::Release(void)
{
	MV1DeleteModel(modelId_);
}

int MiniCamera::GetScreen(void) const
{
	return miniScreen_;
}
