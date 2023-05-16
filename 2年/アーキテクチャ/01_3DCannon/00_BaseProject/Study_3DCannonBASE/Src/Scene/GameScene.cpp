#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Object/Stage.h"
#include "../Object/Cannon.h"
#include "../Object/EnemyBase.h"
#include "../Object/ShotBase.h"
#include "GameScene.h"

GameScene::GameScene(void)
{
}

GameScene::~GameScene(void)
{
}

void GameScene::Init(void)
{
	Camera* camera = SceneManager::GetInstance().GetCamera();
	camera->ChangeMode(Camera::MODE::FIXED_POINT);

	stage_ = new Stage();
	stage_->Init();

	cannon_ = new Cannon();
	cannon_->Init();

	// 敵のモデル
	enemyModelId_ = MV1LoadModel(
		(Application::PATH_MODEL + "Enemy/Birb.mv1").c_str());
	auto enemy = new EnemyBase(enemyModelId_);
	enemy->Init();
	enemys_.push_back(enemy);

	// ゲームオーバー地点
	gameoverPoint_ = { 450.0f, 30.0f, 75.0f };

	// ゲームオーバー判定
	isGameover_ = false;

	// ゲームオーバー画像
	imgGameover_ = LoadGraph((Application::PATH_IMAGE + "Gameover.png").c_str());

	// カメラをフリーモードにする
	//SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FREE);

}

void GameScene::Update(void)
{
	// シーン遷移
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}
	if (isGameover_)
	{
		// ゲームオーバーなら処理しない
		return;
	}

	stage_->Update();

	cannon_->Update();

	int stageModelId = stage_->GetModelId();

	auto shots = cannon_->GetShots();
	for (auto shot : shots)
	{
		shot->Update();

		// 弾がSHOT状態じゃなかったら衝突判定しない
		if (!shot->IsShot())
		{
			// 爆発中や処理終了後は、以降の処理は実行しない
			continue;
		}

		// ステージモデルと球体の衝突判定
		auto info = MV1CollCheck_Sphere(stageModelId, -1, shot->GetPos(), ShotBase::COL_RADIUS);
		if (info.HitNum > 0)
		{
			// 衝突している
			shot->Blast();
		}

		// 当たり判定結果ポリゴン配列の後始末をする
		MV1CollResultPolyDimTerminate(info);

		// 敵との衝突判定
		for (auto enemy : enemys_)
		{
			// 球体と球体の衝突判定
			if(AsoUtility::IsHitSpheres(
				enemy->GetCollisionPos(), enemy->GetCollisionRadius(), 
				shot->GetPos(), shot->COL_RADIUS))
			{
				// 敵にダメージを与える
				enemy->Damage(1);
				shot->Blast();
				break;
			}
		}
	}

	for (auto enemy : enemys_)
	{
		enemy->Update();

		// ゲームオーバー判定
		// 球体と球体の衝突判定
		if (AsoUtility::IsHitSpheres(
			enemy->GetCollisionPos(), enemy->GetCollisionRadius(),
			gameoverPoint_, OVER_COL_RADIUS))
		{
			isGameover_ = true;
			break;
		}
	}

}

void GameScene::Draw(void)
{
	stage_->Draw();

	cannon_->Draw();

	for (auto enemy : enemys_)
	{
		enemy->Draw();
	}

	auto shots = cannon_->GetShots();
	for (auto shot : shots)
	{
		shot->Draw();
		// デバッグ用
		if (shot->IsAlive())
		{
			DrawSphere3D(shot->GetPos(), shot->COL_RADIUS, 10, 0xff0000, 0xff0000, false);
		}
	}

	// デバッグ用
	for (auto enemy : enemys_)
	{
		DrawSphere3D(enemy->GetCollisionPos(), enemy->GetCollisionRadius(), 10, 0xff0000, 0xff0000, false);
	}

	// デバッグ用
	DrawSphere3D(gameoverPoint_, OVER_COL_RADIUS, 10, 0xff0000, 0xff0000, false);

	// ゲームオーバー画像の表示
	if (isGameover_)
	{
		// ゲームオーバー画像
		DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, 1.0, 0.0, imgGameover_, true);
	}

}

void GameScene::Release(void)
{
	stage_->Release();
	delete stage_;

	cannon_->Release();
	delete cannon_;

	for (auto enemy : enemys_)
	{
		enemy->Release();
	}

}
