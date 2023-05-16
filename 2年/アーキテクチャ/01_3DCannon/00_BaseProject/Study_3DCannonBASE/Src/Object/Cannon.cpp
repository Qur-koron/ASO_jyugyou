#include <DxLib.h>
#include "../Application.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "ShotBase.h"
#include "Cannon.h"

Cannon::Cannon(void)
{
}

Cannon::~Cannon(void)
{
}

void Cannon::Init(void)
{
	// Stand���f���ǂݍ���
	// �O���t�@�C���̂R�c���f�������[�h
	standModelId_ = MV1LoadModel(
		(Application::PATH_MODEL + "Cannon/Stand.mv1").c_str());
	standScl_ = { 0.8f, 0.8f, 0.8f };
	standPos_ = { 0.0f, 10.0f, -200.0f };
	standRot_ = { 0.0f, 0.0f, 0.0f };

	// �y�䂩��̑��΍��W�Ƃ���
	barrelLocalPos_ = { 0.0f, 100.0f, 0.0f };

	// Barrel���f���ǂݍ���
	// �O���t�@�C���̂R�c���f�������[�h
	barrelModelId_ = MV1LoadModel(
		(Application::PATH_MODEL + "Cannon/Barrel.mv1").c_str());
	barrelScl_ = { 0.8f, 0.8f, 0.8f };
	//barrelPos_ = { 0.0f, 110.0f, -200.0f };
	// ����
	barrelPos_ = VAdd(standPos_, barrelLocalPos_);
	barrelRot_ = { 0.0f, 0.0f, 0.0f };

	// �e�̃��f��
	shotModelId_ =MV1LoadModel(
		(Application::PATH_MODEL + "Cannon/Shot.mv1").c_str());
	// �e���˂̍d������
	stepShotDelay_ = 0.0f;

	// �����G�t�F�N�g�ǂݍ���
	LoadDivGraph((Application::PATH_IMAGE + "Blast.png").c_str(),
		BLAST_ANIM_NUM, 4, 4, BLAST_SIZE_X, BLAST_SIZE_Y, blastImgs_, true);

	// 3D���f���ɐ�����𔽉f������
	Update();

}

void Cannon::Update(void)
{
	// ��]����
	ProcessPot();

	// ���ˑ���
	ProcessShot();

	// �R�c���f���̑傫����ݒ�(�����́Ax, y, z�̔{��)
	MV1SetScale(standModelId_, standScl_);
	// �R�c���f���̈ʒu(�����́A�R�c���W)
	MV1SetPosition(standModelId_, standPos_);
	// �R�c���f���̌���(�����́Ax, y, z�̉�]�ʁB�P�ʂ̓��W�A���B)
	MV1SetRotationXYZ(standModelId_, standRot_);

	// �R�c���f���̑傫����ݒ�(�����́Ax, y, z�̔{��)
	MV1SetScale(barrelModelId_, barrelScl_);
	// �R�c���f���̈ʒu(�����́A�R�c���W)
	MV1SetPosition(barrelModelId_, barrelPos_);
	// �R�c���f���̌���(�����́Ax, y, z�̉�]�ʁB�P�ʂ̓��W�A���B)
	MV1SetRotationXYZ(barrelModelId_, barrelRot_);
}

void Cannon::Draw(void)
{
	// �e�̔��ˈʒu
	//VECTOR pos = barrelPos_;
	// �C�g����̑��΍��W
	//VECTOR localPos = { 0.0f, 25.0f, 30.0f };
	// ���[�J�����W���烏�[���h���W�֕ϊ�
	//pos = VAdd(pos, localPos);
	// �e�̔��ˈʒu�ڈ�
	//DrawSphere3D(pos, 10.0f, 10, 0x00ff00, 0x00ff00, true);
	// �C��̃��f���`��
	MV1DrawModel(standModelId_);
	// �C�g�̃��f���`��(�ꎞ�I�ɔ������ɂ���)
	//MV1SetMaterialDifColor(barrelModelId_, 0, GetColorF(1.0f, 1.0f, 1.0f, 0.5f));
	MV1DrawModel(barrelModelId_);

}

