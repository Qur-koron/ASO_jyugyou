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

	// �G�̃��f��
	enemyModelId_ = MV1LoadModel(
		(Application::PATH_MODEL + "Enemy/Birb.mv1").c_str());
	auto enemy = new EnemyBase(enemyModelId_);
	enemy->Init();
	enemys_.push_back(enemy);

	// �Q�[���I�[�o�[�n�_
	gameoverPoint_ = { 450.0f, 30.0f, 75.0f };

	// �Q�[���I�[�o�[����
	isGameover_ = false;

	// �Q�[���I�[�o�[�摜
	imgGameover_ = LoadGraph((Application::PATH_IMAGE + "Gameover.png").c_str());

	// �J�������t���[���[�h�ɂ���
	//SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FREE);

}

void GameScene::Update(void)
{
	// �V�[���J��
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}
	if (isGameover_)
	{
		// �Q�[���I�[�o�[�Ȃ珈�����Ȃ�
		return;
	}

	stage_->Update();

	cannon_->Update();

	int stageModelId = stage_->GetModelId();

	auto shots = cannon_->GetShots();
	for (auto shot : shots)
	{
		shot->Update();

		// �e��SHOT��Ԃ���Ȃ�������Փ˔��肵�Ȃ�
		if (!shot->IsShot())
		{
			// �������⏈���I����́A�ȍ~�̏����͎��s���Ȃ�
			continue;
		}

		// �X�e�[�W���f���Ƌ��̂̏Փ˔���
		auto info = MV1CollCheck_Sphere(stageModelId, -1, shot->GetPos(), ShotBase::COL_RADIUS);
		if (info.HitNum > 0)
		{
			// �Փ˂��Ă���
			shot->Blast();
		}

		// �����蔻�茋�ʃ|���S���z��̌�n��������
		MV1CollResultPolyDimTerminate(info);

		// �G�Ƃ̏Փ˔���
		for (auto enemy : enemys_)
		{
			// ���̂Ƌ��̂̏Փ˔���
			if(AsoUtility::IsHitSpheres(
				enemy->GetCollisionPos(), enemy->GetCollisionRadius(), 
				shot->GetPos(), shot->COL_RADIUS))
			{
				// �G�Ƀ_���[�W��^����
				enemy->Damage(1);
				shot->Blast();
				break;
			}
		}
	}

	for (auto enemy : enemys_)
	{
		enemy->Update();

		// �Q�[���I�[�o�[����
		// ���̂Ƌ��̂̏Փ˔���
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
		// �f�o�b�O�p
		if (shot->IsAlive())
		{
			DrawSphere3D(shot->GetPos(), shot->COL_RADIUS, 10, 0xff0000, 0xff0000, false);
		}
	}

	// �f�o�b�O�p
	for (auto enemy : enemys_)
	{
		DrawSphere3D(enemy->GetCollisionPos(), enemy->GetCollisionRadius(), 10, 0xff0000, 0xff0000, false);
	}

	// �f�o�b�O�p
	DrawSphere3D(gameoverPoint_, OVER_COL_RADIUS, 10, 0xff0000, 0xff0000, false);

	// �Q�[���I�[�o�[�摜�̕\��
	if (isGameover_)
	{
		// �Q�[���I�[�o�[�摜
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
