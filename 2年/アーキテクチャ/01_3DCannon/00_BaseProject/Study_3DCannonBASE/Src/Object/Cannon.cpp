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
	// StandÉÇÉfÉãì«Ç›çûÇ›
	// äOïîÉtÉ@ÉCÉãÇÃÇRÇcÉÇÉfÉãÇÉçÅ[Éh
	standModelId_ = MV1LoadModel(
		(Application::PATH_MODEL + "Cannon/Stand.mv1").c_str());
	standScl_ = { 0.8f, 0.8f, 0.8f };
	standPos_ = { 0.0f, 10.0f, -200.0f };
	standRot_ = { 0.0f, 0.0f, 0.0f };

	// BarrelÉÇÉfÉãì«Ç›çûÇ›
	// äOïîÉtÉ@ÉCÉãÇÃÇRÇcÉÇÉfÉãÇÉçÅ[Éh
	barrelModelId_ = MV1LoadModel(
		(Application::PATH_MODEL + "Cannon/Barrel.mv1").c_str());
	barrelScl_ = { 0.8f, 0.8f, 0.8f };
	//barrelPos_ = { 0.0f, 110.0f, -200.0f };
	barrelRot_ = { 0.0f, 0.0f, 0.0f };

	// ìyë‰Ç©ÇÁÇÃëäëŒç¿ïWÇ∆Ç∑ÇÈ
	barrelLocalPos_ = { 0.0f, 100.0f, 0.0f };

	// íeÇÃÉÇÉfÉã
	shotModelId_ =MV1LoadModel(
		(Application::PATH_MODEL + "Cannon/Shot.mv1").c_str());
	// íeî≠éÀÇÃçdíºéûä‘
	stepShotDelay_ = 0.0f;

	// 3DÉÇÉfÉãÇ…êßå‰èÓïÒÇîΩâfÇ≥ÇπÇÈ
	Update();

}

void Cannon::Update(void)
{
	// âÒì]ëÄçÏ
	ProcessPot();

	// î≠éÀëÄçÏ
	ProcessShot();

	// VECTORìØémÇÃâ¡éZ VAddä÷êî
	barrelPos_ = VAdd(standPos_, barrelLocalPos_);

	// ÇRÇcÉÇÉfÉãÇÃëÂÇ´Ç≥Çê›íË(à¯êîÇÕÅAx, y, zÇÃî{ó¶)
	MV1SetScale(standModelId_, standScl_);
	// ÇRÇcÉÇÉfÉãÇÃà íu(à¯êîÇÕÅAÇRÇcç¿ïW)
	MV1SetPosition(standModelId_, standPos_);
	// ÇRÇcÉÇÉfÉãÇÃå¸Ç´(à¯êîÇÕÅAx, y, zÇÃâÒì]ó ÅBíPà ÇÕÉâÉWÉAÉìÅB)
	MV1SetRotationXYZ(standModelId_, standRot_);

	// ÇRÇcÉÇÉfÉãÇÃëÂÇ´Ç≥Çê›íË(à¯êîÇÕÅAx, y, zÇÃî{ó¶)
	MV1SetScale(barrelModelId_, barrelScl_);
	// ÇRÇcÉÇÉfÉãÇÃà íu(à¯êîÇÕÅAÇRÇcç¿ïW)
	MV1SetPosition(barrelModelId_, barrelPos_);
	// ÇRÇcÉÇÉfÉãÇÃå¸Ç´(à¯êîÇÕÅAx, y, zÇÃâÒì]ó ÅBíPà ÇÕÉâÉWÉAÉìÅB)
	MV1SetRotationXYZ(barrelModelId_, barrelRot_);
}

void Cannon::Draw(void)
{
	// íeÇÃî≠éÀà íu
	VECTOR pos = barrelPos_;
	// ñCêgÇ©ÇÁÇÃëäëŒç¿ïW
	VECTOR localPos = { 0.0f, 25.0f, 30.0f };
	// ÉçÅ[ÉJÉãç¿ïWÇ©ÇÁÉèÅ[ÉãÉhç¿ïWÇ÷ïœä∑
	pos = VAdd(pos, localPos);
	// íeÇÃî≠éÀà íuñ⁄à¿
	DrawSphere3D(pos, 10.0f, 10, 0x00ff00, 0x00ff00, true);
	// ñCë‰ÇÃÉÇÉfÉãï`âÊ
	MV1DrawModel(standModelId_);
	// ñCêgÇÃÉÇÉfÉãï`âÊ(àÍéûìIÇ…îºìßñæÇ…Ç∑ÇÈ)
	MV1SetMaterialDifColor(barrelModelId_, 0, GetColorF(1.0f, 1.0f, 1.0f, 0.5f));
	MV1DrawModel(barrelModelId_);

}

void Cannon::Release(void)
{
	// ÉçÅ[ÉhÇ≥ÇÍÇΩÇRÇcÉÇÉfÉãÇÉÅÉÇÉäÇ©ÇÁâï˙
	MV1DeleteModel(standModelId_);

	MV1DeleteModel(barrelModelId_);

	MV1DeleteModel(shotModelId_);
	for (auto shot : shots_)
	{
		shot->Release();
		delete shot;
	}

}

std::vector<ShotBase*> Cannon::GetShots(void)
{
	return shots_;
}

void Cannon::ProcessShot(void)
{
	auto& ins = InputManager::GetInstance();

	// çUåÇÉLÅ[ÇâüÇ∑Ç∆ÅAíeÇê∂ê¨
	if (ins.IsNew(KEY_INPUT_Z) && stepShotDelay_ <= 0.0f)
	{
		// óLå¯Ç»íeÇéÊìæÇ∑ÇÈ
		ShotBase* shot = GetValidShot();

		// íPà çsóÒ(ñ≥âÒì]ÇÃèÛë‘)
		MATRIX matRot = MGetIdent();

		// ÇªÇÍÇºÇÍÇÃé≤ÇÃçsóÒÇçÏÇËÅAçXÇ…çsóÒÇçáê¨ÇµÇƒÇ¢Ç≠
		matRot = MMult(matRot, MGetRotX(barrelRot_.x));
		matRot = MMult(matRot, MGetRotY(barrelRot_.y));
		matRot = MMult(matRot, MGetRotZ(barrelRot_.z));

		//MATRIX matrRotY90 = MMult(matrRotY90, matrRotY90);
		//
		// íeÇê∂ê¨(ï˚å¸ÇÕâºÇ≈ê≥ñ ï˚å¸)
		//shot->CreateShot(barrelPos_, { 0.0f, 0.0f, 1.0f });
		
		// ï˚å¸Ç∆ìØÇ∂óvóÃÇ≈ÅAëäëŒç¿ïWÇâÒì]
		VECTOR localPosRot = VTransform(, matRot);

		pos = VAdd(pos, localPosRot);

		// íeî≠éÀå„ÇÃçdíºéûä‘ÉZÉbÉg
		stepShotDelay_ = SHOT_DELAY;

	}

	// íeî≠éÀå„ÇÃçdíºéûä‘Çå∏ÇÁÇµÇƒÇ¢Ç≠
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
		if (!shots_[i]->IsAlive())
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
	// 
	auto& ins = InputManager::GetInstance();

	// âÒì]ó 
	float rotPow = 2.0f * DX_PI_F / 180.0f;

	//â°âÒì]
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

	// ècâÒì]
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
