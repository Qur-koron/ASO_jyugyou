#include <string>
#include <DxLib.h>
#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Object/Grid.h"
#include "TitleScene.h"

TitleScene::TitleScene(void)
{
	imgTitle_ = -1;
}

TitleScene::~TitleScene(void)
{
}

void TitleScene::Init(void)
{
	grid_ = new Grid();
	grid_->Init();

	imgTitle_ = LoadGraph((Application::PATH_IMAGE + "Title.png").c_str());
}

void TitleScene::Update(void)
{

	// シーン遷移
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
	}

	grid_->Update();

}

void TitleScene::Draw(void)
{
	// 背景
	grid_->Draw();

	// オブジェクト
	//VECTOR pos = VGet( 0.0f, 0.0f, 0.0f );

	//白い球体
	VECTOR pos = { 0.0f, 0.0f, 0.0f };
	DrawSphere3D(pos, 50.0f, 10, 0xffffff, 0xffffff, true);

	//赤い球体
	pos = { 0.0f, 0.0f, 100.0f };
	DrawSphere3D(pos, 80.0f, 10, 0xff0000, 0xffffff, true);

	// UI
	DrawRotaGraph(
		Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2 - 100,
		1.0f, 0.0, imgTitle_, true);

}

void TitleScene::Release(void)
{
	DeleteGraph(imgTitle_);

	grid_->Release();
	delete grid_;

}