void Cannon::Release(void)
{
	// ���[�h���ꂽ�R�c���f����������������
	MV1DeleteModel(standModelId_);

	MV1DeleteModel(barrelModelId_);

	MV1DeleteModel(shotModelId_);
	for (auto shot : shots_)
	{
		shot->Release();
		delete shot;
	}

	// �ǂݍ��񂾉摜�̉��
	for (int i = 0; i < BLAST_ANIM_NUM; i++)
	{
		DeleteGraph(blastImgs_[i]);
	}

}

std::vector<ShotBase*> Cannon::GetShots(void)
{
	return shots_;
}

void Cannon::ProcessShot(void)
{
	auto& ins = InputManager::GetInstance();

	// �U���L�[�������ƁA�e�𐶐�
	if (ins.IsNew(KEY_INPUT_Z) && stepShotDelay_ <= 0.0f)
	{
		// �L���Ȓe���擾����
		ShotBase* shot = GetValidShot();

		// �P�ʍs��(����]�̏��)
		MATRIX matRot = MGetIdent();

		// ���ꂼ��̎��̍s������A�X�ɍs����������Ă���
		matRot = MMult(matRot, MGetRotX(barrelRot_.x));
		matRot = MMult(matRot, MGetRotY(barrelRot_.y));
		matRot = MMult(matRot, MGetRotZ(barrelRot_.z));

		//MATRIX matrRotY90 = MMult(matrRotY90, matrRotY90);
		//
		// �e�𐶐�(�����͉��Ő��ʕ���)
		//shot->CreateShot(barrelPos_, { 0.0f, 0.0f, 1.0f });
		
		// ��]�s����g�p���āA�x�N�g������]������
		VECTOR dir = VTransform({ 0.0f, 0.0f, 1.0f }, matRot);

		// �e�̔��ˈʒu
		VECTOR pos = barrelPos_;

		// �C�g����̑��΍��W
		VECTOR localPos = { 0.0f, 25.0f, 30.0f };

		// �����Ɠ����v�̂ŁA���΍��W����]
		VECTOR localPosRot = VTransform(localPos, matRot);

		// ���[�J�����W���烏�[���h���W�֕ϊ�
		pos = VAdd(pos, localPosRot);

		// �e�𐶐�(�����͉��Ő��ʕ���)
		shot->CreateShot(pos, dir);

		// �e���ˌ�̍d�����ԃZ�b�g
		stepShotDelay_ = SHOT_DELAY;

	}

	// �e���ˌ�̍d�����Ԃ����炵�Ă���
	if (stepShotDelay_ > 0.0f)
	{
		stepShotDelay_ -= 1.0f / SceneManager::DEFAULT_FPS;
	}

}

ShotBase* Cannon::GetValidShot(void)
{
	size_t size = shots_.size();
	for (int i = 0; i < size; i++)
	{
		if (!shots_[i]->IsShot())
		{
			return shots_[i];
		}
	}

	ShotBase* shot = new ShotBase(shotModelId_, blastImgs_, BLAST_ANIM_NUM);
	shots_.push_back(shot);

	return shot;

}

void Cannon::ProcessPot(void)
{
	// InputManager& ins = InputManager::GetInstance();
	auto& ins = InputManager::GetInstance();

	// ��]��
	float rotPow = 2.0f * DX_PI_F / 180.0f;

	//����]
	if (ins.IsNew(KEY_INPUT_RIGHT))
	{
		standRot_.y += rotPow;
		barrelRot_.y = standRot_.y;
	}

	if (ins.IsNew(KEY_INPUT_LEFT))
	{
		standRot_.y -= rotPow;
		barrelRot_.y = standRot_.y;
	}

	// �c��]
	if (ins.IsNew(KEY_INPUT_UP))
	{
		barrelRot_.x -= rotPow;
		if (barrelRot_.x < -MAX_ANGLE_X)
		{
			barrelRot_.x = -MAX_ANGLE_X;
		}
	}

	if (ins.IsNew(KEY_INPUT_DOWN))
	{
		barrelRot_.x += rotPow;
		if (barrelRot_.x > MAX_ANGLE_X)
		{
			barrelRot_.x = MAX_ANGLE_X;
		}
	}

}
